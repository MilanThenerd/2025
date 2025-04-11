import React, { useState } from 'react';
import '../index.css';

const QueryTable = ({ onCancel, tables, columns, onQuery }) => {
  const [queries, setQueries] = useState([
    { table: '', column: '', operator: '', value: '' }
  ]);
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

    setError('');
    setIsLoading(true);
    
    try {
      let queryString = 'SELECT * FROM ';
      
      const firstTable = queries[0].table;
      queryString += firstTable;
      
      const whereClauses = queries
        .filter(q => q.table === firstTable)
        .map(q => {
          const columnType = getColumnType(q.table, q.column);
          let value = q.value;
          
          if (columnType === 'string') {
            value = `'${value}'`;
          } else if (columnType === 'date') {
            value = `DATE '${value}'`;
          } else if (columnType === 'boolean') {
            value = value === 'true' ? 'TRUE' : 'FALSE';
          }
          
          return `${q.column} ${q.operator} ${value}`;
        });
      
      if (whereClauses.length > 0) {
        queryString += ' WHERE ' + whereClauses.join(' AND ');
      }
      
      if (searchTerm.trim()) {
        const searchConditions = Object.keys(columns[firstTable] || {})
          .filter(colName => getColumnType(firstTable, colName) === 'string')
          .map(colName => `${colName} LIKE '%${searchTerm}%'`);
          
        if (searchConditions.length > 0) {
          queryString += whereClauses.length > 0 ? ' AND ' : ' WHERE ';
          queryString += '(' + searchConditions.join(' OR ') + ')';
        }
      }

      const queryResults = await onQuery(queryString);
      
      setResults(queryResults || []);
    } catch (err) {
      setError('Error executing query: ' + err.message);
      setResults([]);
    } finally {
      setIsLoading(false);
    }
  };

  const resultColumns = results.length > 0 
    ? Array.from(new Set(results.flatMap(Object.keys)))
    : [];

  return (
    <div className="tableContainer">
      <h2 className="tableTitle">Query Builder</h2>
      
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

            <div className="queryActions">
              <button 
                onClick={() => removeQuery(index)} 
                className="actionButton smallButton removeButton"
              >
                ×
              </button>
              {index === queries.length - 1 && (
                <button 
                  onClick={addQuery} 
                  className="actionButton smallButton addButton"
                >
                  +
                </button>
              )}
            </div>
          </div>
        ))}
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

      <div className="resultsContainer">
        <h3>Query Results ({results.length} records)</h3>
        
        {isLoading ? (
          <div className="loadingIndicator">Loading results...</div>
        ) : results.length > 0 ? (
          <div className="resultsTableWrapper">
            <table className="resultsTable">
              <thead>
                <tr>
                  {resultColumns.map(col => (
                    <th key={col}>{col}</th>
                  ))}
                </tr>
              </thead>
              <tbody>
                {results.map((row, rowIndex) => (
                  <tr key={rowIndex}>
                    {resultColumns.map(col => (
                      <td key={`${rowIndex}-${col}`}>
                        {typeof row[col] === 'object' 
                          ? JSON.stringify(row[col]) 
                          : String(row[col] ?? 'NULL')}
                      </td>
                    ))}
                  </tr>
                ))}
              </tbody>
            </table>
          </div>
        ) : (
          <div className="noResults">No results to display</div>
        )}
      </div>
    </div>
  );
};

export default QueryTable;