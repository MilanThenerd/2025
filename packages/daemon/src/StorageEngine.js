import fs from "fs/promises";
import path from "path";
import { randomBytes } from 'crypto';

class StorageEngine {
    static dataDir = path.join(process.cwd(), "data");

    static async start() {
        try {
            await fs.mkdir(StorageEngine.dataDir, { recursive: true });
            console.log(`Data directory created at ${StorageEngine.dataDir}`);
            return true;
        } catch (err) {
            console.error("Failed to create data directory:", err);
            throw err;
        }
    }

    /**
     * Creates the specified database, collection, or document if it doesn't exist, or use the existing 
     * one if it already exists. This method follows an "upsert" pattern
     *
     * @description
     * The behavior depends on which parameters are provided:
     * - create('databaseName') will create a database
     * - create('databaseName', 'collectionName') will create a collection in the database
     * - create('databaseName', 'collectionName', 'document') will create a document in the collection
     * 
     * @param {String} databaseName The name of database to create or use
     * @param {String} collectionName The name of the collection to create or use
     * @param {Object} document The document to create
     *
     * @returns {Object} returns a response code and message
     */

    static async export(databaseName, collectionName, document) {
        console.log("Exporting: ", databaseName, collectionName, document);
        let exportPath = path.join(StorageEngine.dataDir);
    
        if (databaseName) {
            exportPath = path.join(exportPath, databaseName);
        }
    
        if (collectionName) {
            if (!databaseName) {
                throw new Error("Database name is required to export a collection");
            }
            exportPath = path.join(exportPath, collectionName);
        }
    
        if (document) {
            if (!databaseName || !collectionName) {
                throw new Error("Database and collection name are required to export a document");
            }
            exportPath = path.join(exportPath, document);
        }
    
        console.log("Export path: ", exportPath);
        const built = await StorageEngine.buildObjectFromDir(exportPath);
    
        let finalObject;
        if (document) {
            finalObject = { [databaseName]: { [collectionName]: { [document]: built } } };
        } else if (collectionName) {
            finalObject = { [databaseName]: { [collectionName]: built } };
        } else if (databaseName) {
            finalObject = { [databaseName]: built };
        } else {
            finalObject = built;
        }
    
        return finalObject;
    }
    

    static async buildObjectFromDir(dirPath) {
        const result = {};
        const items = await fs.readdir(dirPath);

        for (const item of items) {
            const fullPath = path.join(dirPath, item);
            const stats = await fs.stat(fullPath);

            if (stats.isDirectory()) {
                result[item] = await StorageEngine.buildObjectFromDir(fullPath);
            } else if (stats.isFile() && item.endsWith('.json')) {
                const key = path.basename(item, '.json');
                try {
                    const fileContent = await fs.readFile(fullPath, 'utf-8');
                    result[key] = JSON.parse(fileContent);
                } catch (err) {
                    console.error(`Error reading/parsing ${fullPath}:`, err);
                }
            }
        }

        return result;
    }

    static async create(databaseName, collectionName, document) {
        let result = { status: "success", message: "", created: [] };
        let dbPath = "";
        let collectionPath = "";
        let documentPath = "";

        try {
            if (!databaseName || typeof databaseName !== "string") {
                throw new Error("No valid database name provided");
            }

            if (collectionName && typeof collectionName !== "string") {
                throw new Error("Collection name must be a string");
            }

            if (document && typeof document !== "object") {
                throw new Error("Document must be a valid object");
            }

            if (document && !collectionName) {
                throw new Error("Database and Document provided but no Collection");
            }

            dbPath = path.join(StorageEngine.dataDir, databaseName);

            let dbExists = false;
            try {
                await fs.access(dbPath);
                dbExists = true;
            } catch (err) {
                //Database will be created
            }

            if (!dbExists) {
                await fs.mkdir(dbPath, { recursive: true });
                result.message += `Database '${databaseName}' created. `;
                result.created.push("database");
            } else {
                result.message += `Using database '${databaseName}'. `;
            }

            if (!collectionName) {
                return result;
            }

            collectionPath = path.join(dbPath, collectionName);

            let colExists = false;
            try {
                await fs.access(collectionPath);
                colExists = true;
            } catch (err) {
                //Collection will be created
            }

            if (!colExists) {
                await fs.mkdir(collectionPath, { recursive: true });
                result.message += `Collection '${collectionName}' created. `;
                result.created.push("collection");
            } else {
                result.message += `Using collection '${databaseName}'. `;
            }

            if (!document) {
                return result;
            }

            
            document._id = StorageEngine.generateId();
            
            //timestamp
            document._createdAt = new Date().toISOString();

            documentPath = path.join(collectionPath, `${document._id}.json`);
            let documentExists = false;
            try {
                await fs.access(documentPath);
                documentExists = true;
            } catch (err) {
                // Document will be created
            }

            if (documentExists) {
                throw new Error(`Document with ID ${document._id} already exists`);
            }

            await fs.writeFile(
                documentPath,
                JSON.stringify(document, null, 2),
                "utf8"
            );
            result.message += `Document with ID '${document._id}' created.`;
            result.created.push("document");

            return result;
        } catch (error) {
            console.error("Creation failed:", error);
            throw error;
        }
    }

