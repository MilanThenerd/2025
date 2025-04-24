// import React, { useEffect } from 'react';
// import './HomePage.css';

// const HomePage = ({
//   username,
//   databases,
//   setShowHomePage,
//   setShowCreateDatabase,
//   setSelectedDatabase,
//   onNavigateToTables,
//   onLogout,
//   setDatabases, // For updating databases state
// }) => {
//   useEffect(() => {
//     console.log('HomePage databases updated:', databases);
//   }, [databases]);

//   const handleDatabaseClick = (databaseName) => {
//     if (typeof setSelectedDatabase !== 'function') {
//       console.warn('setSelectedDatabase is not a function. Check useTableManagement return value.');
//       return;
//     }
//     setShowHomePage(false);
//     setShowCreateDatabase(false);
//     setSelectedDatabase(databaseName);
//   };

//   const handleDeleteDatabase = (databaseName) => {
//     if (window.confirm(`Are you sure you want to delete ${databaseName}?`)) {
//       setDatabases(databases.filter((db) => db.name !== databaseName));
//       if (setSelectedDatabase && databaseName === databases.find((db) => db.name === databaseName)?.name) {
//         setSelectedDatabase(null);
//       }
//     }
//   };

//   return (
//     <div className="homePageWrapper">
//       {/* Left Section */}
//       <div className="homePageContainer">
//         <h1>Welcome, {username}!</h1>
//         <p>This is your database management dashboard.</p>
//         <div className="homePageActions">
//           <button
//             onClick={onNavigateToTables}
//             className="navigateButton"
//             aria-label="Create New Database"
//           >
//             Create New Database
//           </button>
//           <button
//             onClick={onLogout}
//             className="logoutButton"
//             aria-label="Log out"
//           >
//             Logout
//           </button>
//         </div>
//       </div>

//       {/* Right Section (Purple) */}
//       <div className="databasesMirrorSection">
//         <h2>Databases Overview</h2>
//         {!databases || databases.length === 0 ? (
//           <p>No databases to display.</p>
//         ) : (
//           <ul className="databaseList">
//             {databases.map((database) => (
//               <div key={database.name} className="databaseRow">
//                 <li
//                   className="databaseItem"
//                   onClick={() => handleDatabaseClick(database.name)}
//                 >
//                   <span>{database.name}</span>
//                 </li>
//                 <button
//                   onClick={(e) => {
//                     e.stopPropagation(); // Prevent triggering any parent click events
//                     handleDeleteDatabase(database.name);
//                   }}
//                   className="deleteButton"
//                   aria-label={`Delete database ${database.name}`}
//                 >
//                   x
//                 </button>
//               </div>
//             ))}
//           </ul>
//         )}
//       </div>
//     </div>
//   );
// };

// export default HomePage;


import React, { useEffect } from 'react';
import './HomePage.css';

const HomePage = ({
  username,
  databases,
  setShowHomePage,
  setShowCreateDatabase,
  setSelectedDatabase,
  onNavigateToTables,
  onLogout,
  handleDeleteDatabase, // Use prop instead of setDatabases
}) => {
  useEffect(() => {
    console.log('HomePage databases updated:', databases);
  }, [databases]);

  const handleDatabaseClick = (databaseName) => {
    if (typeof setSelectedDatabase !== 'function') {
      console.warn('setSelectedDatabase is not a function. Check useTableManagement return value.');
      return;
    }
    setShowHomePage(false);
    setShowCreateDatabase(false);
    setSelectedDatabase(databaseName);
  };

  return (
    <div className="homePageWrapper">
      {/* Left Section */}
      <div className="homePageContainer">
        <h1>Welcome, {username}!</h1>
        <p>This is your database management dashboard.</p>
        <div className="homePageActions">
          <button
            onClick={onNavigateToTables}
            className="navigateButton"
            aria-label="Create New Database"
          >
            Create New Database
          </button>
          <button
            onClick={onLogout}
            className="logoutButton"
            aria-label="Log out"
          >
            Logout
          </button>
        </div>
      </div>

      {/* Right Section (Purple) */}
      <div className="databasesMirrorSection">
        <h2>Databases Overview</h2>
        {!databases || databases.length === 0 ? (
          <p>No databases to display.</p>
        ) : (
          <ul className="databaseList">
            {databases.map((database) => (
              <div key={database.name} className="databaseRow">
                <li
                  className="databaseItem"
                  onClick={() => handleDatabaseClick(database.name)}
                >
                  <span>{database.name}</span>
                </li>
                <button
                  onClick={(e) => {
                    e.stopPropagation();
                    handleDeleteDatabase(database.name);
                  }}
                  className="deleteButton"
                  aria-label={`Delete database ${database.name}`}
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