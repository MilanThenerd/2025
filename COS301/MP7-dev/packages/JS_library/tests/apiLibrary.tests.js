const APIWrapper = require('../src/apiLibrary.js');

global.fetch = jest.fn();

describe('APIWrapper', () => {
    let apiWrapper;
    const baseURL = 'https://api.example.com';

    beforeEach(() => {
        // Create a new instance before each test
        apiWrapper = new APIWrapper(baseURL);

        // Reset all mocks
        jest.clearAllMocks();

        // Setup the default mock implementation for fetch
        global.fetch.mockImplementation(() =>
            Promise.resolve({
                ok: true,
                json: () => Promise.resolve({ success: true, data: {} })
            })
        );
    });

    // Test the constructor
    test('constructor sets the baseURL correctly', () => {
        expect(apiWrapper.baseURL).toBe(baseURL);
    });

    // Test the core request method
    describe('Core request method', () => {
        test('request method sends GET request correctly', async () => {
            const endpoint = '/test-endpoint';
            await apiWrapper.request(endpoint);

            expect(global.fetch).toHaveBeenCalledWith(
                `${baseURL}${endpoint}`,
                expect.objectContaining({
                    method: 'GET',
                    headers: expect.objectContaining({
                        'Content-Type': 'application/json'
                    })
                })
            );
        });

        test('request method sends POST request with body correctly', async () => {
            const endpoint = '/test-endpoint';
            const body = { name: 'Test', value: 123 };
            await apiWrapper.request(endpoint, 'POST', body);

            expect(global.fetch).toHaveBeenCalledWith(
                `${baseURL}${endpoint}`,
                expect.objectContaining({
                    method: 'POST',
                    headers: expect.objectContaining({
                        'Content-Type': 'application/json'
                    }),
                    body: JSON.stringify(body)
                })
            );
        });

        test('request method handles API errors', async () => {
            global.fetch.mockImplementationOnce(() =>
                Promise.resolve({
                    ok: false,
                    statusText: 'Not Found'
                })
            );

            console.error = jest.fn(); // Mock console.error

            const result = await apiWrapper.request('/test-endpoint');

            expect(console.error).toHaveBeenCalled();
            expect(result).toBeNull();
        });

        test('request method handles network errors', async () => {
            global.fetch.mockImplementationOnce(() =>
                Promise.reject(new Error('Network error'))
            );

            console.error = jest.fn(); // Mock console.error

            const result = await apiWrapper.request('/test-endpoint');

            expect(console.error).toHaveBeenCalled();
            expect(result).toBeNull();
        });
    });

    // Database CRUD Operations testing
    describe('Database CRUD Operations', () => {
        // CREATE
        test('createDatabase throws error without database ID', async () => {
            await expect(apiWrapper.createDatabase()).rejects.toThrow('Database id is required');
        });

        test('createDatabase makes correct API call', async () => {
            const dbId = 'testDB';
            await apiWrapper.createDatabase(dbId);

            expect(global.fetch).toHaveBeenCalledWith(
                `${baseURL}/databases`,
                expect.objectContaining({
                    method: 'POST',
                    body: "{\"database\":\"testDB\"}"
                })
            );
        });

        // READ
        test('getDatabase throws error without database ID', async () => {
            await expect(apiWrapper.getDatabase()).rejects.toThrow('Database id is required');
        });

        test('getDatabase makes correct API call', async () => {
            const dbId = 'testDB';
            await apiWrapper.getDatabase(dbId);

            expect(global.fetch).toHaveBeenCalledWith(
                `${baseURL}/databases/${dbId}?equalityComparison=eq&pageNumber=1&limit=10`,
                expect.objectContaining({
                    method: 'GET',
                    headers: {"Content-Type": "application/json"}
                })
            );
        });

        test('getAllDatabase makes correct API call', async () => {
            await apiWrapper.getAllDatabase();

            expect(global.fetch).toHaveBeenCalledWith(
                `${baseURL}/allDatabases?pageNumber=1&limit=10`,
                expect.objectContaining({
                    method: 'GET'
                })
            );
        });

        // UPDATE
        test('updateDatabase throws error without database ID', async () => {
            await expect(apiWrapper.updateDatabase()).rejects.toThrow('Database id is required');
        });

        test('updateDatabase throws error without updated data', async () => {
            const dbId = 'testDB';
            await expect(apiWrapper.updateDatabase(dbId)).rejects.toThrow('updated database data is required');
        });

        test('updateDatabase makes correct API call', async () => {
            const dbId = 'testDB';
            const updatedData = { name: 'New Database Name' };
            await apiWrapper.updateDatabase(dbId, updatedData);

            expect(global.fetch).toHaveBeenCalledWith(
                `${baseURL}/databases/${dbId}`,
                expect.objectContaining({
                    method: 'PUT',
                    body: JSON.stringify(updatedData)
                })
            );
        });

        // DELETE
        test('deleteDatabase throws error without database ID', async () => {
            await expect(apiWrapper.deleteDatabase()).rejects.toThrow('Database id is required');
        });

        test('deleteDatabase makes correct API call', async () => {
            const dbId = 'testDB';
            await apiWrapper.deleteDatabase(dbId);

            expect(global.fetch).toHaveBeenCalledWith(
                `${baseURL}/databases/${dbId}?equalityComparison=eq`,
                expect.objectContaining({
                    method: 'DELETE'
                })
            );
        });
    });

    // Collection CRUD Operations testing
    describe('Collection CRUD Operations', () => {
        // CREATE
        test('createCollection throws error without database ID', async () => {
            await expect(apiWrapper.createCollection()).rejects.toThrow('Database name is required');
        });

        test('createCollection throws error without collection name', async () => {
            const dbId = 'testDB';
            await expect(apiWrapper.createCollection(dbId)).rejects.toThrow('Collection name is required');
        });

        test('createCollection makes correct API call', async () => {
            const dbId = 'testDB';
            const collectionName = 'testCollection';
            await apiWrapper.createCollection(dbId, collectionName);

            expect(global.fetch).toHaveBeenCalledWith(
                `${baseURL}/databases/${dbId}/collections`,
                expect.objectContaining({
                    method: 'POST',
                    body: '{\"collection\":\"testCollection\"}'   
                    })
            );
        });

        // READ
        test('getCollection throws error without database ID', async () => {
            await expect(apiWrapper.getCollection()).rejects.toThrow('Database id is required');
        });

        test('getCollection throws error without collection name', async () => {
            const dbId = 'testDB';
            await expect(apiWrapper.getCollection(dbId)).rejects.toThrow('Collection name is required');
        });

        test('getCollection makes correct API call', async () => {
            const dbId = 'testDB';
            const collectionName = 'testCollection';
            await apiWrapper.getCollection(dbId, collectionName);

            expect(global.fetch).toHaveBeenCalledWith(
                `${baseURL}/databases/${dbId}/collections/${collectionName}?equalityComparison=eq`,
                expect.objectContaining({
                    method: 'GET'
                })
            );
        });

        test('getAllCollections throws error without database ID', async () => {
            await expect(apiWrapper.getAllCollections()).rejects.toThrow('Database id is required');
        });

        test('getAllCollections makes correct API call', async () => {
            const dbId = 'testDB';
            await apiWrapper.getAllCollections(dbId);

            expect(global.fetch).toHaveBeenCalledWith(
                `${baseURL}/databases/${dbId}/allCollections?pageNumber=1&limit=10`,
                expect.objectContaining({
                    method: 'GET'
                })
            );
        });

        // UPDATE
        test('updateCollecion throws error without database ID', async () => {
            await expect(apiWrapper.updateCollection()).rejects.toThrow('Database id is required');
        });

        test('updateCollecion throws error without updated collection data', async () => {
            const dbId = 'testDB';
            await expect(apiWrapper.updateCollection(dbId)).rejects.toThrow('Updated collection data is required');
        });

        test('updateCollecion makes correct API call', async () => {
            const dbId = 'testDB';
            const collection = "col";
            const updatedCollection = { name: 'New Collection Name' };
            await apiWrapper.updateCollection(dbId, collection,updatedCollection);

            expect(global.fetch).toHaveBeenCalledWith(
                `${baseURL}/databases/${dbId}/collections/col`,
                expect.objectContaining({
                    method: 'PUT',
                    body: JSON.stringify(updatedCollection)
                })
            );
        });

        // DELETE
        test('dropCollection throws error without database ID', async () => {
            await expect(apiWrapper.dropCollection()).rejects.toThrow('Database id is required');
        });

        test('dropCollection throws error without collection name', async () => {
            const dbId = 'testDB';
            await expect(apiWrapper.dropCollection(dbId)).rejects.toThrow('Collection name is required');
        });

        test('dropCollection makes correct API call', async () => {
            const dbId = 'testDB';
            const collectionName = 'testCollection';
            await apiWrapper.dropCollection(dbId, collectionName);

            expect(global.fetch).toHaveBeenCalledWith(
                `${baseURL}/databases/${dbId}/collections/${collectionName}?equalityComparison=eq`,
                expect.objectContaining({
                    method: 'DELETE'
                })
            );
        });
    });

    // Document CRUD Operations testing
    describe('Document CRUD Operations', () => {
        // CREATE
        test('createDocument throws error without database ID', async () => {
            await expect(apiWrapper.createDocument()).rejects.toThrow('Database id is required');
        });

        test('createDocument throws error without collection name', async () => {
            const dbId = 'testDB';
            await expect(apiWrapper.createDocument(dbId)).rejects.toThrow('Collection name is required');
        });

        test('createDocument throws error without document name', async () => {
            const dbId = 'testDB';
            const collectionName = 'testCollection';
            await expect(apiWrapper.createDocument(dbId, collectionName)).rejects.toThrow('Document object is required');
        });

        test('createDocument makes correct API call', async () => {
            const dbId = 'testDB';
            const collectionName = 'testCollection';
            const documentName = 'testDoc';
            await apiWrapper.createDocument(dbId, collectionName, documentName);

            expect(global.fetch).toHaveBeenCalledWith(
                `${baseURL}/databases/${dbId}/collections/${collectionName}/documents`,
                expect.objectContaining({
                    method: 'POST',
                    body: "\"testDoc\""
                        
                    
                })
            );
        });

        // READ
        test('getDocument throws error without database ID', async () => {
            await expect(apiWrapper.getDocument()).rejects.toThrow('Database id is required');
        });

        test('getDocument throws error without collection name', async () => {
            const dbId = 'testDB';
            await expect(apiWrapper.getDocument(dbId)).rejects.toThrow('Collection name is required');
        });

        test('getDocument throws error without document name', async () => {
            const dbId = 'testDB';
            const collectionName = 'testCollection';
            await expect(apiWrapper.getDocument(dbId, collectionName)).rejects.toThrow('Document data is required');
        });

        test('getDocument makes correct API call', async () => {
            const dbId = 'testDB';
            const collectionName = 'testCollection';
            const field= "f1";
            const value = 1;
            await apiWrapper.getDocument(dbId, collectionName, field,value);

            expect(global.fetch).toHaveBeenCalledWith(
                `${baseURL}/databases/${dbId}/collections/${collectionName}/documents?operator=eq&pageNumber=1&limit=10`,
                expect.objectContaining({
                    method: 'GET'
                })
            );
        });

        test('getAllDocuments throws error without database ID', async () => {
            await expect(apiWrapper.getAllDocuments()).rejects.toThrow('Database id is required');
        });

        test('getAllDocuments throws error without collection name', async () => {
            const dbId = 'testDB';
            await expect(apiWrapper.getAllDocuments(dbId)).rejects.toThrow('Collection name is required');
        });

        test('getAllDocuments makes correct API call', async () => {
            const dbId = 'testDB';
            const collectionName = 'testCollection';
            await apiWrapper.getAllDocuments(dbId, collectionName);

            expect(global.fetch).toHaveBeenCalledWith(
                `${baseURL}/databases/${dbId}/collections/${collectionName}/allDocuments?pageNumber=1&limit=10`,
                expect.objectContaining({
                    method: 'GET'
                })
            );
        });

        // UPDATE
        test('updateDocument throws error without database ID', async () => {
            await expect(apiWrapper.updateDocument()).rejects.toThrow('Database id is required');
        });

        test('updateDocument throws error without collection name', async () => {
            const dbId = 'testDB';
            await expect(apiWrapper.updateDocument(dbId)).rejects.toThrow('Collection name is required');
        });

        test('updateDocument throws error without document name', async () => {
            const dbId = 'testDB';
            const collectionName = 'testCollection';
            await expect(apiWrapper.updateDocument(dbId, collectionName)).rejects.toThrow('Document name is required');
        });

        test('updateDocument throws error without updated data', async () => {
            const dbId = 'testDB';
            const collectionName = 'testCollection';
            const documentName = 'testDoc';
            await expect(apiWrapper.updateDocument(dbId, collectionName, documentName)).rejects.toThrow('Updated data is required');
        });

        test('updateDocument makes correct API call', async () => {
            const dbId = 'testDB';
            const collectionName = 'testCollection';
            const documentName = 'testDoc';
            const updatedDocument = { name: 'Updated Doc', content: 'New content' };
            await apiWrapper.updateDocument(dbId, collectionName, documentName, updatedDocument);

            expect(global.fetch).toHaveBeenCalledWith(
                `${baseURL}/databases/${dbId}/collections/${collectionName}/documents/${documentName}`,
                expect.objectContaining({
                    method: 'PUT',
                    body: JSON.stringify(updatedDocument)
                })
            );
        });

        // DELETE
        test('deleteDocument throws error without database ID', async () => {
            await expect(apiWrapper.deleteDocument()).rejects.toThrow('Database id is required');
        });

        test('deleteDocument throws error without collection name', async () => {
            const dbId = 'testDB';
            await expect(apiWrapper.deleteDocument(dbId)).rejects.toThrow('Collection name is required');
        });

        test('deleteDocument throws error without document name', async () => {
            const dbId = 'testDB';
            const collectionName = 'testCollection';
            await expect(apiWrapper.deleteDocument(dbId, collectionName)).rejects.toThrow('Document data is required');
        });

        test('deleteDocument makes correct API call', async () => {
            const dbId = 'testDB';
            const collectionName = 'testCollection';
            const documentName = 'testDoc';
            const field="f1";
            const value=1;
            await apiWrapper.deleteDocument(dbId, collectionName, field,value);

            expect(global.fetch).toHaveBeenCalledWith(
                `${baseURL}/databases/${dbId}/collections/${collectionName}/documents?operator=eq&field=f1&value=1`,
                expect.objectContaining({
                    method: 'DELETE'
                })
            );
        });
    });

    //request method test
    describe('request method', () => {
        test('request method sends GET request correctly', async () => {
            const endpoint = '/test-endpoint';
            await apiWrapper.request(endpoint);

            expect(global.fetch).toHaveBeenCalledWith(
                `${baseURL}${endpoint}`,
                expect.objectContaining({
                    method: 'GET',
                    headers: expect.objectContaining({
                        'Content-Type': 'application/json'
                    })
                })
            );
        });

        test('request method sends POST request with body correctly', async () => {
            const endpoint = '/test-endpoint';
            const body = { name: 'Test', value: 123 };
            await apiWrapper.request(endpoint, 'POST', body);

            expect(global.fetch).toHaveBeenCalledWith(
                `${baseURL}${endpoint}`,
                expect.objectContaining({
                    method: 'POST',
                    headers: expect.objectContaining({
                        'Content-Type': 'application/json'
                    }),
                    body: JSON.stringify(body)
                })
            );
        });

        test('request method handles API errors', async () => {
            global.fetch.mockImplementationOnce(() =>
                Promise.resolve({
                    ok: false,
                    statusText: 'Not Found'
                })
            );

            console.error = jest.fn(); // Mock console.error

            const result = await apiWrapper.request('/test-endpoint');

            expect(console.error).toHaveBeenCalled();
            expect(result).toBeNull();
        });

        test('request method handles network errors', async () => {
            global.fetch.mockImplementationOnce(() =>
                Promise.reject(new Error('Network error'))
            );

            console.error = jest.fn(); // Mock console.error

            const result = await apiWrapper.request('/test-endpoint');

            expect(console.error).toHaveBeenCalled();
            expect(result).toBeNull();
        });
    });

    //Testing userRegistration
    describe('API Wrapper Core Functions', () => {
        test('request function makes correct GET request', async () => {
            await apiWrapper.request('/users');
            expect(global.fetch).toHaveBeenCalledWith(
                `${baseURL}/users`,
                expect.objectContaining({ method: 'GET' })
            );
        });
        test('request function handles POST request', async () => {
            const userData = { name: 'Test User', email: 'test@example.com' };
            await apiWrapper.request('/register', 'POST', userData);
            expect(global.fetch).toHaveBeenCalledWith(
                `${baseURL}/register`,
                expect.objectContaining({
                    method: 'POST',
                    body: JSON.stringify(userData)
                })
            );
        });
    });
});