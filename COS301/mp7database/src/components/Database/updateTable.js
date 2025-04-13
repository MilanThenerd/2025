import React, { useState } from 'react';
import '../../index.css';

const UpdateTable = ({ row, columns, onSave, onCancel, selectedTable }) => {
  const [rowData, setRowData] = useState(row);
  const [error, setError] = useState('');
  const [isSaving, setIsSaving] = useState(false);

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

  const handleSave = async () => {
    try {
      setIsSaving(true);
      
      for (const column of columns) {
        const value = rowData[column.name];
        if (value === undefined || value === '') {
          setError(`All fields are required.`);
          setIsSaving(false);
          return;
        }
        if (column.type === 'number' && typeof value !== 'number') {
          setError(`Invalid data type for column "${column.name}". Expected a number.`);
          setIsSaving(false);
          return;
        }
        if (column.type === 'boolean' && typeof value !== 'boolean') {
          setError(`Invalid data type for column "${column.name}". Expected a boolean.`);
          setIsSaving(false);
          return;
        }
        if (column.type === 'date' && isNaN(Date.parse(value))) {
          setError(`Invalid data type for column "${column.name}". Expected a valid date.`);
          setIsSaving(false);
          return;
        }
        if (column.type === 'string' && typeof value !== 'string') {
          setError(`Invalid data type for column "${column.name}". Expected a string.`);
          setIsSaving(false);
          return;
        }
      }

      await onSave(rowData);
    } catch (err) {
      setError(err.message || 'Failed to update data');
    } finally {
      setIsSaving(false);
    }
  };

  return (
    <div className="tableContainer">
      <h2 className="tableTitle">Update Data in {selectedTable}</h2>
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
                disabled={isSaving}
              />
            ) : column.type === 'boolean' ? (
              <select
                value={rowData[column.name] ?? ''}
                onChange={(e) => handleInputChange(e, column.name, column.type)}
                className="inputField"
                disabled={isSaving}
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
                disabled={isSaving}
              />
            )}
          </div>
        ))}
      </div>
      <div className="buttonGroup">
        <button 
          onClick={handleSave} 
          className="actionButton"
          disabled={isSaving}
        >
          {isSaving ? 'Saving...' : 'Save'}
        </button>
        <button 
          onClick={onCancel} 
          className="actionButton"
          disabled={isSaving}
        >
          Cancel
        </button>
      </div>
    </div>
  );
};

export default UpdateTable;