import { useState, useEffect, useCallback } from 'react';
import api from './api';

const mockApi = {
  fetchTables: async () => [
    { 
      name: 'users', 
      columns: [
        { name: 'id', type: 'number' },
        { name: 'name', type: 'string' },
        { name: 'email', type: 'string' },
        { name: 'active', type: 'boolean' }
      ]
    },
    { 
      name: 'products', 
      columns: [
        { name: 'id', type: 'number' },
        { name: 'name', type: 'string' },
        { name: 'price', type: 'number' },
        { name: 'in_stock', type: 'boolean' }
      ]
    }
  ],

  fetchTableData: async (tableName) => {
    if (tableName === 'users') {
      return [
        { id: 1, name: 'John Doe', email: 'john@example.com', active: true },
        { id: 2, name: 'Jane Smith', email: 'jane@example.com', active: false }
      ];
    }
    if (tableName === 'products') {
      return [
        { id: 1, name: 'Laptop', price: 999.99, in_stock: true },
        { id: 2, name: 'Phone', price: 699.99, in_stock: false }
      ];
    }
    return [];
  },

  createTable: async (tableName, columns) => ({ name: tableName, columns }),
  insertRow: async (tableName, rowData) => ({ ...rowData, id: Math.floor(Math.random() * 10000) }),
  updateRow: async (tableName, rowId, updatedData) => ({ ...updatedData, id: rowId }),
  deleteRow: async (tableName, rowId) => true,
  deleteTable: async (tableName) => true,
  executeQuery: async (query) => [
    { id: 1, name: 'John', surname: 'Doe' }, 
    {id: 2 , name:'Leroy' , surname:'Jenkins'}
  ],
  login: async (username, password) => ({ success: true, token: 'mock-token' }),
  register: async (username, email, password) => ({ success: true }),
  verifyToken: async (token) => ({ valid: true })
};

