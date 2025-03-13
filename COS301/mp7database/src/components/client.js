import { useState, useEffect } from 'react';

export const useTableManagement = () => {
  const [tables, setTables] = useState([]);
  const [selectedTable, setSelectedTable] = useState('');
  const [tableData, setTableData] = useState([]);
  const [showCreateTable, setShowCreateTable] = useState(false);
  const [showInsertTable, setShowInsertTable] = useState(false);
  const [showUpdateTable, setShowUpdateTable] = useState(false);
  const [rowToUpdate, setRowToUpdate] = useState(null);
  const [hoveredTable, setHoveredTable] = useState(null);
  const [darkMode, setDarkMode] = useState(false);

  useEffect(() => {
    if (tables.length > 0) {
      setSelectedTable(tables[0].name);
    } else {
      setSelectedTable('');
    }
  }, [tables]);

  const handleCreateTable = (tableName, columns) => {
    const newTable = { name: tableName, columns };
    setTables([...tables, newTable]);
    setShowCreateTable(false);
  };

  const handleImportTable = (file) => {
    const reader = new FileReader();
    reader.onload = (e) => {
      try {
        const importedData = JSON.parse(e.target.result);
        const { tableName, columns, data } = importedData;

        const tableExists = tables.some((table) => table.name === tableName);
        if (tableExists) {
          alert(`Table "${tableName}" already exists.`);
          return;
        }

        setTables((prevTables) => [...prevTables, { name: tableName, columns }]);
        setTableData(data);
        setSelectedTable(tableName);
        setShowCreateTable('');
        alert(`Table "${tableName}" imported successfully.`);
      } catch (error) {
        alert("Invalid JSON file. Please upload a valid JSON file.");
      }
    };
    reader.readAsText(file);
  };

  const handleDeleteTable = () => {
    setTables(tables.filter((table) => table.name !== selectedTable));
    setSelectedTable('');
    setTableData([]);
  };

  const handleExportTable = () => {
    if (tableData.length === 0) {
      alert("No data to export.");
      return;
    }

    const selectedTableData = tables.find((table) => table.name === selectedTable);
    if (!selectedTableData) {
      alert("No table selected.");
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

  const handleInsertRowData = (newRow) => {
    setTableData([...tableData, newRow]);
    setShowInsertTable(false);
  };

  const handleUpdateRow = (row) => {
    setRowToUpdate(row);
    setShowUpdateTable(true);
  };

  const handleSaveUpdatedRow = (updatedRow) => {
    const updatedData = tableData.map((row) =>
      row === rowToUpdate ? updatedRow : row
    );
    setTableData(updatedData);
    setShowUpdateTable(false);
  };

  const handleDeleteRow = (rowToDelete) => {
    const updatedData = tableData.filter((row) => row !== rowToDelete);
    setTableData(updatedData);
  };

  return {
    tables,
    selectedTable,
    tableData,
    showCreateTable,
    showInsertTable,
    showUpdateTable,
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
    setShowCreateTable,
    setShowInsertTable,
    setShowUpdateTable,
    setSelectedTable,
  };
};