import React, { useState } from 'react';

const CreateCollection = ({ onCancel, onCreate, onImport , selectedDatabase }) => {
  const [collectionName, setCollectionName] = useState('');
  const [error, setError] = useState('');
  const [isCreating, setIsCreating] = useState(false);
  const [isImporting, setIsImporting] = useState(false);

  const handleCreateCollection = async () => {
    try {
      setIsCreating(true);
      setError('');

      if (!collectionName.trim()) {
        setError("Collection name cannot be empty.");
        return;
      }

      await onCreate(collectionName);
    } catch (err) {
      setError(err.message || 'Failed to create collection');
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
        setError(err.message || 'Failed to import collection');
      } finally {
        setIsImporting(false);
        e.target.value = '';
      }
    }
  };

  return (
    <div className="tableContainer">
      <h2 className="tableTitle">Create Collection in {selectedDatabase}</h2>
      {error && <div className="errorMessage">{error}</div>}
      <input
        type="text"
        placeholder="Collection Name"
        value={collectionName}
        onChange={(e) => setCollectionName(e.target.value)}
        className="inputField"
        disabled={isCreating || isImporting}
      />
      <div className="buttonGroup">
        <button 
          onClick={handleCreateCollection} 
          className="createButton"
          disabled={isCreating || isImporting}
        >
          {isCreating ? 'Creating...' : 'Create Collection'}
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

export default CreateCollection;