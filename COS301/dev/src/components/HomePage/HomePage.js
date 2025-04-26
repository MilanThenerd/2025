import React, { useEffect } from 'react';
import './HomePage.css';

const HomePage = ({
  username,
  databases = { results: [] },
  setShowHomePage,
  setShowCreateDatabase,
  setSelectedDatabase,
  onNavigateToTables,
  onLogout,
  handleDeleteDatabase,
}) => {
  useEffect(() => {
    console.log('HomePage databases:', databases);
  }, [databases]);

  const handleDatabaseClick = (databaseName) => {
    if (typeof setSelectedDatabase !== 'function') {
      console.warn('setSelectedDatabase is not a function');
      return;
    }
    setShowHomePage(false);
    setShowCreateDatabase(false);
    setSelectedDatabase(databaseName);
  };

  const confirmDelete = (databaseName, e) => {
    e.stopPropagation();
    const isConfirmed = window.confirm(
      `Are you sure you want to delete the database "${databaseName}"? This action cannot be undone.`
    );
    if (isConfirmed) {
      handleDeleteDatabase(databaseName);
    }
  };

  return (
    <div className="homePageWrapper">
      {/* Left Section */}
      <div className="homePageContainer">
        <h1>Welcome, {username}!</h1>
        <p>This is your database management dashboard.</p>
        <div className="homePageActions">
          <button onClick={onNavigateToTables} className="navigateButton">
            Create New Database
          </button>
          <button onClick={onLogout} className="logoutButton">
            Logout
          </button>
        </div>
      </div>

      {/* Right Section */}
      <div className="databasesMirrorSection">
        <h2>Databases Overview</h2>
        {!databases.results || databases.results.length === 0 ? (
          <p>No databases to display.</p>
        ) : (
          <ul className="databaseList">
            {databases.results.map((db) => (
              <div key={db.database} className="databaseRow">
                <li
                  className="databaseItem"
                  onClick={() => handleDatabaseClick(db.database)}
                >
                  <span>{db.database}</span>
                  <span className="databaseStats">
                    ({db.collections.length} collections, {db.count} documents)
                  </span>
                </li>
                <button
                  onClick={(e) => confirmDelete(db.database, e)}
                  className="deleteButton"
                >
                  x
                </button>
              </div>
            ))}
          </ul>
        )}
      </div>
    </div>
  );
};

export default HomePage;