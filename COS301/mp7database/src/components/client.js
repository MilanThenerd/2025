import { useState, useEffect } from 'react';

const mockApi = {
  fetchTables: async () => {
    return [
      { name: 'users', columns: [
        { name: 'id', type: 'number' },
        { name: 'name', type: 'string' },
        { name: 'email', type: 'string' },
        { name: 'active', type: 'boolean' }
      ]},
      { name: 'products', columns: [
        { name: 'id', type: 'number' },
        { name: 'name', type: 'string' },
        { name: 'price', type: 'number' },
        { name: 'in_stock', type: 'boolean' }
      ]}
    ];
  },

  fetchTableData: async (tableName) => {
    if (tableName === 'users') {
      return [
        { id: 1, name: 'John Doe', email: 'john@example.com', active: true },
        { id: 2, name: 'Jane Smith', email: 'jane@example.com', active: false }
      ];
    } else if (tableName === 'products') {
      return [
        { id: 1, name: 'Laptop', price: 999.99, in_stock: true },
        { id: 2, name: 'Phone', price: 699.99, in_stock: false }
      ];
    }
    return [];
  },

  createTable: async (tableName, columns) => {
    return { name: tableName, columns };
  },

  insertRow: async (tableName, rowData) => {
    return { ...rowData, id: Math.floor(Math.random() * 10000) };
  },

  updateRow: async (tableName, rowId, updatedData) => {
    return { ...updatedData, id: rowId };
  },

  deleteRow: async (tableName, rowId) => {
    return true;
  },

  deleteTable: async (tableName) => {
    return true;
  },

  executeQuery: async (query) => {
    console.log("Mock query executed:", query);
    return [
      { id: 1, name: 'Sample Result', value: 'Test' }
    ];
  }
};

let api = typeof window.api !== 'undefined' ? window.api : mockApi;