    /**
     * Deletes a database, collection, or document based on provided parameters.
     * 
     * @description
     * The behavior depends on which parameters are provided:
     * - delete('databaseName') will delete an entire database
     * - delete('databaseName', 'collectionName') will delete a collection in the database
     * - delete('databaseName', 'collectionName', 'documentId') will delete a document in the collection
     * 
     * @param {String} databaseName - The name of the database to delete or operate within
     * @param {String} [collectionName] - The name of the collection to delete or operate within (optional)
     * @param {String} [documentId] - The ID of the document to delete (optional)
     *
     * @returns {Object} An object containing:
     * - deleted: Boolean indicating successful deletion
     * - databaseName/collectionName/documentId: The name/ID of what was deleted
     * 
     * @throws {Error} If the target doesn't exist or there's an error during deletion
     */
    static async delete(databaseName, collectionName, documentId) {
        if (!databaseName && !collectionName && !documentId) {
            throw new Error(
                "Database name, Collection name and document ID are required"
            );
        }

        // Delete database
        if (databaseName && !collectionName && !documentId) {
            try {
                const databasePath = path.join(StorageEngine.dataDir, databaseName);
                await fs.rm(databasePath, { recursive: true });
                return { deleted: true, databaseName };
            } catch (error) {
                if (error.code === "ENOENT") {
                    throw new Error(`Database not found: ${databaseName}`);
                }
                throw error;
            }
        }

        // Delete collection
        if (databaseName && collectionName && !documentId) {
            const databasePath = path.join(StorageEngine.dataDir, databaseName);
            try {
                const collectionPath = path.join(databasePath, collectionName);
                await fs.rm(collectionPath, { recursive: true });
 
                const collections = await fs.readdir(databasePath);
                if (collections.length === 0) {
                    await fs.rmdir(databasePath);
                }
                return { deleted: true, collectionName };
            } catch (error) {
                if (error.code === "ENOENT") {
                    throw new Error(`Collection not found: ${collectionName} in database ${databaseName}`);
                }
                throw error;
            }
        }

        // Delete document
        if (databaseName && collectionName && documentId) {
            try {
                const databasePath = path.join(StorageEngine.dataDir, databaseName);
                const collectionPath = path.join(databasePath, collectionName);

                const files = await fs.readdir(collectionPath);
                const matchingFile = files.find(file => file.startsWith(`${documentId}`));

                if (!matchingFile) {
                    throw new Error(`Document not found: ${documentId} in collection ${collectionName}`);
                }

                const documentPath = path.join(collectionPath, matchingFile);
                await fs.unlink(documentPath);

                const remainingFiles = await fs.readdir(collectionPath);
                if (remainingFiles.length === 0) {
                    await fs.rmdir(collectionPath);
                }
                return { deleted: true, documentId };
            } catch (error) {
                if (error.code === "ENOENT") {
                    throw new Error(
                        `Document not found: ${documentId} in collection ${collectionName}`
                    );
                }
                throw error;
            }
        } else {
            throw new Error("Invalid arguments");
        }

    }

