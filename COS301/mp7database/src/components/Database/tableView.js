const TableView = ({
  selectedTable,
  tableData,
  handleUpdateRow,
  handleDeleteRow,
  handleInsertRow,
  handleDeleteTable,
  handleExportTable,
}) => {
  return (
    <div className="tableContainer">
      <h2 className="tableTitle">Data in {selectedTable}</h2>
      <div className="buttonGroup">
        <button className="actionButton" onClick={handleInsertRow}>
          Insert Data
        </button>
        <button className="actionButton" onClick={handleDeleteTable}>
          Delete Table
        </button>
        <button className="actionButton" onClick={handleExportTable}>
          Export Table
        </button>
      </div>
      <table className="dataTable">
        <thead>
          <tr>
            {tableData.length > 0 &&
              Object.keys(tableData[0]).map((key) => (
                <th key={key} className="tableHeader">
                  {key}
                </th>
              ))}
            <th className="tableHeader">Update</th>
            <th className="tableHeader">Delete</th>
          </tr>
        </thead>
        <tbody>
          {tableData.map((row, index) => (
            <tr key={index} className="tableRow">
              {Object.values(row).map((value, i) => (
                <td key={i} className="tableCell">
                  {typeof value === 'boolean' ? value.toString() : value}
                </td>
              ))}
              <td className="tableCell">
                <button
                  className="updateButton"
                  onClick={() => handleUpdateRow(row)}
                >
                  Update
                </button>
              </td>
              <td className="tableCell">
                <button
                  className="deleteButton"
                  onClick={() => handleDeleteRow(row)}
                >
                  Delete
                </button>
              </td>
            </tr>
          ))}
        </tbody>
      </table>
    </div>
  );
};

export default TableView;