export const useTableManagement = () => {
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

  useEffect(() => {
    const loadTables = async () => {
      setLoading(true);
      try {
        const fetchedTables = await api.fetchTables();
        setTables(fetchedTables);
        if (fetchedTables.length > 0) {
          setSelectedTable(fetchedTables[0].name);
        }
      } catch (err) {
        console.error("API failed, using mock data:", err.message);
        try {
          const mockTables = await mockApi.fetchTables();
          setTables(mockTables);
          if (mockTables.length > 0) {
            setSelectedTable(mockTables[0].name);
          }
        } catch (mockErr) {
          setError("Failed to load tables: " + mockErr.message);
        }
      } finally {
        setLoading(false);
      }
    };
    loadTables();
  }, []);

  useEffect(() => {
    if (!selectedTable) return;
    
    const loadTableData = async () => {
      setLoading(true);
      try {
        const data = await api.fetchTableData(selectedTable);
        setTableData(data);
        setError(null);
      } catch (err) {
        console.error("API failed, using mock data:", err.message);
        try {
          const mockData = await mockApi.fetchTableData(selectedTable);
          setTableData(mockData);
          setError(null);
        } catch (mockErr) {
          setError("Failed to load table data: " + mockErr.message);
          setTableData([]);
        }
      } finally {
        setLoading(false);
      }
    };
    loadTableData();
  }, [selectedTable]);

  const handleCreateTable = async (tableName, columns) => {
    try {
      setLoading(true);
      const newTable = await api.createTable(tableName, columns);
      setTables([...tables, newTable]);
      setSelectedTable(tableName);
      setShowCreateTable(false);
      setError(null);
    } catch (err) {
      console.error("API failed, using mock operation:", err.message);
      try {
        const mockTable = await mockApi.createTable(tableName, columns);
        setTables([...tables, mockTable]);
        setSelectedTable(tableName);
        setShowCreateTable(false);
        setError(null);
      } catch (mockErr) {
        setError("Failed to create table: " + mockErr.message);
      }
    } finally {
      setLoading(false);
    }
  };

  const handleImportTable = async (file) => {
    try {
      setLoading(true);
      const reader = new FileReader();
      reader.onload = async (e) => {
        try {
          const importedData = JSON.parse(e.target.result);
          const { tableName, columns } = importedData;

          let newTable;
          try {
            newTable = await api.createTable(tableName, columns);
          } catch (apiErr) {
            console.error("API failed, using mock operation:", apiErr.message);
            newTable = await mockApi.createTable(tableName, columns);
          }
          
          if (importedData.data && importedData.data.length > 0) {
            for (const row of importedData.data) {
              try {
                await api.insertRow(tableName, row);
              } catch (apiErr) {
                console.error("API failed, using mock operation:", apiErr.message);
                await mockApi.insertRow(tableName, row);
              }
            }
          }
          
          setTables([...tables, newTable]);
          setSelectedTable(tableName);
          setShowCreateTable(false);
          setError(null);
        } catch (err) {
          setError("Invalid JSON file or import failed: " + err.message);
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

  const handleDeleteTable = async () => {
    try {
      setLoading(true);
      try {
        await api.deleteTable(selectedTable);
      } catch (apiErr) {
        console.error("API failed, using mock operation:", apiErr.message);
        await mockApi.deleteTable(selectedTable);
      }
      setTables(tables.filter(table => table.name !== selectedTable));
      setSelectedTable(tables.length > 1 ? tables[0].name : '');
      setError(null);
    } catch (err) {
      setError(err.message);
    } finally {
      setLoading(false);
    }
  };

  const handleExportTable = () => {
    if (tableData.length === 0) {
      setError("No data to export.");
      return;
    }

    const selectedTableData = tables.find(table => table.name === selectedTable);
    if (!selectedTableData) {
      setError("No table selected.");
      return;
    }

    const exportData = {
      tableName: selectedTableData.name,
      columns: selectedTableData.columns,
      data: tableData,
    };

    const jsonString = JSON.stringify(exportData, null, 2);
    const blob = new Blob([jsonString], { type: 'application/json' });
    const url = URL.createObjectURL(blob);
    const link = document.createElement('a');
    link.href = url;
    link.download = `${selectedTableData.name}_export.json`;
    link.click();
    URL.revokeObjectURL(url);
  };

  const handleInsertRow = () => {
    setShowInsertTable(true);
  };

  const handleInsertRowData = async (newRow) => {
    try {
      setLoading(true);
      let insertedRow;
      try {
        insertedRow = await api.insertRow(selectedTable, newRow);
      } catch (apiErr) {
        console.error("API failed, using mock operation:", apiErr.message);
        insertedRow = await mockApi.insertRow(selectedTable, newRow);
      }
      setTableData([...tableData, insertedRow]);
      setShowInsertTable(false);
      setError(null);
    } catch (err) {
      setError(err.message);
    } finally {
      setLoading(false);
    }
  };

  const handleUpdateRow = (row) => {
    setRowToUpdate(row);
    setShowUpdateTable(true);
  };

  const handleSaveUpdatedRow = async (updatedRow) => {
    try {
      setLoading(true);
      let updated;
      try {
        updated = await api.updateRow(selectedTable, updatedRow.id, updatedRow);
      } catch (apiErr) {
        console.error("API failed, using mock operation:", apiErr.message);
        updated = await mockApi.updateRow(selectedTable, updatedRow.id, updatedRow);
      }
      const updatedData = tableData.map(row => 
        row.id === updatedRow.id ? updated : row
      );
      setTableData(updatedData);
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
      try {
        await api.deleteRow(selectedTable, rowToDelete.id);
      } catch (apiErr) {
        console.error("API failed, using mock operation:", apiErr.message);
        await mockApi.deleteRow(selectedTable, rowToDelete.id);
      }
      const updatedData = tableData.filter(row => row.id !== rowToDelete.id);
      setTableData(updatedData);
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
      let results;
      try {
        results = await api.executeQuery(query);
      } catch (apiErr) {
        console.error("API failed, using mock operation:", apiErr.message);
        results = await mockApi.executeQuery(query);
      }
      console.log('Query results:', results);
      return results;
    } catch (err) {
      setError(err.message);
      throw err;
    } finally {
      setLoading(false);
    }
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
    setHoveredTable,
    setDarkMode,
    handleCreateTable,
    handleImportTable,
    handleDeleteTable,
    handleExportTable,
    handleInsertRow,
    handleInsertRowData,
    handleUpdateRow,
    handleSaveUpdatedRow,
    handleDeleteRow,
    handleExecuteQuery,
    setShowCreateTable,
    setShowInsertTable,
    setShowUpdateTable,
    setShowQueryTable,
    setSelectedTable,
    setError,
  };
};