const useTableManagement = () => {
  const [isLoggedIn, setIsLoggedIn] = useState(false);
  const [authToken, setAuthToken] = useState(null);
  const [tables, setTables] = useState([]);
  const [selectedTable, setSelectedTable] = useState('');
  const [tableData, setTableData] = useState([]);
  const [loading, setLoading] = useState(false);
  const [error, setError] = useState(null);
  const [showCreateTable, setShowCreateTable] = useState(false);
  const [showInsertTable, setShowInsertTable] = useState(false);
  const [showUpdateTable, setShowUpdateTable] = useState(false);
  const [showQueryTable, setShowQueryTable] = useState(false);
  const [rowToUpdate, setRowToUpdate] = useState(null);
  const [hoveredTable, setHoveredTable] = useState(null);
  const [darkMode, setDarkMode] = useState(false);
  const [username, setUsername] = useState('');
  const [rememberMe , setRememberMe] = useState(false);

  const callApiWithFallback = async (apiCall, mockCall, errorMessage) => {
    try {
      return await apiCall();
    } catch (apiErr) {
      console.error("API failed, trying mock:", apiErr.message);
      try {
        return await mockCall();
      } catch (mockErr) {
        throw new Error(`${errorMessage}: ${mockErr.message}`);
      }
    }
  };

  const fetchTables = useCallback(async () => {
    return callApiWithFallback(
      async () => {
        const collections = await api.getAllCollections('main-database');
        return collections.map(collection => ({
          name: collection.name,
          columns: collection.columns || []
        }));
      },
      () => mockApi.fetchTables(),
      "Failed to load tables"
    );
  }, []);

  const fetchTableData = useCallback(async (tableName) => {
    return callApiWithFallback(
      async () => {
        const documents = await api.getAllDocuments('main-database', tableName);
        return documents;
      },
      () => mockApi.fetchTableData(tableName),
      "Failed to load table data"
    );
  }, []);

  useEffect(() => {
    const loadTables = async () => {
      setLoading(true);
      try {
        const fetchedTables = await fetchTables();
        setTables(fetchedTables);
        if (fetchedTables.length > 0 && !selectedTable) {
          setSelectedTable(fetchedTables[0].name);
        }
      } catch (err) {
        setError(err.message);
      } finally {
        setLoading(false);
      }
    };

    if (isLoggedIn) loadTables();
  }, [isLoggedIn, fetchTables, selectedTable]);

  useEffect(() => {
    const loadTableData = async () => {
      if (!selectedTable) return;
      setLoading(true);
      try {
        const data = await fetchTableData(selectedTable);
        setTableData(data);
        setError(null);
      } catch (err) {
        setError(err.message);
        setTableData([]);
      } finally {
        setLoading(false);
      }
    };

    loadTableData();
  }, [selectedTable, fetchTableData]);

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

  const handleLogin = async (username, password) => {
    setLoading(true);
    try {
      const response = await callApiWithFallback(
        () => api.loginUser(username + password),
        () => mockApi.login(username, password),
        "Login failed"
      );

      if (response?.token) {
        setIsLoggedIn(true);
        setAuthToken(response.token);
        setUsername(username);
        if(rememberMe)
        {
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
      const response = await callApiWithFallback(
        () => api.registerUser({ username, email, password }),
        () => mockApi.register(username, email, password),
        "Registration failed"
      );

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
    setTables([]);
    setTableData([]);
    setSelectedTable('');
    setRememberMe(false);
  };

  const handleCreateTable = async (tableName, columns) => {
    try {
      setLoading(true);
      await callApiWithFallback(
        () => api.createCollection('main-database', tableName),
        () => mockApi.createTable(tableName, columns),
        "Failed to create table"
      );
      
      setTables([...tables, { name: tableName, columns }]);
      setSelectedTable(tableName);
      setShowCreateTable(false);
      setError(null);
    } catch (err) {
      setError(err.message);
    } finally {
      setLoading(false);
    }
  };

  const handleDeleteTable = async () => {
    try {
      setLoading(true);
      await callApiWithFallback(
        () => api.dropCollection('main-database', selectedTable),
        () => mockApi.deleteTable(selectedTable),
        "Failed to delete table"
      );
      
      const newTables = tables.filter(table => table.name !== selectedTable);
      setTables(newTables);
      setSelectedTable(newTables.length > 0 ? newTables[0].name : '');
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
      const insertedRow = await callApiWithFallback(
        () => api.createDocument('main-database', selectedTable, newRow),
        () => mockApi.insertRow(selectedTable, newRow),
        "Failed to insert row"
      );
      
      setTableData([...tableData, insertedRow]);
      setShowInsertTable(false);
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
      const updated = await callApiWithFallback(
        () => api.updateDocument('main-database', selectedTable, updatedRow.id, updatedRow),
        () => mockApi.updateRow(selectedTable, updatedRow.id, updatedRow),
        "Failed to update row"
      );
      
      setTableData(tableData.map(row => row.id === updatedRow.id ? updated : row));
      setShowUpdateTable(false);
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
      await callApiWithFallback(
        () => api.deleteDocument('main-database', selectedTable, rowToDelete.id),
        () => mockApi.deleteRow(selectedTable, rowToDelete.id),
        "Failed to delete row"
      );
      
      setTableData(tableData.filter(row => row.id !== rowToDelete.id));
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
      const results = await callApiWithFallback(
        () => api.queryDocuments(selectedTable || 'default', query),
        () => mockApi.executeQuery(query),
        "Query execution failed"
      );
      return results;
    } catch (err) {
      setError(err.message);
      throw err;
    } finally {
      setLoading(false);
    }
  };

  const handleImportTable = async (file) => {
    setLoading(true);
    try {
      const reader = new FileReader();
      reader.onload = async (e) => {
        try {
          const { tableName, columns, data } = JSON.parse(e.target.result);
          
          await handleCreateTable(tableName, columns);
          
          if (data?.length) {
            for (const row of data) {
              await handleInsertRowData(row);
            }
          }
        } catch (err) {
          setError("Invalid import file: " + err.message);
        } finally {
          setLoading(false);
        }
      };
      reader.readAsText(file);
    } catch (err) {
      setError(err.message);
      setLoading(false);
    }
  };

  const handleExportTable = () => {
    if (!selectedTable || tableData.length === 0) {
      setError("No data to export");
      return;
    }

    const table = tables.find(t => t.name === selectedTable);
    if (!table) {
      setError("Table not found");
      return;
    }

    const exportData = {
      tableName: table.name,
      columns: table.columns,
      data: tableData
    };

    const blob = new Blob([JSON.stringify(exportData, null, 2)], { type: 'application/json' });
    const url = URL.createObjectURL(blob);
    const link = document.createElement('a');
    link.href = url;
    link.download = `${table.name}_export.json`;
    link.click();
    URL.revokeObjectURL(url);
  };

  return {
    tables,
    selectedTable,
    tableData,
    loading,
    error,
    showCreateTable,
    showInsertTable,
    showUpdateTable,
    showQueryTable,
    rowToUpdate,
    hoveredTable,
    darkMode,
    isLoggedIn,
    authToken,
    username,
    
    setHoveredTable,
    setDarkMode,
    setShowCreateTable,
    setShowInsertTable,
    setShowUpdateTable,
    setShowQueryTable,
    setSelectedTable,
    setError,
    setIsLoggedIn,
    setRememberMe,
    rememberMe,

    handleLogin,
    handleRegister,
    handleLogout,
    handleCreateTable,
    handleImportTable,
    handleDeleteTable,
    handleExportTable,
    handleInsertRow: () => setShowInsertTable(true),
    handleInsertRowData,
    handleUpdateRow: (row) => {
      setRowToUpdate(row);
      setShowUpdateTable(true);
    },
    handleSaveUpdatedRow,
    handleDeleteRow,
    handleExecuteQuery
  };
};

export default useTableManagement;