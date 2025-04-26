import React, { useState } from 'react';
import LoginRegister from './components/LoginRegister/LoginRegister';
import useCollectionManagement from './components/Server/uiBackend';
import CreateDatabase from './components/Database/CreateDatabase';
import CreateCollection from './components/Database/CreateCollection';
import InsertDocument from './components/Database/InsertDocument';
import UpdateDocument from './components/Database/UpdateDocument';
import CollectionView from './components/Database/CollectionView';
import QueryCollection from './components/Database/QueryCollection';
import HomePage from './components/HomePage/HomePage'
import './App.css';
import ProfileView from './components/Database/ProfileView';

const App = () => {
  const {
    // State
    isLoggedIn,
    username,
    rememberMe,
    databases,
    selectedDatabase,
    collections,
    selectedCollection,
    collectionData = {data: []},
    loading,
    error,
    showCreateCollection,
    showInsertCollection,
    showUpdateCollection,
    showQueryCollection,
    rowToUpdate,
    hoveredCollection,
    darkMode,
    fetchCollections,

    // Setters
    setSelectedDatabase,
    setSelectedCollection,
    setShowCreateCollection,
    setShowInsertCollection,
    setShowUpdateCollection,
    setShowQueryCollection,
    setHoveredCollection,
    setDarkMode,
    setError,
    setRememberMe,
    getUser,

    // Handlers
    handleLogin,
    handleRegister,
    handleLogout,
    handleUpdateUser,
    handleCreateDatabase,
    handleDeleteDatabase,
    handleCreateCollection,
    handleDeleteCollection,
    handleInsertRowData,
    handleSaveUpdatedRow,
    handleDeleteRow,
    handleExecuteQuery,
    handleImportCollection,
    handleExportCollection,
    handleExportDatabase,
    handleUpdateRow,
    fetchCollectionData,
  } = useCollectionManagement();

  const [showCreateDatabase, setShowCreateDatabase] = useState(false);
  const [showProfileView, setShowProfileView] = useState(false);
  const [showHomePage, setShowHomePage] = useState(true);

  const handleNavigateToTables = () => {
    console.log('Navigating to create database form');
    setShowHomePage(false);
    setShowCreateDatabase(true);
  };

  const handleNavigateToHome = () => {
    console.log('Navigating back to HomePage');
    setShowHomePage(true);
    setShowCreateDatabase(false);
    setShowCreateCollection(false);
    setShowInsertCollection(false);
    setShowQueryCollection(false);
    setSelectedDatabase(null);
  };

  const handleDatabaseCreated = (databaseName) => {
    console.log('Database created:', databaseName);
    handleCreateDatabase(databaseName);
    console.log('Updated databases:', databases);
    setShowCreateDatabase(false);
    setShowHomePage(true);
  };

  return !isLoggedIn ? (
    <LoginRegister
      onLogin={handleLogin}
      onRegister={handleRegister}
      rememberMe={rememberMe}
      setRememberMe={setRememberMe}
    />
  ) : (
    <div className={`appContainer ${darkMode ? 'darkMode' : ''}`}>
      {showHomePage ? (
        <HomePage
          username={username}
          databases={databases || []}
          setShowHomePage={setShowHomePage}
          setShowCreateDatabase={setShowCreateDatabase}
          setSelectedDatabase={setSelectedDatabase}
          onNavigateToTables={handleNavigateToTables}
          onLogout={handleLogout}
          handleDeleteDatabase={handleDeleteDatabase}
          setCollections={fetchCollections}
          setCollectionData={fetchCollectionData}
        />
      ) : (
        <>
          {loading && (
            <div className="loadingOverlay">
              <div className="loadingSpinner"></div>
              <p>Loading...</p>
            </div>
          )}

          {error && (
            <div className="errorBanner">
              {error}
              <button onClick={() => setError(null)} className="closeError">
                ×
              </button>
            </div>
          )}

          {!showCreateDatabase && (
            <div className="sidebar">
              <div className="user-info">
                <button onClick={() =>
                  {
                    setShowProfileView(true)
                    setShowQueryCollection(false)
                    setShowUpdateCollection(false)
                    setShowInsertCollection(false)
                    setShowCreateCollection(false)
                  }
                } 
                  className="usernameButton">👤</button>
                <h2 className="sidebarTitle">{username}</h2>
                <button onClick={handleLogout} className="logout-button">
                  Logout
                </button>
              </div>

              <p>Selected Database: {selectedDatabase || 'None'}</p>

              <button
                onClick={handleNavigateToHome}
                className="homeButton"
                aria-label="Go to Home Page"
              >
                🏠 Home
              </button>

              {selectedDatabase && (
                <div className="sidebarSection">
                  <div className="sidebarHeader">
                    <h2 className="sidebarTitle">Collections</h2>
                  </div>
                  <ul className="sidebarList">
                    {collections.map(collection => (
                      <li
                        key={collection.name}
                        onMouseEnter={() => setHoveredCollection(collection)}
                        onMouseLeave={() => setHoveredCollection(null)}
                        onClick={() => {
                          setSelectedCollection(collection.name);
                          setShowCreateCollection(false);
                          setShowInsertCollection(false);
                          setShowQueryCollection(false);
                          setShowCreateDatabase(false);
                          setShowProfileView(false);
                        }}
                        className={`sidebarItem ${hoveredCollection === collection.name ? 'sidebarItemHover' : ''} ${selectedCollection === collection.name ? 'sidebarItemSelected' : ''}`}
                      >
                        {collection.name}
                      </li>
                    ))}
                  </ul>
                  <button
                    onClick={() => setShowCreateCollection(true)}
                    className="actionButton"
                    disabled={!selectedDatabase}
                  >
                    +
                  </button>
                </div>
              )}
              <div className="actionButtons">
                <button
                  onClick={() => setShowQueryCollection(true)}
                  className="actionButton"
                  disabled={!selectedDatabase}
                >
                  Query
                </button>
                <button
                  onClick={() => setDarkMode(!darkMode)}
                  className="actionButton"
                >
                  {darkMode ? '☀️ Light' : '🌙 Dark'}
                </button>
              </div>
            </div>
          )}

          <div className="mainContent">
            {showCreateDatabase ? (
              <CreateDatabase
                onCreate={handleDatabaseCreated}
                onCancel={() => {
                  setShowCreateDatabase(false);
                  setShowHomePage(true);
                }}
              />
            ) : showCreateCollection ? (
              <CreateCollection
                onCancel={() => setShowCreateCollection(false)}
                onCreate={handleCreateCollection}
                onImport={handleImportCollection}
                selectedDatabase={selectedDatabase}
              />
            ) : showInsertCollection ? (
              <InsertDocument
                onCancel={() => setShowInsertCollection(false)}
                onInsert={handleInsertRowData}
                selectedCollection={selectedCollection}
              />
            ) : showUpdateCollection ? (
              <UpdateDocument
                row={rowToUpdate}
                onSave={handleSaveUpdatedRow}
                onCancel={() => setShowUpdateCollection(false)}
                selectedCollection={selectedCollection}
              />
            ) : showQueryCollection ? (
              <QueryCollection
                onCancel={() => setShowQueryCollection(false)}
                databases={databases.results.map(database => database)}
                collections={collections}
                onQuery={handleExecuteQuery}
                selectedDatabase={selectedDatabase}
                fetchCollections={fetchCollections}
                handleUpdateRow={handleUpdateRow}
                handleDeleteRow={handleDeleteRow}
              />
            ) : showProfileView ? (
              <ProfileView
              onCancel={() => setShowProfileView(false)}
              username={username}
              onUpdateUser={handleUpdateUser}
              onGetUser={getUser}
            />
            ) : (
              <div className="tableConainer">
                {!selectedDatabase ? (
                  <div>
                    <h2 className='tableTitle'>Select a database to get started</h2>
                    <p className='tableTitle'>Or create a new database using the + button</p>
                  </div>
                ) : !selectedCollection ? (
                  <div>
                    <h2 className='tableTitle'>Select a collection to view documents</h2>
                    <p className='tableTitle'>Or create a new collection using the + button</p>
                    <button 
                      className="toggleButton"
                      onClick={handleExportDatabase}
                      title="Export database data"
                    >
                      <span className="icon">⤓</span> Export
                    </button>
                  </div>
                ) : (
                  <CollectionView
                    selectedDatabase={selectedDatabase}
                    selectedCollection={selectedCollection}
                    collectionData={collectionData}
                    handleUpdateRow={handleUpdateRow}
                    handleDeleteRow={handleDeleteRow}
                    handleInsertRow={() => setShowInsertCollection(true)}
                    handleDeleteCollection={handleDeleteCollection}
                    handleExportCollection={handleExportCollection}
                  />
                )}
              </div>
            )}
          </div>
        </>
      )}
    </div>
  );
};

export default App;