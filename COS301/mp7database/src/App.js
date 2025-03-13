import React from 'react';
import { useTableManagement } from './components/client';
import CreateTable from './components/createTable';
import InsertTable from './components/insertTable';
import UpdateTable from './components/updateTable';
import TableView   from './components/tableView';
import './App.css';

const App = () => {
  const {
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
  } = useTableManagement();

  return (
    <div className={`appContainer ${darkMode ? 'darkMode' : ''}`}>

      <div className="sidebar">
        <h2 className="sidebarTitle">Tables</h2>
        <ul className="tableList">
          {tables.map((table) => (
            <li
              key={table.name}
              onMouseEnter={() => setHoveredTable(table.name)}
              onMouseLeave={() => setHoveredTable(null)}
              onClick={() => {
                setSelectedTable(table.name);
                setShowCreateTable(false);
                setShowInsertTable(false);
              }}
              className={`tableItem ${hoveredTable === table.name ? 'tableItemHover' : ''} ${selectedTable === table.name ? 'tableItemSelected' : ''}`}
            >
              {table.name}
              <div className="underline"></div>
              {hoveredTable === table.name && (
                <div className="tableColumns">
                  <strong>Columns:</strong>{' '}
                  {table.columns.map((col) => `${col.name} (${col.type})`).join(', ')}
                </div>
              )}
            </li>
          ))}
        </ul>
        <button
          onClick={() => setShowCreateTable(true)}
          className="addTableButton"
        >
          + New Table
        </button>
        <button
          onClick={() => setDarkMode(!darkMode)}
          className="toggleButton"
        >
          {darkMode ? '☀️ Light Mode' : '🌙 Dark Mode'}
        </button>
      </div>

      <div className="mainContent">
        {showCreateTable ? (
          <CreateTable
            onCancel={() => setShowCreateTable(false)}
            onCreate={handleCreateTable}
            onImport={handleImportTable}
          />
        ) : showInsertTable ? (
          <InsertTable
            onCancel={() => setShowInsertTable(false)}
            onInsert={handleInsertRowData}
            columns={tables.find((table) => table.name === selectedTable)?.columns || []}
          />
        ) : showUpdateTable ? (
          <UpdateTable
            row={rowToUpdate}
            columns={tables.find((table) => table.name === selectedTable)?.columns || []}
            onSave={handleSaveUpdatedRow}
            onCancel={() => setShowUpdateTable(false)}
          />
        ) : (
          <TableView
            selectedTable={selectedTable}
            tableData={tableData}
            handleUpdateRow={handleUpdateRow}
            handleDeleteRow={handleDeleteRow}
            handleInsertRow={handleInsertRow}
            handleDeleteTable={handleDeleteTable}
            handleExportTable={handleExportTable}
          />
        )}
      </div>
    </div>
  );
};

export default App;