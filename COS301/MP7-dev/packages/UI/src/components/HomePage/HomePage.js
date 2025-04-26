import React, { useEffect, useState } from 'react';
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
  setCollections,
  setCollectionData,
}) => {

  const [counts, setCounts] = useState({});
  useEffect(() => {
    console.log('HomePage databases:', databases);
  }, [databases]);

  useEffect(() => {
    const fetchDatabaseCounts = async () => {
      if (!databases.results || databases.results.length === 0) return;
      try {
        const newCounts = {};
        
        for (const db of databases.results) {
          const collections = await setCollections(db.database);
          let totalDocuments = 0;
          
          for (const collection of collections) {
            const documents = await setCollectionData(db.database ,collection.name);
            totalDocuments += documents?.data?.length || 0;
          }
          
          newCounts[db.database] = {
            collections: collections.length,
            documents: totalDocuments
          };
        }
        
        setCounts(newCounts);
      } catch (error) {
        console.error("Error fetching database counts:", error);
      } 
    };

    fetchDatabaseCounts();
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
                  ({counts[db.database]?.collections || 0} collections,{' '}
                    {counts[db.database]?.documents || 0} documents)
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
