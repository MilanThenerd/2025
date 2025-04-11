import React, { useState } from 'react';
import '../index.css';

const InsertTable = ({ onCancel, onInsert, columns, selectedTable }) => {
  const [rowData, setRowData] = useState({});
  const [error, setError] = useState('');
  const [isSubmitting, setIsSubmitting] = useState(false);

  const handleInputChange = (e, key, type) => {
    let value = e.target.value;

    if (type === 'number') {
      if (value === '') {
        setError(`Field cannot be empty for column: ${key}`);
        return;
      }
      value = parseFloat(value);
      if (isNaN(value)) {
        setError(`Invalid number for column: ${key}`);
        return;
      }
    } else if (type === 'boolean') {
      value = value.toLowerCase();
      if (value === 'true') {
        value = true;
      } else if (value === 'false') {
        value = false;
      } else {
        setError(`Invalid boolean value for column: ${key}`);
        return;
      }
    } else if (type === 'date') {
      if (value === '') {
        setError(`Field cannot be empty for column: ${key}`);
        return;
      }
      const date = new Date(value);
      if (isNaN(date.getTime())) {
        setError(`Invalid date for column: ${key}. Use the format yyyy-mm-dd.`);
        return;
      }
      value = date.toISOString().split('T')[0];
    }

    setRowData({ ...rowData, [key]: value });
    setError('');
  };

  const handleSubmit = async () => {
    try {
      setIsSubmitting(true);
      for (const column of columns) {
        const value = rowData[column.name];
        if (value === undefined || value === '') {
          setError(`All fields are required.`);
          setIsSubmitting(false);
          return;
        }
        if (column.type === 'number' && typeof value !== 'number') {
          setError(`Invalid data type for column "${column.name}". Expected a number.`);
          setIsSubmitting(false);
          return;
        }
        if (column.type === 'boolean' && typeof value !== 'boolean') {
          setError(`Invalid data type for column "${column.name}". Expected a boolean.`);
          setIsSubmitting(false);
          return;
        }
        if (column.type === 'date' && isNaN(Date.parse(value))) {
          setError(`Invalid data type for column "${column.name}". Expected a valid date.`);
          setIsSubmitting(false);
          return;
        }
        if (column.type === 'string' && typeof value !== 'string') {
          setError(`Invalid data type for column "${column.name}". Expected a string.`);
          setIsSubmitting(false);
          return;
        }
      }

      await onInsert(rowData);
      onCancel();
    } catch (err) {
      setError(err.message || 'Failed to insert data');
    } finally {
      setIsSubmitting(false);
    }
  };

  return (
    <div className="tableContainer">
      <h2 className="tableTitle">Insert Data into {selectedTable}</h2>
      {error && <div className="errorMessage">{error}</div>}
      <div className="inputGroup">
        {columns.map((column) => (
          <div key={column.name} className="inputFieldContainer">
            <label>{column.name} ({column.type})</label>
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
      <div className="buttonGroup">
        <button 
          onClick={handleSubmit} 
          className="actionButton"
          disabled={isSubmitting}
        >
          {isSubmitting ? 'Inserting...' : 'Insert'}
        </button>
        <button 
          onClick={onCancel} 
          className="actionButton"
          disabled={isSubmitting}
        >
          Cancel
        </button>
      </div>
    </div>
  );
};

export default InsertTable;