    //Rename to fetch?
    /**
     * Reads a document from the specified database and collection.
     * 
     * @description
     * Retrieves a single document by its ID from the specified database and collection.
     * The document must exist, or an error will be thrown.
     * 
     * @param {String} databaseName - The name of the database containing the document
     * @param {String} collectionName - The name of the collection containing the document
     * @param {String} documentId - The ID of the document to read
     *
     * @returns {Object} The parsed JSON content of the document
     * 
     * @throws {Error} If the database, collection, or document doesn't exist, or if there's an error reading/parsing the document
     */
    static async read(databaseName, collectionName, documentId) {
        if (!databaseName || !collectionName || !documentId) {
            throw new Error("Database, collection name and documentID required");
        } else {
            const databasePath = path.join(StorageEngine.dataDir, databaseName);
            try {
                await fs.access(databasePath);
            } catch (error) {
                if (error.code === "ENOENT") {
                    throw new Error("Database does not exist");
                }
                throw new Error("Error accessing database: " + error.message);
            }

            const collectionPath = path.join(databasePath, collectionName);
            try {
                await fs.access(collectionPath);
            } catch (error) {
                if (error.code === "ENOENT") {
                    throw new Error("Collection does not exist");
                }
                throw new Error("Error accessing collection: " + error.message);
            }

            const documentPath = path.join(collectionPath, `${documentId}.json`);
            try {
                await fs.access(documentPath);
            } catch (error) {
                if (error.code === "ENOENT") {
                    throw new Error("Document does not exist");
                }
                throw new Error("Error accessing document: " + error.message);
            }

            const fileContent = await fs.readFile(documentPath, "utf8");
            return JSON.parse(fileContent);
        }
    }

    /**
     * Updates a database, collection, or document based on provided parameters.
     * 
     * @description
     * The behavior depends on which parameters are provided:
     * - update(newName, 'databaseName') will rename a database
     * - update(newName, 'databaseName', 'collectionName') will rename a collection
     * - update(newData, 'databaseName', 'collectionName', 'documentId') will update a document
     * 
     * When updating a document, the function merges the new data with existing data,
     * updating existing fields and adding new ones while preserving fields not included
     * in the new data. If both old and new documents have ID fields, they must match.
     * 
     * @param {String|Object} newData - For databases/collections: the new name string.
     *                                  For documents: the new data object to merge with existing data.
     * @param {String} databaseName - The name of the database to update or operate within
     * @param {String} [collectionName] - The name of the collection to update or operate within (optional)
     * @param {String} [documentId] - The ID of the document to update (optional)
     *
     * @returns {Object} For documents: returns the updated document data (merged result)
     *                   For database/collection renames: returns an object with renamed:true and from/to details
     * 
     * @throws {Error} If:
     * - newData is missing
     * - The database name is missing
     * - The database, collection, or document doesn't exist
     * - There's an error during the rename operation
     * - There's an error parsing the existing document
     * - The document IDs don't match (if both have ID fields)
     */
    static async update(newData, databaseName, collectionName, documentId) {
        if (!newData) {
            throw new Error('New Data missing');
        }

        if (!databaseName) {
            throw new Error('Database name required');
        }

        const databasePath = path.join(StorageEngine.dataDir, databaseName);

        if (!collectionName) {
            try {
                await fs.access(databasePath);
            } catch (error) {
                if (error.code === 'ENOENT') {
                    throw new Error('Database does not exist');
                }
                throw new Error('Error accessing database: ' + error.message);
            }
            const newPath = path.join(StorageEngine.dataDir, newData)
            await fs.rename(databasePath, newPath);
            return { renamed: true, from: databaseName, to: newData };

        } else if (collectionName && !documentId) {
            const collectionPath = path.join(databasePath, collectionName);

            try {
                await fs.access(collectionPath);
            } catch (error) {
                if (error.code === 'ENOENT') {
                    throw new Error('Collection does not exist');
                }
                throw new Error('Error accessing collection: ' + error.message);
            }
            const newPath = path.join(databasePath, newData)
            await fs.rename(collectionPath, newPath);
            return { renamed: true, from: collectionName, to: newData };

        } else if (collectionName && documentId) {
            const collectionPath = path.join(databasePath, collectionName);
            const documentPath = path.join(collectionPath, `${documentId}.json`);

            try {
                await fs.access(documentPath);
            } catch (error) {
                if (error.code === 'ENOENT') {
                    throw new Error('Document does not exist');
                }
                throw new Error('Error accessing document: ' + error.message);
            }
            const existingDataRaw = await fs.readFile(documentPath, 'utf8');
            let existingData;

            try {
                existingData = JSON.parse(existingDataRaw);
            } catch (error) {
                throw new Error('Error parsing existing document: ' + error.message);
            }

            if (existingData._id !== undefined && newData._id !== undefined && existingData._id !== newData._id) {
                throw new Error('Object ID mismatch: cannot update document with different ID');
            }

            const updatedData = { ...existingData, ...newData };

            await fs.writeFile(documentPath, JSON.stringify(updatedData, null, 2), 'utf8');

            return updatedData;
        } else {
            throw new Error('Collection name and document ID required');
        }
    }


