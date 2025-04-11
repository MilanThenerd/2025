import React, { useState } from 'react';
import '../index.css';

const DATA_TYPES = ['string', 'number', 'boolean', 'date'];

const CreateTable = ({ onCancel, onCreate, onImport }) => {
  const [tableName, setTableName] = useState('');
  const [columns, setColumns] = useState([{ name: '', type: 'string' }]);
  const [error, setError] = useState('');
  const [isCreating, setIsCreating] = useState(false);
  const [isImporting, setIsImporting] = useState(false);

  const handleAddColumn = () => {
    setColumns([...columns, { name: '', type: 'string' }]);
  };

  const handleCreateTable = async () => {
    try {
      setIsCreating(true);
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

      await onCreate(tableName, columns);
    } catch (err) {
      setError(err.message || 'Failed to create table');
    } finally {
      setIsCreating(false);
    }
  };

  const handleFileUpload = async (e) => {
    const file = e.target.files[0];
    if (file) {
      try {
        setIsImporting(true);
        await onImport(file);
      } catch (err) {
        setError(err.message || 'Failed to import table');
      } finally {
        setIsImporting(false);
        e.target.value = '';
      }
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
        disabled={isCreating || isImporting}
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
            disabled={isCreating || isImporting}
          />
          <select
            value={col.type}
            onChange={(e) => {
              const newColumns = [...columns];
              newColumns[index].type = e.target.value;
              setColumns(newColumns);
            }}
            className="inputField"
            disabled={isCreating || isImporting}
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
        <button 
          onClick={handleAddColumn} 
          className="addColumnButton"
          disabled={isCreating || isImporting}
        >
          Add Column
        </button>
        <button 
          onClick={handleCreateTable} 
          className="createButton"
          disabled={isCreating || isImporting}
        >
          {isCreating ? 'Creating...' : 'Create Table'}
        </button>
        <label className="fileInputLabel" style={{ opacity: isCreating || isImporting ? 0.5 : 1 }}>
          <input
            type="file"
            accept=".json"
            onChange={handleFileUpload}
            className="fileInput"
            disabled={isCreating || isImporting}
          />
        </label>
        <button 
          onClick={onCancel} 
          className="cancelButton"
          disabled={isCreating || isImporting}
        >
          Cancel
        </button>
      </div>
      {(isCreating || isImporting) && <div className="loadingText">Processing...</div>}
    </div>
  );
};

export default CreateTable;