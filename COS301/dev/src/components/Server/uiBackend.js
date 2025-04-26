import { useState, useEffect, useCallback } from 'react';
import api from './api';

const useCollectionManagement = () => {
  const [isLoggedIn, setIsLoggedIn] = useState(false);
  const [authToken, setAuthToken] = useState(null);
  const [collections, setCollections] = useState([]);
  const [databases, setDatabases] = useState([]);
  const [selectedDatabase, setSelectedDatabase] = useState(null);
  const [selectedCollection, setSelectedCollection] = useState(null);
  const [collectionData, setCollectionData] = useState([]);
  const [loading, setLoading] = useState(false);
  const [error, setError] = useState(null);
  const [showCreateCollection, setShowCreateCollection] = useState(false);
  const [showInsertCollection, setShowInsertCollection] = useState(false);
  const [showUpdateCollection, setShowUpdateCollection] = useState(false);
  const [showQueryCollection, setShowQueryCollection] = useState(false);
  const [rowToUpdate, setRowToUpdate] = useState(null);
  const [hoveredCollection, setHoveredCollection] = useState(null);
  const [darkMode, setDarkMode] = useState(false);
  const [username, setUsername] = useState('');
  const [rememberMe, setRememberMe] = useState(false);
  const [hoveredDatabase, setHoveredDatabase] = useState(null);

  const fetchDatabases = useCallback(async () => {
    try {
      return await api.getAllDatabases();
    } catch (err) {
      throw new Error("Failed to load databases: " + err.message);
    }
  }, []);

  const createDatabase = async (dbName) => {
    try {
      return await api.createDatabase(dbName);
    } catch (err) {
      throw new Error("Failed to create database: " + err.message);
    }
  };

  const deleteDatabase = async (dbName) => {
    try {
      return await api.deleteDatabase(dbName);
    } catch (err) {
      throw new Error("Failed to delete database: " + err.message);
    }
  };

  const fetchCollections = useCallback(async (dbName) => {
    try {
      const collections = await api.getAllCollections(dbName);
      return collections.map(collection => ({
        name: collection.name,
        columns: collection.columns || []
      }));
    } catch (err) {
      throw new Error("Failed to load collections: " + err.message);
    }
  }, []);

  const createCollection = async (dbName, collectionName) => {
    try {
      return await api.createCollection(dbName, collectionName);
    } catch (err) {
      throw new Error("Failed to create collection: " + err.message);
    }
  };

  const deleteCollection = async (dbName, collectionName) => {
    try {
      return await api.deleteCollection(dbName, collectionName);
    } catch (err) {
      throw new Error("Failed to delete collection: " + err.message);
    }
  };

  const fetchCollectionData = useCallback(async (dbName, collectionName) => {
    try {
      return await api.getAllDocuments(dbName, collectionName);
    } catch (err) {
      throw new Error("Failed to load collection data: " + err.message);
    }
  }, []);

  const insertDocument = async (dbName, collectionName, document) => {
    try {
      return await api.createDocument(dbName, collectionName, document);
    } catch (err) {
      throw new Error("Failed to insert document: " + err.message);
    }
  };

  const updateDocument = async (dbName, collectionName, docId, updatedData) => {
    try {
      return await api.updateDocument(dbName, collectionName, docId, updatedData);
    } catch (err) {
      throw new Error("Failed to update document: " + err.message);
    }
  };

  const deleteDocument = async (dbName, collectionName, docId) => {
    try {
      return await api.deleteDocument(dbName, collectionName, docId);
    } catch (err) {
      throw new Error("Failed to delete document: " + err.message);
    }
  };

  const executeQuery = async (dbName, query) => {
    try {
      return await api.queryDocuments(dbName, query);
    } catch (err) {
      throw new Error("Query execution failed: " + err.message);
    }
  };

  useEffect(() => {
    const verifyAuth = async () => {
      const token = localStorage.getItem('authToken');
      if (token) {
        setLoading(true);
        try {
          const { valid } = await api.verifyToken(token);
          if (valid) {
            setIsLoggedIn(true);
            setAuthToken(token);
            setRememberMe(true);
          }
        } catch (err) {
          localStorage.removeItem('authToken');
        } finally {
          setLoading(false);
        }
      }
    };

    verifyAuth();
  }, []);

  useEffect(() => {
    const loadDatabases = async () => {
      if (!isLoggedIn) return;
      setLoading(true);
      try {
        const dbList = await fetchDatabases();
        setDatabases(dbList);
        if (dbList.length > 0 && !selectedDatabase) {
          setSelectedDatabase(dbList[0].name);
        }
      } catch (err) {
        setError(err.message);
      } finally {
        setLoading(false);
      }
    };

    loadDatabases();
  }, [isLoggedIn, fetchDatabases]);

  useEffect(() => {
    const loadCollections = async () => {
      if (!selectedDatabase) return;
      setLoading(true);
      try {
        const collectionList = await fetchCollections(selectedDatabase);
        setCollections(collectionList);

        const collectionExist = collectionList.some(
          collection => collection.name === selectedCollection
        );
        if (collectionList.length > 0 && (!selectedCollection || !collectionExist)) {
          setSelectedCollection(collectionList[0].name);
        }
        setSelectedCollection('');
      } catch (err) {
        setError(err.message);
      } finally {
        setLoading(false);
      }
    };

    loadCollections();
  }, [selectedDatabase, fetchCollections]);

  useEffect(() => {
    const loadCollectionData = async () => {
      if (!selectedDatabase || !selectedCollection) return;
      setLoading(true);
      try {
        const data = await fetchCollectionData(selectedDatabase, selectedCollection);
        setCollectionData(data);
        setError(null);
      } catch (err) {
        setError(err.message);
        setCollectionData([]);
      } finally {
        setLoading(false);
      }
    };

    loadCollectionData();
  }, [selectedDatabase, selectedCollection, fetchCollectionData]);

  const getUser = async (username) => {
    setLoading(true);
    try {
      const response = await api.getUser(username);
      return response.user || response;
    } catch (err) {
      setError(err.message);
      throw err;
    } finally {
      setLoading(false);
    }
  };

  const handleUpdateUser = async (username, data) => {
    setLoading(true);
    try {
      const response = await api.updateUser(username, data);
      if (response?.success) {
        setUsername(response.user?.username || username);
        return true;
      } else {
        throw new Error('Invalid data');
      }
    } catch (err) {
      setError(err.message);
      return false;
    } finally {
      setLoading(false);
    }
  };

  const handleLogin = async (username, password) => {
    setLoading(true);
    try {
      const response = await api.loginUser({username, password});
      if (response.message === "Login successful") {
        setIsLoggedIn(true);
        setAuthToken(response.token);
        setUsername(response.user?.username || username);
        if (rememberMe) {
          localStorage.setItem('authToken', response.token);
        }
      } else {
        throw new Error('Invalid login response');
      }
    } catch (err) {
      setError(err.message);
    } finally {
      setLoading(false);
    }
  };

  const handleRegister = async (username, email, password) => {
    setLoading(true);
    try {
      const response = await api.registerUser({ username, email, password });
      if (!response?.success) {
        throw new Error('Registration failed');
      }
      return true;
    } catch (err) {
      setError(err.message);
      return false;
    } finally {
      setLoading(false);
    }
  };

  const handleLogout = () => {
    localStorage.removeItem('authToken');
    setIsLoggedIn(false);
    setUsername('');
    setDatabases([]);
    setCollections([]);
    setCollectionData([]);
    setSelectedDatabase(null);
    setSelectedCollection(null);
    setRememberMe(false);
  };

  const handleCreateDatabase = async (dbName) => {
    try {
      setLoading(true);
      await createDatabase(dbName);
      const updatedDbs = await fetchDatabases();
      setDatabases(updatedDbs);
      setError(null);
    } catch (err) {
      setError(err.message);
    } finally {
      setLoading(false);
    }
  };

  const handleDeleteDatabase = async (dbName) => {
    try {
      setLoading(true);
      await deleteDatabase(dbName);
      const updatedDbs = await fetchDatabases();
      setDatabases(updatedDbs);
      if (selectedDatabase === dbName) {
        setSelectedDatabase(updatedDbs.length > 0 ? updatedDbs[0].name : null);
      }
      setError(null);
    } catch (err) {
      setError(err.message);
    } finally {
      setLoading(false);
    }
  };

  const handleCreateCollection = async (collectionName) => {
    try {
      setLoading(true);
      await createCollection(selectedDatabase, collectionName);
      const updatedCollections = await fetchCollections(selectedDatabase);
      setCollections(updatedCollections);
      setSelectedCollection(collectionName);
      setShowCreateCollection(false);
      setError(null);
    } catch (err) {
      setError(err.message);
    } finally {
      setLoading(false);
    }
  };

  const handleDeleteCollection = async () => {
    try {
      setLoading(true);
      await deleteCollection(selectedDatabase, selectedCollection);
      const updatedCollections = await fetchCollections(selectedDatabase);
      setCollections(updatedCollections);
      setSelectedCollection(updatedCollections.length > 0 ? updatedCollections[0].name : null);
      setError(null);
    } catch (err) {
      setError(err.message);
    } finally {
      setLoading(false);
    }
  };

  const handleInsertRowData = async (newRow) => {
    try {
      setLoading(true);
      const insertedRow = await insertDocument(selectedDatabase, selectedCollection, newRow);
      setCollectionData([...collectionData, insertedRow]);
      setShowInsertCollection(false);
      setError(null);
    } catch (err) {
      setError(err.message);
    } finally {
      setLoading(false);
    }
  };

  const handleSaveUpdatedRow = async (updatedRow) => {
    try {
      setLoading(true);
      const updated = await updateDocument(
        selectedDatabase, 
        selectedCollection, 
        updatedRow.id, 
        updatedRow
      );
      setCollectionData(collectionData.map(row => row.id === updatedRow.id ? updated : row));
      setShowUpdateCollection(false);
      setError(null);
    } catch (err) {
      setError(err.message);
    } finally {
      setLoading(false);
    }
  };

  const handleDeleteRow = async (rowToDelete) => {
    try {
      setLoading(true);
      await deleteDocument(selectedDatabase, selectedCollection, rowToDelete.id);
      setCollectionData(collectionData.filter(row => row.id !== rowToDelete.id));
      setError(null);
    } catch (err) {
      setError(err.message);
    } finally {
      setLoading(false);
    }
  };

  const handleExecuteQuery = async (query) => {
    try {
      setLoading(true);
      const results = await executeQuery(selectedDatabase, query);
      return results;
    } catch (err) {
      setError(err.message);
      throw err;
    } finally {
      setLoading(false);
    }
  };

  const handleImportCollection = async (file) => {
    setLoading(true);
    try {
      const reader = new FileReader();
      
      return new Promise((resolve, reject) => {
        reader.onload = async (e) => {
          try {
            const importData = JSON.parse(e.target.result);
            
            if (!importData.collectionName || !importData.columns || !importData.data) {
              throw new Error("Invalid import file format");
            }
            
            await handleCreateCollection(importData.collectionName);

            if (importData.data?.length) {
              for (const row of importData.data) {
                await handleInsertRowData(row);
              }
            }
            const data = await fetchCollectionData(selectedDatabase, importData.collectionName);
            setCollectionData(data);
            
            resolve(true);
          } catch (err) {
            setError("Import failed: " + err.message);
            reject(err);
          } finally {
            setLoading(false);
          }
        };
        
        reader.onerror = () => {
          setError("Failed to read file");
          setLoading(false);
          reject(new Error("File read error"));
        };
        
        reader.readAsText(file);
      });
    } catch (err) {
      setError(err.message);
      setLoading(false);
      throw err;
    }
  };

  const handleExportDatabase = async () => {
    if (!selectedDatabase || databases.length === 0) {
      setError("No database selected or no databases available");
      return;
    }
  
    try {
      setLoading(true);
      setError(null);
      
      const database = databases.find(d => d.name === selectedDatabase);
      if (!database) {
        setError("Database not found");
        return;
      }

      const allCollections = await fetchCollections(selectedDatabase);
      
      const collectionsWithData = await Promise.all(
        allCollections.map(async (collection) => {
          const data = await fetchCollectionData(selectedDatabase, collection.name);
          return {
            name: collection.name,
            columns: collection.columns,
            data: data
          };
        })
      );
  
      const exportData = {
        databaseName: database.name,
        createdAt: database.createdAt,
        collections: collectionsWithData,
        exportedAt: new Date().toISOString()
      };
  
      const blob = new Blob([JSON.stringify(exportData, null, 2)], { 
        type: 'application/json' 
      });
      const url = URL.createObjectURL(blob);
      const link = document.createElement('a');
      link.href = url;
      link.download = `${selectedDatabase}_full_export_${new Date().toISOString().slice(0,10)}.json`;
      link.click();
      URL.revokeObjectURL(url);
    } catch (err) {
      setError(`Export failed: ${err.message}`);
    } finally {
      setLoading(false);
    }
  };

  const handleExportCollection = () => {
    if (!selectedCollection || collectionData.length === 0) {
      setError("No data to export");
      return;
    }

    const collection = collections.find(c => c.name === selectedCollection);
    if (!collection) {
      setError("Collection not found");
      return;
    }

    const exportData = {
      collectionName: collection.name,
      columns: collection.columns,
      data: collectionData
    };

    const blob = new Blob([JSON.stringify(exportData, null, 2)], { type: 'application/json' });
    const url = URL.createObjectURL(blob);
    const link = document.createElement('a');
    link.href = url;
    link.download = `${selectedDatabase}_${selectedCollection}_export.json`;
    link.click();
    URL.revokeObjectURL(url);
  };

  return {
    isLoggedIn,
    authToken,
    username,
    rememberMe,
    databases,
    selectedDatabase,
    collections,
    selectedCollection,
    collectionData,
    loading,
    error,
    showCreateCollection,
    showInsertCollection,
    showUpdateCollection,
    showQueryCollection,
    rowToUpdate,
    hoveredCollection,
    darkMode,
    hoveredDatabase,
    getUser,

    setSelectedDatabase,
    setSelectedCollection,
    setShowCreateCollection,
    setShowInsertCollection,
    setShowUpdateCollection,
    setShowQueryCollection,
    setHoveredCollection,
    setHoveredDatabase,
    setDarkMode,
    setError,
    setIsLoggedIn,
    setRememberMe,

    handleLogin,
    handleRegister,
    handleUpdateUser,
    handleLogout,
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
    handleInsertRow: () => setShowInsertCollection(true),
    handleUpdateRow: (row) => {
      setRowToUpdate(row);
      setShowUpdateCollection(true);
      setShowQueryCollection(false);
    },

    fetchCollections
  };
};

export default useCollectionManagement;
