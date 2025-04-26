import React, { useState, useEffect } from 'react';

const UpdateDocument = ({ row, onSave, onCancel, selectedCollection }) => {
  const [rowData, setRowData] = useState(row);
  const [customColumns, setCustomColumns] = useState([]);
  const [newColumnName, setNewColumnName] = useState('');
  const [newColumnType, setNewColumnType] = useState('string');
  const [error, setError] = useState('');
  const [isSaving, setIsSaving] = useState(false);
  const [fieldTypes, setFieldTypes] = useState({});
  const [editingArrayIndex, setEditingArrayIndex] = useState(null);
  const [editingArrayField, setEditingArrayField] = useState(null);
  const [arrayEditValue, setArrayEditValue] = useState('');
  const [editingObjectField, setEditingObjectField] = useState(null);
  const [objectEditValue, setObjectEditValue] = useState('');

  const detectType = (value) => {
    if (value === null || value === undefined) return 'null';
    if (Array.isArray(value)) {
      if (value.length > 0 && typeof value[0] === 'object' && !Array.isArray(value[0])) {
        return 'arrayOfObjects';
      }
      return 'array';
    }
    if (typeof value === 'boolean') return 'boolean';
    if (typeof value === 'number') return 'number';
    if (typeof value === 'string' && !isNaN(Date.parse(value))) return 'date';
    if (typeof value === 'object') return 'object';
    return 'string';
  };

  useEffect(() => {
    const initialTypes = {};
    Object.keys(row).forEach(key => {
      if (key !== '_id') {
        initialTypes[key] = detectType(row[key]);
      }
    });
    setFieldTypes(initialTypes);
  }, [row]);

  const handleSave = async () => {
    try {
      setIsSaving(true);
      await onSave(rowData);
    } catch (err) {
      setError(err.message || 'Failed to update data');
    } finally {
      setIsSaving(false);
    }
  };

  const handleInputChange = (e, key, type) => {
    let value = e.target.value;
    setError('');

    if (type === 'number') {
      value = value === '' ? null : parseFloat(value);
      if (value !== null && isNaN(value)) {
        setError(`Invalid number for column: ${key}`);
        return;
      }
    } else if (type === 'boolean') {
      value = value.toLowerCase();
      if (value === 'true') value = true;
      else if (value === 'false') value = false;
      else value = null;
    } else if (type === 'date') {
      if (value) {
        const date = new Date(value);
        if (isNaN(date.getTime())) {
          setError(`Invalid date for column: ${key}. Use the format yyyy-mm-dd.`);
          return;
        }
        value = date.toISOString().split('T')[0];
      } else {
        value = null;
      }
    }

    setRowData({ ...rowData, [key]: value });
  };

  const changeFieldType = (fieldName, newType) => {
    setFieldTypes({ ...fieldTypes, [fieldName]: newType });
    
    let initialValue;
    switch (newType) {
      case 'array':
        initialValue = [];
        break;
      case 'arrayOfObjects':
        initialValue = [{}];
        break;
      case 'object':
        initialValue = {};
        break;
      case 'number':
        initialValue = null;
        break;
      case 'boolean':
        initialValue = null;
        break;
      case 'date':
        initialValue = null;
        break;
      default:
        initialValue = '';
    }
    
    setRowData({ ...rowData, [fieldName]: initialValue });
  };

  const removeField = (fieldName) => {
    const newRowData = { ...rowData };
    delete newRowData[fieldName];
    setRowData(newRowData);

    const newFieldTypes = { ...fieldTypes };
    delete newFieldTypes[fieldName];
    setFieldTypes(newFieldTypes);
  };

  const addCustomColumn = () => {
    if (!newColumnName.trim()) {
      setError('Column name cannot be empty');
      return;
    }

    if (fieldTypes[newColumnName]) {
      setError(`Field "${newColumnName}" already exists`);
      return;
    }

    const newColumn = { name: newColumnName, type: newColumnType };
    setFieldTypes({ ...fieldTypes, [newColumnName]: newColumnType });
    
    let initialValue;
    switch (newColumnType) {
      case 'array':
        initialValue = [];
        break;
      case 'arrayOfObjects':
        initialValue = [{}];
        break;
      case 'object':
        initialValue = {};
        break;
      case 'number':
        initialValue = null;
        break;
      case 'boolean':
        initialValue = null;
        break;
      case 'date':
        initialValue = null;
        break;
      default:
        initialValue = '';
    }
    
    setRowData({ ...rowData, [newColumnName]: initialValue });
    setNewColumnName('');
    setError('');
  };

  const handleArrayItemAdd = (fieldName, isObject = false) => {
    const currentArray = Array.isArray(rowData[fieldName]) ? rowData[fieldName] : [];
    const newItem = isObject ? {} : '';
    setRowData({
      ...rowData,
      [fieldName]: [...currentArray, newItem]
    });
    
    if (isObject) {
      setEditingArrayField(fieldName);
      setEditingArrayIndex(currentArray.length);
      setObjectEditValue(JSON.stringify(newItem, null, 2));
    } else {
      setEditingArrayField(fieldName);
      setEditingArrayIndex(currentArray.length);
      setArrayEditValue(newItem);
    }
  };

  const handleArrayItemEdit = (fieldName, index, value) => {
    const newArray = [...rowData[fieldName]];
    newArray[index] = value;
    setRowData({
      ...rowData,
      [fieldName]: newArray
    });
  };

  const handleArrayItemRemove = (fieldName, index) => {
    const newArray = [...rowData[fieldName]];
    newArray.splice(index, 1);
    setRowData({
      ...rowData,
      [fieldName]: newArray
    });
  };

  const saveArrayEdit = (fieldName) => {
    handleArrayItemEdit(fieldName, editingArrayIndex, arrayEditValue);
    setEditingArrayField(null);
    setEditingArrayIndex(null);
    setArrayEditValue('');
  };

  const handleArrayObjectEdit = (fieldName, index, value) => {
    try {
      const parsed = JSON.parse(value);
      const newArray = [...rowData[fieldName]];
      newArray[index] = parsed;
      setRowData({
        ...rowData,
        [fieldName]: newArray
      });
      return true;
    } catch (err) {
      setError('Invalid JSON format');
      return false;
    }
  };

  const saveArrayObjectEdit = (fieldName) => {
    if (handleArrayObjectEdit(fieldName, editingArrayIndex, objectEditValue)) {
      setEditingArrayField(null);
      setEditingArrayIndex(null);
      setObjectEditValue('');
    }
  };

  const startEditingObject = (fieldName) => {
    setEditingObjectField(fieldName);
    setObjectEditValue(JSON.stringify(rowData[fieldName] || {}, null, 2));
  };

  const saveObjectEdit = (fieldName) => {
    try {
      const parsed = JSON.parse(objectEditValue);
      setRowData({ ...rowData, [fieldName]: parsed });
      setEditingObjectField(null);
      setError('');
    } catch (err) {
      setError('Invalid JSON format');
    }
  };

  const renderObjectField = (fieldName, fieldValue) => {
    if (editingObjectField === fieldName) {
      return (
        <div className="object-field">
          <textarea
            value={objectEditValue}
            onChange={(e) => setObjectEditValue(e.target.value)}
            className={`inputField json-input ${error.includes('JSON') ? 'invalid' : ''}`}
            placeholder="Enter valid JSON object"
            rows={5}
          />
          <div className="object-edit-buttons">
            <button 
              onClick={() => saveObjectEdit(fieldName)}
              className="toggleButton small"
            >
              Save
            </button>
            <button
              onClick={() => {
                setEditingObjectField(null);
                setError('');
              }}
              className="toggleButton small"
            >
              Cancel
            </button>
          </div>
          {error && error.includes('JSON') && (
            <div className="json-error">{error}</div>
          )}
        </div>
      );
    }

    return (
      <div className="object-field">
        <pre>{JSON.stringify(fieldValue, null, 2)}</pre>
        <button
          onClick={() => startEditingObject(fieldName)}
          className="toggleButton small"
        >
          Edit
        </button>
      </div>
    );
  };

  const renderArrayField = (fieldName, fieldValue, fieldType) => {
    const isArrayOfObjects = fieldType === 'arrayOfObjects';
    
    return (
      <div className="array-field">
        <div className="array-items">
          {fieldValue.map((item, index) => (
            <div key={index} className="array-item">
              {editingArrayField === fieldName && editingArrayIndex === index ? (
                <div className="array-edit">
                  {isArrayOfObjects ? (
                    <textarea
                      value={objectEditValue}
                      onChange={(e) => setObjectEditValue(e.target.value)}
                      className="inputField json-input"
                      rows={5}
                    />
                  ) : (
                    <input
                      type="text"
                      value={arrayEditValue}
                      onChange={(e) => setArrayEditValue(e.target.value)}
                      className="inputField"
                    />
                  )}
                  <div className="array-edit-buttons">
                    <button 
                      onClick={() => isArrayOfObjects ? saveArrayObjectEdit(fieldName) : saveArrayEdit(fieldName)}
                      className="toggleButton small"
                    >
                      Save
                    </button>
                    <button
                      onClick={() => {
                        setEditingArrayField(null);
                        setEditingArrayIndex(null);
                      }}
                      className="toggleButton small"
                    >
                      Cancel
                    </button>
                  </div>
                </div>
              ) : (
                <div className="array-item-content">
                  <span>
                    {isArrayOfObjects ? (
                      <pre>{JSON.stringify(item, null, 2)}</pre>
                    ) : (
                      typeof item === 'object' ? JSON.stringify(item) : String(item)
                    )}
                  </span>
                  <div className="array-item-actions">
                    <button
                      onClick={() => {
                        setEditingArrayField(fieldName);
                        setEditingArrayIndex(index);
                        if (isArrayOfObjects) {
                          setObjectEditValue(JSON.stringify(item, null, 2));
                        } else {
                          setArrayEditValue(item);
                        }
                      }}
                      className="toggleButton small"
                    >
                      Edit
                    </button>
                    <button
                      onClick={() => handleArrayItemRemove(fieldName, index)}
                      className="toggleButton small"
                    >
                      ×
                    </button>
                  </div>
                </div>
              )}
            </div>
          ))}
        </div>
        <button
          onClick={() => handleArrayItemAdd(fieldName, isArrayOfObjects)}
          className="toggleButton small"
        >
          + Add {isArrayOfObjects ? 'Object' : 'Item'}
        </button>
      </div>
    );
  };

  const existingFields = Object.keys(rowData).filter(key => key !== '_id');

  return (
    <div className="tableContainer">
      <h2 className="tableTitle">Update Document in {selectedCollection}</h2>
      {error && !error.includes('JSON') && <div className="errorMessage">{error}</div>}

      {existingFields.length > 0 && (
        <div className="sectionDivider">
          <h3>Document Fields</h3>
          <div>
            {existingFields.map((field) => (
              <div key={field} className="field-row">
                <span className="field-label">{field}</span>

                <select
                  value={fieldTypes[field] || 'string'}
                  onChange={(e) => changeFieldType(field, e.target.value)}
                  className="inputField"
                  disabled={isSaving}
                >
                  <option value="string">String</option>
                  <option value="number">Number</option>
                  <option value="boolean">Boolean</option>
                  <option value="date">Date</option>
                  <option value="array">Array</option>
                  <option value="arrayOfObjects">Array of Objects</option>
                  <option value="object">Object</option>
                </select>

                {fieldTypes[field] === 'date' ? (
                  <input
                    type="date"
                    value={rowData[field]?.split('T')[0] || ''}
                    onChange={(e) => handleInputChange(e, field, 'date')}
                    className="inputField"
                    disabled={isSaving}
                  />
                ) : fieldTypes[field] === 'boolean' ? (
                  <select
                    value={rowData[field] ?? ''}
                    onChange={(e) => handleInputChange(e, field, 'boolean')}
                    className="inputField"
                    disabled={isSaving}
                  >
                    <option value="">Select a value</option>
                    <option value="true">True</option>
                    <option value="false">False</option>
                  </select>
                ) : fieldTypes[field] === 'array' || fieldTypes[field] === 'arrayOfObjects' ? (
                  renderArrayField(field, rowData[field] || [], fieldTypes[field])
                ) : fieldTypes[field] === 'object' ? (
                  renderObjectField(field, rowData[field] || {})
                ) : (
                  <input
                    type={fieldTypes[field] === 'number' ? 'number' : 'text'}
                    placeholder={`Enter ${field}`}
                    value={rowData[field] || ''}
                    onChange={(e) => handleInputChange(e, field, fieldTypes[field])}
                    className="inputField"
                    disabled={isSaving}
                  />
                )}

                <button
                  onClick={() => removeField(field)}
                  className="toggleButton removeField"
                  title="Remove field"
                  disabled={isSaving}
                >
                  ×
                </button>
              </div>
            ))}
          </div>
        </div>
      )}

      <div className="sectionDivider">
        <h3>Add New Fields</h3>
        <div className="addColumnForm">
          <input
            type="text"
            placeholder="New field name"
            value={newColumnName}
            onChange={(e) => setNewColumnName(e.target.value)}
            className="inputField"
            disabled={isSaving}
          />
          <select
            value={newColumnType}
            onChange={(e) => setNewColumnType(e.target.value)}
            className="inputField"
            disabled={isSaving}
          >
            <option value="string">String</option>
            <option value="number">Number</option>
            <option value="boolean">Boolean</option>
            <option value="date">Date</option>
            <option value="array">Array</option>
            <option value="arrayOfObjects">Array of Objects</option>
            <option value="object">Object</option>
          </select>
          <button
            onClick={addCustomColumn}
            className="actionButton small"
            disabled={isSaving}
          >
            Add Field
          </button>
        </div>
      </div>

      <div className="buttonGroup">
        <button
          onClick={handleSave}
          className="actionButton primary"
          disabled={isSaving}
        >
          {isSaving ? 'Saving...' : 'Save Changes'}
        </button>
        <button
          onClick={onCancel}
          className="actionButton secondary"
          disabled={isSaving}
        >
          Cancel
        </button>
      </div>
    </div>
  );
};

export default UpdateDocument;