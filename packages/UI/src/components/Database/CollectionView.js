import React, { useState, useEffect } from 'react';

const CollectionView = ({
  selectedDatabase,
  selectedCollection,
  collectionData,
  handleUpdateRow,
  handleDeleteRow,
  handleInsertRow,
  handleDeleteCollection,
  handleExportCollection,
}) => {
  const [currentPage, setCurrentPage] = useState(1);
  const [rowsPerPage] = useState(12);
  
  const indexOfLastCard = currentPage * rowsPerPage;
  const indexOfFirstCard = indexOfLastCard - rowsPerPage;
  const currentCards = collectionData.slice(indexOfFirstCard, indexOfLastCard);
  const totalPages = Math.ceil(collectionData.length / rowsPerPage);

  const paginate = (pageNumber) => setCurrentPage(pageNumber);
  
  useEffect(() => {
    const cards = document.querySelectorAll('.document-card');
    if (cards.length > 0) {
      cards.forEach(card => {
        card.style.animation = 'none';
        void card.offsetHeight;
        const index = Array.from(cards).indexOf(card);
        card.style.animation = '';
        card.style.animationDelay = `${index * 0.1}s`;
      });
    }
  }, [collectionData,currentPage]);

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
    <div className="collection-view">
      <div className="collection-header">
        <h2 className="tableTitle">
          <span className="database-name">{selectedDatabase}</span>
          <span className="divider">/</span>
          <span className="collection-name">{selectedCollection}</span>
        </h2>
        
        <div className="card-actions">
          <button 
            className="toggleButton"
            onClick={handleInsertRow}
            title="Add new document"
          >
            <span className="icon">+</span> Add Document
          </button>
          <button 
            className="toggleButton"
            onClick={() => {
              if (window.confirm(`Are you sure you want to delete the collection "${selectedCollection}"?`)) {
                handleDeleteCollection();
              }
            }}
            title="Delete this collection"
          >
            <span className="icon">🗑</span> Delete Collection
          </button>
          <button 
            className="toggleButton"
            onClick={handleExportCollection}
            title="Export collection data"
          >
            <span className="icon">⤓</span> Export
          </button>
        </div>
      </div>

      {collectionData.length === 0 ? (
        <div className="empty-state">
          <p>No documents found in this collection.</p>
          <button 
            className="toggleButton"
            onClick={handleInsertRow}
          >
            Add First Document
          </button>
        </div>
      ) : (
        <>
          <div className="document-controls">
            <div className="document-count">
              Showing {indexOfFirstCard + 1}-{Math.min(indexOfLastCard, collectionData.length)} of {collectionData.length} document{collectionData.length !== 1 ? 's' : ''}
            </div>
            
            {totalPages > 1 && (
              <div className="pagination-controls">
                <button
                  onClick={() => paginate(currentPage - 1)}
                  disabled={currentPage === 1}
                  className="toggleButton"
                >
                  Previous
                </button>
                
                <div className="page-numbers">
                  {Array.from({ length: totalPages }, (_, i) => i + 1).map(number => (
                    <button
                      key={number}
                      onClick={() => paginate(number)}
                      className={`toggleButton ${currentPage === number ? 'active' : ''}`}
                    >
                      {number}
                    </button>
                  ))}
                </div>
                
                <button
                  onClick={() => paginate(currentPage + 1)}
                  disabled={currentPage === totalPages}
                  className="toggleButton"
                >
                  Next
                </button>
              </div>
            )}
          </div>

          <div className="document-cards-container">
            {currentCards.map((document, index) => (
            <div 
                key={document._id || index} 
                className="document-card"
                style={{
                  opacity: 0,
                  animationDelay: `${(index * 0.1)}s`
                }}
              >
                <div className="card-header">
                  <h3 className="card-title">Document {indexOfFirstCard + index + 1}</h3>
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
        </>
      )}
    </div>
  );
};

export default CollectionView;