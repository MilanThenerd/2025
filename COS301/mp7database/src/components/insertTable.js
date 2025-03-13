import React, { useState } from 'react';
import '../index.css';

const InsertTable = ({ onCancel, onInsert, columns }) => {
  const [rowData, setRowData] = useState({});
  const [error, setError] = useState('');

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
      if (value !== 'true' && value !== 'false') {
        setError(`Invalid boolean for column: ${key}. Use "true" or "false".`);
        return;
      }
      value = value === 'true';
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

  const handleSubmit = () => {
    for (const column of columns) {
      const value = rowData[column.name];
      if (value === undefined || value === '') {
        setError(`All fields are required.`);
        return;
      }
      if (column.type === 'number' && typeof value !== 'number') {
        setError(`Invalid data type for column "${column.name}". Expected a number.`);
        return;
      }
      if (column.type === 'boolean' && typeof value !== 'boolean') {
        setError(`Invalid data type for column "${column.name}". Expected a boolean.`);
        return;
      }
      if (column.type === 'date' && isNaN(Date.parse(value))) {
        setError(`Invalid data type for column "${column.name}". Expected a valid date.`);
        return;
      }
      if (column.type === 'string' && typeof value !== 'string') {
        setError(`Invalid data type for column "${column.name}". Expected a string.`);
        return;
      }
    }
    onInsert(rowData);
    onCancel();
  };

  return (
    <div className="tableContainer">
      <h2 className="tableTitle">Insert Data</h2>
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
              />
            ) : (
              <input
                type="text"
                placeholder={`Enter ${column.name}`}
                value={rowData[column.name] || ''}
                onChange={(e) => handleInputChange(e, column.name, column.type)}
                className="inputField"
              />
            )}
          </div>
        ))}
      </div>
      <div className="buttonGroup">
        <button onClick={handleSubmit} className="actionButton">
          Insert
        </button>
        <button onClick={onCancel} className="actionButton">
          Cancel
        </button>
      </div>
    </div>
  );
};

export default InsertTable;