    /**
     * Lists all databases, collections, or documents based on provided parameters.
     * 
     * @description
     * The behavior depends on which parameters are provided:
     * - list() with no parameters will list all databases
     * - list('databaseName') will list all collections in the database
     * - list('databaseName', 'collectionName') will list all documents in the collection
     * 
     * @param {String} [databaseName] - The name of database to list from (optional)
     * @param {String} [collectionName] - The name of the collection to list from (optional)
     *
     * @returns {Array} 
     * - When listing databases: Array of database names as strings
     * - When listing collections: Array of collection names as strings
     * - When listing documents: Array of document objects with parsed JSON content
     * 
     * @throws {Error} If there's an error accessing the filesystem or parsing document content
     */
    static async list(databaseName, collectionName) {
        if (!databaseName && !collectionName) {
            try {
                try {
                    console.log("Data dir:", StorageEngine.dataDir);
                    await fs.access(StorageEngine.dataDir);
                } catch (error) {
                    if (error.code === "ENOENT") {
                        return [];
                    }
                    throw error;
                }

                const items = await fs.readdir(StorageEngine.dataDir, { withFileTypes: true });

                // Return only directories
                const databases = items
                    .filter((item) => item.isDirectory())
                    .map((dir) => dir.name);

                return databases;
            } catch (error) {
                console.error(`Error listing databases:`, error);
                throw error;
            }
        } else if (!collectionName) {
            try {
                const databasePath = path.join(StorageEngine.dataDir, databaseName);

                try {
                    await fs.access(databasePath);
                } catch (error) {
                    if (error.code === "ENOENT") {
                        return [];
                    }
                    throw error;
                }

                const items = await fs.readdir(databasePath, { withFileTypes: true });

                // Return only directories
                const collections = items
                    .filter((item) => item.isDirectory())
                    .map((dir) => dir.name);

                return collections;
            } catch (error) {
                console.error(`Error listing collections:`, error);
                throw error;
            }
        } else {
            try {
                const databasePath = path.join(StorageEngine.dataDir, databaseName);
                const collectionPath = path.join(databasePath, collectionName);

                try {
                    await fs.access(collectionPath);
                } catch (error) {
                    if (error.code === "ENOENT") {
                        return [];
                    }
                    throw error;
                }

                const files = await fs.readdir(collectionPath);
                const jsonFiles = files.filter((file) => file.endsWith(".json"));

                const documents = await Promise.all(
                    jsonFiles.map(async (file) => {
                        const filePath = path.join(collectionPath, file);
                        const data = await fs.readFile(filePath, "utf8");
                        return JSON.parse(data);
                    })
                );

                return documents;
            } catch (error) {
                console.error(
                    `Error listing documents in collection ${collectionName}:`,
                    error
                );
                throw error;
            }
        }
    }

    /**
     * Searches for documents in a collection based on query criteria.
     * 
     * @description
     * This function searches through documents in a specified collection and returns 
     * those that match the provided query criteria.
     * 
     * @param {String} databaseName - The name of the database to search in
     * @param {String} collectionName - The name of the collection to search in
     * @param {Object} query - An object containing field-value pairs to match against documents
     * 
     * @returns {Array} An array of documents that match the query criteria
     * 
     * @throws {Error} If the database or collection doesn't exist, or if there's an error during the search
     */
    static async search(databaseName, collectionName, query = {}) {
        if (!databaseName || !collectionName) {
            throw new Error("Database and collection name required");
        }

        const databasePath = path.join(StorageEngine.dataDir, databaseName);
        const collectionPath = path.join(databasePath, collectionName);

        try {
            await fs.access(collectionPath);
        } catch (err) {
            if (err.code === "ENOENT") {
                throw new Error(`Collection ${collectionName} in database ${databaseName} does not exist`);
            }
            throw err;
        }

        const files = await fs.readdir(collectionPath);
        const docs = files.filter(file => file.endsWith(".json"));

        let results = await Promise.all(
            docs.map(async (file) => {
                const filepath = path.join(collectionPath, file);
                const data = await fs.readFile(filepath, 'utf8');
                return JSON.parse(data);
            })
        );

        if (Object.keys(query).length > 0) {
            results = results.filter(doc => StorageEngine.matchesQuery(doc, query));
        }

        return results;
    }

