import React, { useState } from 'react';
import './CreateDatabase.css';

const CreateDatabase = ({ onCreate, onCancel }) => {
  const [databaseName, setDatabaseName] = useState('');
  const [formError, setFormError] = useState(null);

  const handleSubmit = (e) => {
    e.preventDefault();
    console.log('Submitting database:', databaseName);
    if (databaseName.trim()) {
      onCreate(databaseName);
      setDatabaseName('');
      setFormError(null);
    } else {
      setFormError('Database name cannot be empty');
    }
  };

  return (
    <div className="createDatabaseContainer">
      <h2>Create New Database</h2>
      {formError && <div className="formError">{formError}</div>}
      <form onSubmit={handleSubmit} className="createDatabaseForm">
        <label htmlFor="databaseName">Database Name:</label>
        <input
          type="text"
          id="databaseName"
          value={databaseName}
          onChange={(e) => setDatabaseName(e.target.value)}
          placeholder="Enter database name"
          required
          aria-required="true"
        />
        <div className="formActions">
          <button type="submit" className="createButton">
            Create
          </button>
          <button
            type="button"
            className="cancelButton"
            onClick={onCancel}
            aria-label="Cancel database creation"
          >
            Cancel
          </button>
        </div>
      </form>
    </div>
  );
};

export default CreateDatabase;