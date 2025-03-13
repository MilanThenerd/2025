import React, { useState } from 'react';
import '../index.css';

const DATA_TYPES = ['string', 'number', 'boolean', 'date'];

const CreateTable = ({ onCancel, onCreate, onImport }) => {
  const [tableName, setTableName] = useState('');
  const [columns, setColumns] = useState([{ name: '', type: 'string' }]);
  const [error, setError] = useState('');

  const handleAddColumn = () => {
    setColumns([...columns, { name: '', type: 'string' }]);
  };

  const handleCreateTable = () => {
    setError('');

    if (!tableName.trim()) {
      setError("Table name cannot be empty.");
      return;
    }

    if (columns.length < 1) {
      setError("At least one column is required.");
      return;
    }

    for (const column of columns) {
      if (!column.name.trim()) {
        setError("Column name cannot be empty.");
        return;
      }
    }

    onCreate(tableName, columns);
  };

  const handleFileUpload = (e) => {
    const file = e.target.files[0];
    if (file) {
      onImport(file);
    }
  };

  return (
    <div className="tableContainer">
      <h2 className="tableTitle">Create Table</h2>
      {error && <div className="errorMessage">{error}</div>}
      <input
        type="text"
        placeholder="Table Name"
        value={tableName}
        onChange={(e) => setTableName(e.target.value)}
        className="inputField"
      />
      {columns.map((col, index) => (
        <div key={index} className="columnInputGroup">
          <input
            type="text"
            placeholder="Column Name"
            value={col.name}
            onChange={(e) => {
              const newColumns = [...columns];
              newColumns[index].name = e.target.value;
              setColumns(newColumns);
            }}
            className="inputField"
          />
          <select
            value={col.type}
            onChange={(e) => {
              const newColumns = [...columns];
              newColumns[index].type = e.target.value;
              setColumns(newColumns);
            }}
            className="inputField"
          >
            {DATA_TYPES.map((type) => (
              <option key={type} value={type}>
                {type}
              </option>
            ))}
          </select>
        </div>
      ))}
      <div className="buttonGroup">
        <button onClick={handleAddColumn} className="addColumnButton">
          Add Column
        </button>
        <button onClick={handleCreateTable} className="createButton">
          Create Table
        </button>
        <input
          type="file"
          accept=".json"
          onChange={handleFileUpload}
          className="fileInput"
        />
        <button onClick={onCancel} className="cancelButton">
          Cancel
        </button>
      </div>
    </div>
  );
};

export default CreateTable;