    /**
     * Helper function to determine if a document matches a query
     * 
     * @param {Object} doc - The document to check
     * @param {Object} query - The query criteria
     * @returns {Boolean} Whether the document matches the query
     */
    static matchesQuery(doc, query) {
        for (const [field, condition] of Object.entries(query)) {
            // Handle special query operators
            if (typeof condition === 'object' && condition !== null) {
                if (!StorageEngine.matchesOperators(doc, field, condition)) {
                    return false;
                }
            }
            // Simple equality match
            else if (doc[field] !== condition) {
                return false;
            }
        }
        return true;
    }

    /**
     * Helper function to handle query operators
     * 
     * @param {Object} doc - The document to check
     * @param {String} field - The field name to check
     * @param {Object} operators - Object containing operators as keys
     * @returns {Boolean} Whether the document matches the operators
     */
    static matchesOperators(doc, field, operators) {
        for (const [op, value] of Object.entries(operators)) {
            switch (op) {
                case '$eq':
                    if (doc[field] !== value) return false;
                    break;
                case '$ne':
                    if (doc[field] === value) return false;
                    break;
            }
        }
        return true;
    }

    /**
     * Determines if the document meets the specified query criteria
     * query supports ($eq, $ne) operators
     * also supports nested queries using dot notation
     * 
     * @param {Object} document - the json doc to evaluate
     * @param {Object} query - the query object with filtering conditions
     * 
     * eg : {     "name": { "$eq": "john" },
     *            "age": { "$eq": 25 },
     *            "status": { "$ne": "inactive" }
     *           }
     * 
     * @returns {Boolean} - true if the document meets the query criteria, false otherwise
     */

    static matchDocument(document, query) {

        for (const field in query) {
            const condition = query[field];
            const value = field.split('.').reduce((obj, key) => (obj ? obj[key] : undefined), document);

            if (typeof condition === 'object' && condition !== null) {

                for (const op in condition) {
                    const expected = condition[op];
                    if (op === '$eq' && value !== expected) {
                        return false;
                    }
                    if (op === '$ne' && value === expected) {
                        return false;
                    }
                }

            } else {
                if (value !== condition) {
                    return false;
                }
            }
        }
        return true;

    }
    /**
       * Reads documents from a specified database and collection, applying an optional query for filtering
       *
       * This function scans the files in the collection directory, parses each JSON document, and filters them
       * using the matchDocument() function. If no query is provided (or the query object is empty)
       * all documents in the collection are returned
       *
       * @param {String} databaseID - The identifier of the database 
       * @param {String} collectionName - The name of the collection within the database 
       * @param {Object} [query={}] - (Optional) A query object for filtering documents.
       *                              Example:
       *                              {
       *                                "name": { "$eq": "john" },
       *                                "age": { "$eq": 25 }
       *                              }
       *
       * @returns {Promise<Object>} An object containing success {Boolean}, data {Array}: An array of documents matching the query  && message {String}: A failure message when success is false
       */

    static async readDocumentsWithQuery(databaseID, collectionName, query = {}) {
        const dbPath = path.join(this.dataDir, databaseID, collectionName);
        try {
            const files = await fs.readdir(dbPath);
            const docs = [];

            for (const file of files) {

                const filePath = path.join(dbPath, file);
                const data = await fs.readFile(filePath, 'utf-8');
                const json = JSON.parse(data);

                if (StorageEngine.matchDocument(json, query)) {
                    docs.push(json);
                }
            }

            return { success: true, data: docs, message: '' };
        } catch (error) {
            return { success: false, message: `Read failed: ${error.message}` };
        }

    }

