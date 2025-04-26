import React, { useState } from 'react';
import '../../index.css';

const InsertCollection = ({ onCancel, onInsert, selectedCollection }) => {
  const [rowData, setRowData] = useState({});
  const [customColumns, setCustomColumns] = useState([]);
  const [newColumnName, setNewColumnName] = useState('');
  const [newColumnType, setNewColumnType] = useState('string');
  const [error, setError] = useState('');
  const [isSubmitting, setIsSubmitting] = useState(false);

  const handleInputChange = (e, key, type) => {
    let value = e.target.value;
    setError('');

    if (type === 'number') {
      value = value === '' ? null : parseFloat(value);
      if (value !== null && isNaN(value)) {
        setError(`Invalid number for column: ${key}`);
        return;
      }
    } else if (type === 'boolean') {
      value = value.toLowerCase();
      if (value === 'true') value = true;
      else if (value === 'false') value = false;
      else value = null;
    } else if (type === 'date') {
      if (value) {
        const date = new Date(value);
        if (isNaN(date.getTime())) {
          setError(`Invalid date for column: ${key}. Use the format yyyy-mm-dd.`);
          return;
        }
        value = date.toISOString().split('T')[0];
      } else {
        value = null;
      }
    }

    setRowData({ ...rowData, [key]: value });
  };

  const addCustomColumn = () => {
    if (!newColumnName.trim()) {
      setError('Column name cannot be empty');
      return;
    }
    
      const newColumn = {
      name: newColumnName,
      type: newColumnType
    };

    setCustomColumns([...customColumns, newColumn]);
    setRowData({ ...rowData, [newColumnName]: '' });
    setNewColumnName('');
    setError('');
  };

  const removeCustomColumn = (columnName) => {
    setCustomColumns(customColumns.filter(col => col.name !== columnName));
    const newRowData = { ...rowData };
    delete newRowData[columnName];
    setRowData(newRowData);
  };

  const handleSubmit = async () => {
    try {
      setIsSubmitting(true);
      
      // Validate required fields
      const allColumns = [...customColumns];
      const finalData = { ...rowData };


      await onInsert(finalData);
      onCancel();
    } catch (err) {
      setError(err.message || 'Failed to insert data');
    } finally {
      setIsSubmitting(false);
    }
  };

  return (
    <div className="tableContainer">
      <h2 className="tableTitle">Insert Data into {selectedCollection}</h2>
      {error && <div className="errorMessage">{error}</div>}
      
      {/* Custom Columns Section */}
      <div className="sectionDivider">
        <h3>Fields</h3>
        <div className="addColumnForm">
          <input
            type="text"
            placeholder="New field name"
            value={newColumnName}
            onChange={(e) => setNewColumnName(e.target.value)}
            className="inputField"
            disabled={isSubmitting}
          />
          <select
            value={newColumnType}
            onChange={(e) => setNewColumnType(e.target.value)}
            className="inputField"
            disabled={isSubmitting}
          >
            <option value="string">String</option>
            <option value="number">Number</option>
            <option value="boolean">Boolean</option>
            <option value="date">Date</option>
          </select>
          <button 
            onClick={addCustomColumn}
            className="actionButton small"
            disabled={isSubmitting}
          >
            Add Field
          </button>
        </div>

        {/* Custom Columns Inputs */}
        <div className="inputGroup">
          {customColumns.map((column) => (
            <div key={column.name} className="inputFieldContainer">
              <div className="columnHeader">
                <label>{column.name} ({column.type})</label>
                <button 
                  onClick={() => removeCustomColumn(column.name)}
                  className="toggleButton removeField"
                  title="Remove field"
                >
                  ×
                </button>
              </div>
              {column.type === 'date' ? (
                <input
                  type="date"
                  value={rowData[column.name] || ''}
                  onChange={(e) => handleInputChange(e, column.name, column.type)}
                  className="inputField"
                  disabled={isSubmitting}
                />
              ) : column.type === 'boolean' ? (
                <select
                  value={rowData[column.name] ?? ''}
                  onChange={(e) => handleInputChange(e, column.name, column.type)}
                  className="inputField"
                  disabled={isSubmitting}
                >
                  <option value="">Select a value</option>
                  <option value="true">True</option>
                  <option value="false">False</option>
                </select>
              ) : (
                <input
                  type={column.type === 'number' ? 'number' : 'text'}
                  placeholder={`Enter ${column.name}`}
                  value={rowData[column.name] || ''}
                  onChange={(e) => handleInputChange(e, column.name, column.type)}
                  className="inputField"
                  disabled={isSubmitting}
                />
              )}
            </div>
          ))}
        </div>
      </div>

      <div className="buttonGroup">
        <button 
          onClick={handleSubmit} 
          className="actionButton primary"
          disabled={isSubmitting}
        >
          {isSubmitting ? 'Inserting...' : 'Insert Document'}
        </button>
        <button 
          onClick={onCancel} 
          className="actionButton secondary"
          disabled={isSubmitting}
        >
          Cancel
        </button>
      </div>
    </div>
  );
};

export default InsertCollection;