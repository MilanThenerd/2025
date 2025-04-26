import React, { useState, useEffect, useCallback } from 'react';

const QueryCollection = ({ onCancel, databases, fetchCollections, onQuery, handleUpdateRow,  handleDeleteRow, }) => {
  const [selectedDatabase, setSelectedDatabase] = useState('');
  const [selectedCollection, setSelectedCollection] = useState('');
  const [queryText, setQueryText] = useState('');
  const [collections, setCollections] = useState([]);
  const [searchResults, setSearchResults] = useState([]);
  const [isSearching, setIsSearching] = useState(false);
  const [searchScope, setSearchScope] = useState('all');
  const [selectedField, setSelectedField] = useState('');
  const [fieldValue, setFieldValue] = useState('');
  const [useFieldFilter, setUseFieldFilter] = useState(false);

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
      setSelectedField('');
    }
  }, [selectedDatabase, fetchCollections]);

  useEffect(() => {
    setSelectedField('');
    setFieldValue('');
  }, [selectedCollection]);

  const handleSearch = useCallback(async () => {
    if (!queryText.trim() && !(useFieldFilter && fieldValue.trim())) return;

    setIsSearching(true);

    try {
      let queryObj = {};

      if (useFieldFilter && selectedField && fieldValue) {
        const currentCollection = collections.find(c => c.name === selectedCollection);
        const fieldType = currentCollection?.columns.find(c => c.name === selectedField)?.type;

        let convertedValue;
        switch (fieldType) {
          case 'number':
            convertedValue = Number(fieldValue);
            break;
          case 'boolean':
            convertedValue = fieldValue.toLowerCase() === 'true';
            break;
          case 'date':
            convertedValue = new Date(fieldValue);
            break;
          default:
            convertedValue = fieldValue;
        }

        queryObj[selectedField] = convertedValue;
      } else {
        try {
          if (queryText.trim().startsWith('{') && queryText.trim().endsWith('}')) {
            queryObj = JSON.parse(queryText);
          } else {
            queryObj = { $text: { $search: queryText } };
          }
        } catch (e) {
          queryObj = { $text: { $search: queryText } };
        }
      }

      const query = {
        query: queryObj,
        scope: searchScope,
        ...(searchScope !== 'all' && selectedDatabase && { database: selectedDatabase }),
        ...(searchScope === 'collection' && selectedCollection && { collection: selectedCollection })
      };

      const results = await onQuery(query);
      setSearchResults(results);
    } catch (error) {
      console.error('Search error:', error);
      setSearchResults([]);
    } finally {
      setIsSearching(false);
    }
  }, [queryText, searchScope, selectedDatabase, selectedCollection, selectedField, fieldValue, useFieldFilter, collections, onQuery]);

  const handleKeyDown = (e) => {
    if (e.key === 'Enter') {
      handleSearch();
    }
  };

  const getCurrentCollectionFields = () => {
    if (!selectedCollection) return [];
    const collection = collections.find(c => c.name === selectedCollection);
    return collection?.columns || [];
  };

  const formatValue = (value, indent = 0, isTopLevel = false) => {
    if (value === null || value === undefined) return 'null';
    if (typeof value === 'boolean') return value.toString();
    if (typeof value === 'string' && !isNaN(Date.parse(value))) {
      try {
        const date = new Date(value);
        return date.toLocaleString();
      } catch {
        return value;
      }
    }
    if (Array.isArray(value)) {
      if (value.length === 0) return isTopLevel ? '' : '[]';
      
      const spaces = ' '.repeat(indent + 2);
      const items = value.map(item => 
        `${spaces}${formatValue(item, indent + 2)}`
      ).join(',\n');
      
      return isTopLevel ? `${items}` : `[\n${items}\n${' '.repeat(indent)}]`;
    }
    if (typeof value === 'object') {
      const entries = Object.entries(value);
      if (entries.length === 0) return isTopLevel ? '' : '{}';
      
      const spaces = ' '.repeat(indent + 2);
      const items = entries.map(([key, val]) => 
        `${spaces}"${key}": ${formatValue(val, indent + 2)}`
      ).join(',\n');
      
      return isTopLevel ? `${items}` : `{\n${items}\n${' '.repeat(indent)}}`;
    }
    return typeof value === 'string' ? `"${value}"` : value.toString();
  };

  return (
    <div className="tableContainer">
      <h2 className='tableTitle'>NoSQL Query</h2>
      <div className="query-controls">
        <div className="scope-selector">
          <label>
            <input
              type="radio"
              name="scope"
              checked={searchScope === 'all'}
              onChange={() => setSearchScope('all')}
            />
            Search All
          </label>
          <label>
            <input
              type="radio"
              name="scope"
              checked={searchScope === 'database'}
              onChange={() => setSearchScope('database')}
              disabled={!selectedDatabase}
            />
            Current Database
          </label>
          <label>
            <input
              type="radio"
              name="scope"
              checked={searchScope === 'collection'}
              onChange={() => setSearchScope('collection')}
              disabled={!selectedCollection}
            />
            Current Collection
          </label>
        </div>

        <div className="database-selector">
          <select className="inputField"
            value={selectedDatabase}
            onChange={(e) => setSelectedDatabase(e.target.value)}
          >
            <option value="">Select Database</option>
            {databases.map(db => (
              <option key={db} value={db}>{db}</option>
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

        <div className="query-input">
          <input
            type="text"
            placeholder="Enter query (e.g., { field: value } or just text to search)"
            value={queryText}
            onChange={(e) => setQueryText(e.target.value)}
            onKeyDown={handleKeyDown}
            className="inputField"
          />
          <button className="toggleButton"
            onClick={handleSearch} disabled={isSearching || (!queryText.trim() && !(useFieldFilter && fieldValue.trim()))}>
            {isSearching ? 'Searching...' : 'Search'}
          </button>
        </div>
      </div>

      <button onClick={onCancel} className="toggleButton">Cancel</button>

      <div className="search-results">
        {searchResults.length > 0 ? (
          <div className="document-cards-container">
            <div className="document-count">
              Showing {searchResults.length} document{searchResults.length !== 1 ? 's' : ''}
            </div>
            {searchResults.map((document, index) => (
              <div key={document._id || index} className="document-card">
                <div className="card-header">
                  <h3 className="card-title">Document {index + 1}</h3>
                  <div className="card-actions">
                    <button
                      className="toggleButton"
                      onClick={() => handleUpdateRow(document)}
                      title="Edit document"
                    >
                      ✏️ Edit
                    </button>
                    <button
                      className="toggleButton"
                      onClick={() => {
                        if (window.confirm('Are you sure you want to delete this document?')) {
                          handleDeleteRow(document);
                        }
                      }}
                      title="Delete document"
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
                        {typeof value === 'object' || Array.isArray(value) 
                          ? formatValue(value, 2, true)
                          : formatValue(value)}
                      </pre>
                    </div>
                  ))}
                </div>
              </div>
            ))}
          </div>
      ) : (
      <div className="no-results">
        {isSearching ? 'Searching...' : 'No documents found matching your query'}
      </div>
        )}
    </div>
    </div >
  );
};

export default QueryCollection;