    /**
     * Lists all collections in a given database that match the provided pattern.
     *
     * @param {String} databaseName - The name of the database.
     * @param {String} pattern - Regex pattern (or literal) to match collection names.
     * @returns {Promise<Array<String>>} An array of matching collection names.
     */
    static async getMatchingCollections(databaseName, pattern) {
        const dbPath = path.join(StorageEngine.dataDir, databaseName);
        let allCollections = [];
        try {
        const items = await fs.readdir(dbPath, { withFileTypes: true });
        allCollections = items.filter(item => item.isDirectory()).map(dir => dir.name);
        } catch (error) {
        // If the database directory doesn't exist, return an empty list.
        return [];
        }
        
        const regex = StorageEngine.createRegexFromString(pattern);
        if (regex) {
        return allCollections.filter(collName => regex.test(collName));
        } else {
        // Fallback to exact matching if no valid regex.
        return allCollections.filter(collName => collName === pattern);
        }
    }
  
        /**
     * Creates a regular expression from a given pattern string with debugging logs.
     * Supports "start with" (e.g., "^jh"), "contains" (e.g., "~ta") and "ends with" (e.g., "we$") patterns.
     * If the provided pattern is invalid, returns null.
     *
     * @param {string} patternString
     * @returns {RegExp|null}
     */
    static createRegexFromString(patternString) {
        try {
        let effectivePattern = patternString;
        if (patternString.startsWith("~")) {
            effectivePattern = patternString.slice(1);
        }
        const regex = new RegExp(effectivePattern, "i");
        let regexType = "Contains regex";
        if (patternString.startsWith("^")) {
            regexType = "Start With regex";
        } else if (patternString.endsWith("$")) {
            regexType = "End With regex";
        } else if (patternString.startsWith("~")) {
            regexType = "Contains regex (marker '~')";
        }
        console.log(`Debug: ${regexType} pattern retrieved: ${regex.toString()}`);
        return regex;
        } catch (error) {
        console.error("Debug: Invalid regex pattern provided, falling back to literal matching:", patternString);
        return null;
        }
    }
     
        /**
     * Returns an array of documents (parsed JSON objects) from the given collection
     * that match the provided document pattern.
     * The matching is attempted in this order:
     * - If a document has a "name" property, match against that.
     * - Otherwise, if it has a "product" property, match against that.
     * - Otherwise, match against the document id.
     *
     * @param {String} databaseName - The name of the database.
     * @param {String} collectionName - The name of the collection.
     * @param {String} patternString - The regex pattern (or literal) to match.
     * @param {String|null} field - The document field to match against (optional).
     * @returns {Promise<Array<Object>>} An array of matching documents.
     */
    static async getMatchingDocuments(databaseName, collectionName, patternString, field = null) {
        const documents = await StorageEngine.list(databaseName, collectionName);
        const regex = StorageEngine.createRegexFromString(patternString);
        if (!regex) {
        console.warn(`Invalid regex from pattern '${patternString}', skipping match`);
        return [];
        }
        return documents.filter(doc => {
        if (!field) {
            console.warn(`Missing field for pattern '${patternString}'`);
            return false;
        }
        if (!Object.prototype.hasOwnProperty.call(doc, field)) {
            console.warn(`Field '${field}' not found in document: ${JSON.stringify(doc)}`);
            return false;
        }
        const target = doc[field];
        if (typeof target !== "string") {
            console.warn(`Field '${field}' is not a string in document: ${JSON.stringify(doc)}`);
            return false;
        }
        const result = regex.test(target);
        console.log(`Matching pattern '${patternString}' against '${target}' in field '${field}': ${result}`);
        return result;
        });
    }
        /**
     * Searches for documents in a collection using a regex-based query.
     * Expects the query object to have a single key (the regex pattern) and a "$field" key indicating
     * which document field to match.
     *
     * @param {String} databaseName - The database name.
     * @param {String} collectionName - The collection name.
     * @param {Object} query - The query object in the new-style format.
     *        Example: { "^John": {}, "$field": "username" }
     * @returns {Promise<Array<Object>>} An array of documents that match the regex criteria.
     * @throws {Error} If the "$field" property is missing.
     */
    static async searchByRegex(databaseName, collectionName, query) {
        const queryKeys = Object.keys(query);
        if (!queryKeys.includes("$field")) {
        throw new Error(`Missing "$field" in regex query.`);
        }
        // Remove the "$field" key, leaving only pattern keys.
        const patternKeys = queryKeys.filter(key => key !== "$field");
        if (patternKeys.length === 0) {
        // No regex pattern provided; fallback to a literal search.
        return StorageEngine.search(databaseName, collectionName, {});
        }
        // For simplicity, use the first pattern key.
        const patternString = patternKeys[0];
        const field = query["$field"];
        return await StorageEngine.getMatchingDocuments(databaseName, collectionName, patternString, field);
    }
     /**
     * @param {any} a
     * @param {string} op  One of ==, !=, >, <, >=, <=
     * @param {any} b
     * @returns {boolean}
     */
     static applyOperator(a, op, b) {
        switch (op) {
          case "==" : return a === b;
          case "!=" : return a !== b;
          case ">"  : return a  >  b;
          case "<"  : return a  <  b;
          case ">=" : return a >=  b;
          case "<=" : return a <=  b;
          default   : return false;
        }
      }
     /**
     * Get all database names that satisfy an == / != test.
     * @param {"=="|"!="} op
     * @param {String} value
     * @returns {Promise<String[]>}
     */
    static async filterDatabasesByOp(op, value) {
        const all = await StorageEngine.list();         // list DBs only
        return op === "==" ? all.filter(db => db === value)
                        : all.filter(db => db !== value);
    }
    
