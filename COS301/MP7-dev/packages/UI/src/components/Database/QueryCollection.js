import React, { useState, useEffect, useCallback } from 'react';

const QueryCollection = ({ onCancel, databases, fetchCollections, onQuery, handleUpdateRow, handleDeleteRow }) => {
  const [selectedDatabase, setSelectedDatabase] = useState('');
  const [selectedCollection, setSelectedCollection] = useState('');
  const [collections, setCollections] = useState([]);
  const [searchResults, setSearchResults] = useState([]);
  const [isSearching, setIsSearching] = useState(false);
  // Field filter state
  const [filters, setFilters] = useState([{
    field: '',
    operator: 'eq',
    value: ''
  }]);

  useEffect(() => {
    if (selectedDatabase) {
      const loadCollections = async () => {
        const fetchedCollections = await fetchCollections(selectedDatabase);
        setCollections(fetchedCollections);
      };
      loadCollections();
    } else {
      setCollections([]);
      setSelectedCollection('');
    }
  }, [selectedDatabase, fetchCollections]);

  const handleSearch = useCallback(async () => {
    if (!selectedDatabase || !selectedCollection) return;
    if (filters.some(f => !f.field || f.value === '')) return;

    setIsSearching(true);
    setSearchResults([]);

    try {
      const allDocuments = [];
      
      for (const filter of filters) {
        if (!filter.field || filter.value === '') continue;
        
        const result = await onQuery(
          selectedDatabase,
          selectedCollection,
          filter.field,
          filter.value,
          filter.operator
        );

        const allDocuments = [];
        result.data.forEach(documentResult => {
          documentResult.documents.forEach(document =>{
            allDocuments.push(document);
          });

        });

        setSearchResults(allDocuments);
      }
    } catch (error) {
      console.error('Search error:', error);
      setSearchResults([]);
    } finally {
      setIsSearching(false);
    }
  }, [filters, selectedDatabase, selectedCollection, onQuery]);

  const addFilter = () => {
    setFilters([...filters, {
      field: '',
      operator: 'eq',
      value: ''
    }]);
  };

  const removeFilter = (index) => {
    if (filters.length <= 1) return;
    setFilters(filters.filter((_, i) => i !== index));
  };

  const updateFilter = (index, key, value) => {
    const newFilters = [...filters];
    newFilters[index] = { ...newFilters[index], [key]: value };
    setFilters(newFilters);
  };

  const formatDocumentValue = (value) => {
    if (value === null || value === undefined) return 'null';
    if (typeof value === 'object') return JSON.stringify(value, null, 2);
    return value.toString();
  };

  return (
    <div className="tableContainer">
      <h2 className='tableTitle'>NoSQL Query</h2>
      <div className="query-controls">
        <div className="database-selector">
          <select
            className="inputField"
            value={selectedDatabase}
            onChange={(e) => setSelectedDatabase(e.target.value)}
          >
            <option value="">Select Database</option>
            {databases.map(db => (
              <option key={db.database} value={db.database}>{db.database}</option>
            ))}
          </select>
        </div>

        {selectedDatabase && (
          <div className="collection-selector">
            <select
              className="inputField"
              value={selectedCollection}
              onChange={(e) => setSelectedCollection(e.target.value)}
              disabled={!selectedDatabase || collections.length === 0}
            >
              <option value="">Select Collection</option>
              {collections.map(col => (
                <option key={col.name} value={col.name}>{col.name}</option>
              ))}
            </select>
          </div>
        )}
        <div className="filter-builder">
          {filters.map((filter, index) => (
            <div key={index} className="filter-row">
              <input
                type="text"
                className="inputField"
                value={filter.field}
                onChange={(e) => updateFilter(index, 'field', e.target.value)}
                placeholder="Field name"
              />

              <select
                className="inputField"
                value={filter.operator}
                onChange={(e) => updateFilter(index, 'operator', e.target.value)}
              >
                <option value="eq">=</option>
                <option value="ne">!=</option>
                <option value="g">&gt;</option>
                <option value="l">&lt;</option>
                <option value="ge">&gt;=</option>
                <option value="le">&lt;=</option>
              </select>

              <input
                type="text"
                className="inputField"
                value={filter.value}
                onChange={(e) => updateFilter(index, 'value', e.target.value)}
                placeholder="Value"
              />

              {filters.length > 1 && (
                <button
                  className="toggleButton"
                  onClick={() => removeFilter(index)}
                >
                  Remove
                </button>
              )}
            </div>
          ))}

          <button className="toggleButton" onClick={addFilter}>
            Add Filter
          </button>
        </div>

        <button
          className="toggleButton"
          onClick={handleSearch}
          disabled={isSearching || !selectedDatabase || !selectedCollection || filters.some(f => !f.field || f.value === '')}
        >
          {isSearching ? 'Searching...' : 'Search'}
        </button>
      </div>

      <button onClick={onCancel} className="toggleButton">Cancel</button>

      <div className="search-results">
        {isSearching ? (
          <div className="loading-message">
            <div className="loading-spinner"></div>
            Searching documents...
          </div>
        ) : searchResults.length > 0 ? (
          <div className="document-cards-container">
            <div className="document-count">
              Found {searchResults.length} document{searchResults.length !== 1 ? 's' : ''}
            </div>
            {searchResults.map((document, index) => (
              <div key={document._id || index} className="document-card">
                <div className="card-header">
                  <h3 className="card-title">Document {index + 1}/{document.database || selectedDatabase}/{document.collection || selectedCollection}</h3>
                  <div className="card-actions">
                    <button
                      className="toggleButton"
                      onClick={() => handleUpdateRow({
                        database: document._database || selectedDatabase,
                        collection: document._collection || selectedCollection,
                        document
                      })}
                    >
                      ✏️ Edit
                    </button>
                    <button
                      className="toggleButton"
                      onClick={() => {
                        if (window.confirm('Are you sure you want to delete this document?')) {
                          handleDeleteRow({
                            database: document._database || selectedDatabase,
                            collection: document._collection || selectedCollection,
                            documentId: document._id
                          });
                        }
                      }}
                    >
                      🗑 Delete
                    </button>
                  </div>
                </div>
                <div className="card-content">
                  {Object.entries(document).map(([key, value]) => (
                    <div key={key} className="document-field">
                      <span className="field-name">{key}:</span>
                      <pre className="field-value">
                        {formatDocumentValue(value)}
                      </pre>
                    </div>
                  ))}
                </div>
              </div>
            ))}
          </div>
        ) : (
          <div className="no-results">
            {selectedDatabase && selectedCollection 
              ? 'No documents found matching your query' 
              : 'Select a database and collection to search'}
          </div>
        )}
      </div>
    </div>
  );
};

export default QueryCollection;