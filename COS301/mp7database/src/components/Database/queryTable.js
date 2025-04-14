import React, { useState, useEffect } from 'react';
import '../../index.css';

const QueryTable = ({ onCancel, tables, columns, onQuery }) => {
  const [queries, setQueries] = useState([
    { table: '', column: '', operator: '', value: '' }
  ]);
  const [resultColumns, setResultColumns] = useState([]);
  const [selectedColumns, setSelectedColumns] = useState([]);
  const [joinType, setJoinType] = useState('INNER');
  const [joinConditions, setJoinConditions] = useState([]);
  const [searchTerm, setSearchTerm] = useState('');
  const [error, setError] = useState('');
  const [results, setResults] = useState([]);
  const [isLoading, setIsLoading] = useState(false);

  const operators = {
    number: ['=', '!=', '>', '<', '>=', '<='],
    string: ['=', '!=', 'contains', 'startsWith', 'endsWith'],
    boolean: ['='],
    date: ['=', '!=', '>', '<', '>=', '<=']
  };

  const joinTypes = ['INNER', 'LEFT', 'RIGHT', 'FULL'];

  useEffect(() => {
    const availableColumns = queries
      .filter(q => q.table)
      .flatMap(q => 
        (columns[q.table] || []).map(col => ({
          table: q.table,
          name: col.name,
          type: col.type
        }))
      );
    
    setSelectedColumns(prev => 
      prev.filter(sc => 
        availableColumns.some(ac => ac.table === sc.table && ac.name === sc.name)
      )
    );
  }, [queries, columns]);

  useEffect(() => {
    const uniqueTables = [...new Set(queries.map(q => q.table).filter(Boolean))];
    if (uniqueTables.length > 1 && joinConditions.length === 0) {
      setJoinConditions([{ leftTable: '', leftColumn: '', rightTable: '', rightColumn: '' }]);
    }
  }, [queries]);

  const handleQueryChange = (index, field, value) => {
    const newQueries = [...queries];
    newQueries[index][field] = value;
    
    if (field === 'table') {
      newQueries[index].column = '';
      newQueries[index].operator = '';
      newQueries[index].value = '';
    } else if (field === 'column') {
      newQueries[index].operator = '';
      newQueries[index].value = '';
    }
    
    setQueries(newQueries);
  };

  const groupedColumns = queries
  .filter(q => q.table)
  .reduce((acc, query) => {
    if (!acc[query.table]) {
      acc[query.table] = (columns[query.table] || []).map(col => ({
        name: col.name,
        type: col.type
      }));
    }
    return acc;
  }, {});

  const addQuery = () => {
    setQueries([...queries, { table: '', column: '', operator: '', value: '' }]);
  };

  const removeQuery = (index) => {
    if (queries.length === 1) {
      setQueries([{ table: '', column: '', operator: '', value: '' }]);
    } else {
      setQueries(queries.filter((_, i) => i !== index));
    }
  };

  const addJoinCondition = () => {
    setJoinConditions([...joinConditions, { leftTable: '', leftColumn: '', rightTable: '', rightColumn: '' }]);
  };

  const removeJoinCondition = (index) => {
    if (joinConditions.length === 1) {

      setJoinConditions([{ leftTable: '', leftColumn: '', rightTable: '', rightColumn: '' }]);
    } else {
      setJoinConditions(joinConditions.filter((_, i) => i !== index));
    }
  };

  const handleJoinConditionChange = (index, field, value) => {
    const newConditions = [...joinConditions];
    newConditions[index][field] = value;
    setJoinConditions(newConditions);
  };

  const toggleColumnSelection = (column) => {
    setSelectedColumns(prev => 
      prev.some(c => c.table === column.table && c.name === column.name)
        ? prev.filter(c => !(c.table === column.table && c.name === column.name))
        : [...prev, column]
    );
  };

  const getColumnType = (tableName, columnName) => {
    if (!tableName || !columnName) return 'string';
    const tableColumns = columns[tableName] || [];
    const column = tableColumns.find(col => col.name === columnName);
    return column ? column.type : 'string';
  };

  const getAvailableColumns = (tableName) => {
    if (!tableName) return [];
    return columns[tableName] || [];
  };

  const handleSubmit = async () => {
    const hasInvalidQueries = queries.some(q => 
      !q.table || !q.column || !q.operator || q.value === ''
    );
    
    if (hasInvalidQueries) {
      setError('Please complete all query conditions');
      return;
    }

    const uniqueTables = [...new Set(queries.map(q => q.table).filter(Boolean))];
    if (uniqueTables.length > 1 && joinConditions.length === 0) {
      setError('Please specify join conditions for multiple tables');
      return;
    }

    for (const condition of joinConditions) {
      if (!condition.leftTable || !condition.leftColumn || !condition.rightTable || !condition.rightColumn) {
        setError('Please complete all join conditions');
        return;
      }
    }

    setError('');
    setIsLoading(true);
    
    try {
      const selectClause = selectedColumns.length > 0
        ? selectedColumns.map(col => `${col.table}.${col.name}`).join(', ')
        : '*';
      
      let queryString = `SELECT ${selectClause} FROM ${uniqueTables[0]}`;

      if (uniqueTables.length > 1) {
        for (let i = 1; i < uniqueTables.length; i++) {
          const joinCondition = joinConditions[i - 1];
          queryString += ` ${joinType} JOIN ${uniqueTables[i]} ON `;
          queryString += `${joinCondition.leftTable}.${joinCondition.leftColumn} = `;
          queryString += `${joinCondition.rightTable}.${joinCondition.rightColumn}`;
        }
      }

      const whereClauses = queries
        .filter(q => q.table && q.column && q.operator && q.value !== '')
        .map(q => {
          const columnType = getColumnType(q.table, q.column);
          let value = q.value;
          
          if (columnType === 'string') {
            if (q.operator === 'contains') {
              return `${q.table}.${q.column} LIKE '%${value}%'`;
            } else if (q.operator === 'startsWith') {
              return `${q.table}.${q.column} LIKE '${value}%'`;
            } else if (q.operator === 'endsWith') {
              return `${q.table}.${q.column} LIKE '%${value}'`;
            }
            value = `'${value}'`;
          } else if (columnType === 'date') {
            value = `DATE '${value}'`;
          } else if (columnType === 'boolean') {
            value = value === 'true' ? 'TRUE' : 'FALSE';
          }
          
          return `${q.table}.${q.column} ${q.operator} ${value}`;
        });
      
      if (whereClauses.length > 0) {
        queryString += ' WHERE ' + whereClauses.join(' AND ');
      }
      
      if (searchTerm.trim()) {
        const searchConditions = queries
          .filter(q => q.table)
          .flatMap(q => 
            (columns[q.table] || [])
              .filter(col => col.type === 'string')
              .map(col => `${q.table}.${col.name} LIKE '%${searchTerm}%'`)
          );
          
        if (searchConditions.length > 0) {
          queryString += whereClauses.length > 0 ? ' AND ' : ' WHERE ';
          queryString += '(' + searchConditions.join(' OR ') + ')';
        }
      }

      const queryResults = await onQuery(queryString);
      console.log(queryString);
      setResults(queryResults || []);

      if (queryResults && queryResults.length > 0) {
        setResultColumns(Array.from(new Set(queryResults.flatMap(Object.keys))));
      }
    } catch (err) {
      setError('Error executing query: ' + err.message);
      setResults([]);
    } finally {
      setIsLoading(false);
    }
  };

  const uniqueTablesInQueries = [...new Set(queries.map(q => q.table).filter(Boolean))];

  return (
    <div className="tableContainer">
      <h2 className="tableTitle">Advanced Query Builder</h2>
      
      {error && <div className="errorMessage">{error}</div>}
      
      <div className="inputGroup">
        <div className="inputFieldContainer">
          <label>Global Search</label>
          <input
            type="text"
            placeholder="Search across all results..."
            value={searchTerm}
            onChange={(e) => setSearchTerm(e.target.value)}
            className="inputField"
          />
        </div>
      </div>

      <div className="queryConditionsHorizontal">
        {queries.map((query, index) => (
          <div key={index} className="queryConditionHorizontal">
            <div className="inputFieldContainer">
              <label>Table</label>
              <select
                value={query.table}
                onChange={(e) => handleQueryChange(index, 'table', e.target.value)}
                className="inputField"
              >
                <option value="">Select table</option>
                {tables.map(table => (
                  <option key={table} value={table}>{table}</option>
                ))}
              </select>
            </div>

            <div className="inputFieldContainer">
              <label>Column</label>
              <select
                value={query.column}
                onChange={(e) => handleQueryChange(index, 'column', e.target.value)}
                className="inputField"
                disabled={!query.table}
              >
                <option value="">Select column</option>
                {getAvailableColumns(query.table).map(col => (
                  <option key={col.name} value={col.name}>{col.name}</option>
                ))}
              </select>
            </div>

            <div className="inputFieldContainer">
              <label>Operator</label>
              <select
                value={query.operator}
                onChange={(e) => handleQueryChange(index, 'operator', e.target.value)}
                className="inputField"
                disabled={!query.column}
              >
                <option value="">Select operator</option>
                {query.column && operators[getColumnType(query.table, query.column)]?.map(op => (
                  <option key={op} value={op}>{op}</option>
                ))}
              </select>
            </div>

            <div className="inputFieldContainer">
              <label>Value</label>
              {getColumnType(query.table, query.column) === 'boolean' ? (
                <select
                  value={query.value}
                  onChange={(e) => handleQueryChange(index, 'value', e.target.value)}
                  className="inputField"
                  disabled={!query.operator}
                >
                  <option value="">Select value</option>
                  <option value="true">True</option>
                  <option value="false">False</option>
                </select>
              ) : getColumnType(query.table, query.column) === 'date' ? (
                <input
                  type="date"
                  value={query.value}
                  onChange={(e) => handleQueryChange(index, 'value', e.target.value)}
                  className="inputField"
                  disabled={!query.operator}
                />
              ) : (
                <input
                  type="text"
                  value={query.value}
                  onChange={(e) => handleQueryChange(index, 'value', e.target.value)}
                  className="inputField"
                  disabled={!query.operator}
                />
              )}
            </div>

            {index === queries.length - 1 && (
                <button 
                  onClick={addQuery} 
                  className="actionButton smallButton addButton"
                >
                +
                </button>
              )}
            <div className="queryActions">
              <button 
                onClick={() => removeQuery(index)} 
                className="actionButton smallButton removeButton"
              >
                ×
              </button>
            </div>
          </div>
        ))}
      </div>

      {uniqueTablesInQueries.length > 1 && (
        <div className="queryConditionsHorizontal">        
          <div className="joinButton">
            <label>Join Type</label>
            <select
              value={joinType}
              onChange={(e) => setJoinType(e.target.value)}
              className="inputField"
            >
              {joinTypes.map(type => (
                <option key={type} value={type}>{type} JOIN</option>
              ))}
            </select>
          </div>

          {joinConditions.map((condition, index) => (
            <div key={index} className="queryConditionHorizontal">
              <div className="inputFieldContainer">
                <label>Left Table</label>
                <select
                  value={condition.leftTable}
                  onChange={(e) => handleJoinConditionChange(index, 'leftTable', e.target.value)}
                  className="inputField"
                >
                  <option value="">Select table</option>
                  {uniqueTablesInQueries.map(table => (
                    <option key={`left-${table}`} value={table}>{table}</option>
                  ))}
                </select>
              </div>

              <div className="inputFieldContainer">
                <label>Left Column</label>
                <select
                  value={condition.leftColumn}
                  onChange={(e) => handleJoinConditionChange(index, 'leftColumn', e.target.value)}
                  className="inputField"
                  disabled={!condition.leftTable}
                >
                  <option value="">Select column</option>
                  {getAvailableColumns(condition.leftTable).map(col => (
                    <option key={col.name} value={col.name}>{col.name}</option>
                  ))}
                </select>
              </div>

              <div className="joinEquals">=</div>

              <div className="inputFieldContainer">
                <label>Right Table</label>
                <select
                  value={condition.rightTable}
                  onChange={(e) => handleJoinConditionChange(index, 'rightTable', e.target.value)}
                  className="inputField"
                >
                  <option value="">Select table</option>
                  {uniqueTablesInQueries
                    .filter(table => table !== condition.leftTable)
                    .map(table => (
                      <option key={`right-${table}`} value={table}>{table}</option>
                    ))}
                </select>
              </div>

              <div className="inputFieldContainer">
                <label>Right Column</label>
                <select
                  value={condition.rightColumn}
                  onChange={(e) => handleJoinConditionChange(index, 'rightColumn', e.target.value)}
                  className="inputField"
                  disabled={!condition.rightTable}
                >
                  <option value="">Select column</option>
                  {getAvailableColumns(condition.rightTable).map(col => (
                    <option key={col.name} value={col.name}>{col.name}</option>
                  ))}
                </select>
              </div>
              
              <button 
                onClick={addJoinCondition} 
                className="actionButton smallButton addButton"
              >
                +
              </button>
              <button 
                onClick={() => removeJoinCondition(index)} 
                className="actionButton smallButton removeButton"
              >
                ×
              </button>
            </div>
          ))}
        </div>
      )}

<div className="columnSelectionSection">
        <h3>Select Columns to Display</h3>
        {Object.keys(groupedColumns).length > 0 ? (
          <div className="columnsGridContainer">
            {Object.entries(groupedColumns).map(([tableName, tableColumns]) => (
              <div key={tableName} className="tableColumnGroup">
                <h4 className="tableGroupTitle">{tableName}</h4>
                <div className="columnsGrid">
                  {tableColumns.map(col => (
                    <div key={`${tableName}-${col.name}`} className="columnCheckbox">
                      <input
                        type="checkbox"
                        id={`${tableName}-${col.name}`}
                        checked={selectedColumns.some(c => c.table === tableName && c.name === col.name)}
                        onChange={() => toggleColumnSelection({
                          table: tableName,
                          name: col.name,
                          type: col.type
                        })}
                      />
                      <span class="checkmark"></span>
                      <label htmlFor={`${tableName}-${col.name}`}>
                        {col.name}
                      </label>
                    </div>
                  ))}
                </div>
              </div>
            ))}
          </div>
        ) : (
          <div className="noColumnsMessage">No columns available - select tables first</div>
        )}
      </div>

      <div className="buttonGroup">
        <button 
          onClick={handleSubmit} 
          className="actionButton primaryButton"
          disabled={isLoading}
        >
          {isLoading ? 'Executing...' : 'Execute Query'}
        </button>
        <button onClick={onCancel} className="actionButton">
          Cancel
        </button>
      </div>

      <div className="tableContainer">
        <h2 className="tableTitle">Query Results ({results.length} records)</h2>
        
        {isLoading ? (
          <div className="loadingIndicator">Loading results...</div>
        ) : results.length > 0 ? (
            <table className="dataTable">
              <thead>
                <tr>
                  {selectedColumns.length > 0
                    ? selectedColumns.map(col => (
                        <th key={`${col.table}-${col.name}`} className="tableHeader">
                          {col.table}.{col.name}
                        </th>
                      ))
                    : resultColumns.map(col => (
                        <th key={col} className="tableHeader">{col}</th>
                      ))
                  }
                </tr>
              </thead>
              <tbody>
                {results.map((row, rowIndex) => (
                  <tr key={rowIndex}>
                    {selectedColumns.length > 0
                      ? selectedColumns.map(col => (
                          <td key={`${rowIndex}-${col.table}-${col.name}`} className="tableCell">
                            {typeof row[col.name] === 'object' 
                              ? JSON.stringify(row[col.name]) 
                              : String(row[col.name] ?? 'NULL')}
                          </td>
                        ))
                      : resultColumns.map(col => (
                          <td key={`${rowIndex}-${col}`} className="tableCell">
                            {typeof row[col] === 'object' 
                              ? JSON.stringify(row[col]) 
                              : String(row[col] ?? 'NULL')}
                          </td>
                        ))
                    }
                  </tr>
                ))}
              </tbody>
            </table>
        ) : (
          <div className="noResults">No results to display</div>
        )}
      </div>
    </div>
  );
};

export default QueryTable;