    /**
     * Get all collection names inside a DB that satisfy an == / != test.
     * @param {String} db
     * @param {"=="|"!="} op
     * @param {String} value
     * @returns {Promise<String[]>}
     */
    static async filterCollectionsByOp(db, op, value) {
        const all = await StorageEngine.list(db);       // list collections
        return op === "==" ? all.filter(c => c === value)
                        : all.filter(c => c !== value);
    }


     /**
     * Returns all documents in a collection where a field/operator comparison holds.
     * This helper supports queries like:
     *   { $field: "age", ">": 18 }
     *   { $field: "createdAt", "<=": "2021-01-01" }
     *
     * @param {string} databaseName  Name of the database to search within
     * @param {string} collectionName  Name of the collection to search within
     * @param {Object} query  An object containing exactly one comparison operator
     *                        and a "$field" key indicating which document field to compare.
     *                        Supported operators: "==", "!=", ">", "<", ">=", "<=".
     *
     * @returns {Promise<Array<Object>>}  Array of documents matching the comparison.
     *
     * @throws {Error}  If "$field" is missing or more than one operator key is supplied.
     */
    static async getDocumentsByOperator(databaseName, collectionName, query) {
        const opKeys = Object.keys(query).filter(k => k !== "$field");
        if (!query.$field || opKeys.length !== 1) {
        throw new Error("Must specify exactly one operator key plus $field");
        }
        const op = opKeys[0];
        const compValue = query[op];
        const docs = await StorageEngine.list(databaseName, collectionName);
        return docs.filter(doc => {
        const val = doc[query.$field];
        return StorageEngine.applyOperator(val, op, compValue);
        });
    }

    /**
     * Return every DB name that satisfies an equality / inequality
     * operator test.
     * @param {string} op  "==" or "!="
     * @param {string} value  database name to compare against
     * @returns {Promise<string[]>}
     */

    // static async filterDatabasesByOp(op, value) {
    //     const dbs = await StorageEngine.list();               // all DB names
    //     return dbs.filter(dbName => StorageEngine.applyOperator(dbName, op, value));
    // }
    
    /**
     * Return every collection in a DB that satisfies an = / != filter
     * @param {string} dbName
     * @param {string} op  "==" or "!="
     * @param {string} value collection name to compare against
     * @returns {Promise<string[]>}
     */
    // static async filterCollectionsByOp(dbName, op, value) {
    //     const cols = await StorageEngine.list(dbName);        // all collections
    //     return cols.filter(cName => StorageEngine.applyOperator(cName, op, value));
    // }
    /**

    /**
     * Generates a unique ID for documents that do not contain an ID
     * 
     * @returns {String} returns a unique ID
     */
    static generateId() {
        const timestamp = Math.floor(Date.now() / 1000).toString(16).padStart(8, '0');
        const machineId = randomBytes(3).toString('hex');
        const processId = Math.floor(Math.random() * 65535).toString(16).padStart(4, '0');
        const counter = Math.floor(Math.random() * 16777215).toString(16).padStart(6, '0');

        return timestamp + machineId + processId + counter;
    }
}

export default StorageEngine;
