// Import your API Wrapper class
const APIWrapper = require('../src/apiLibrary');

// Mock the fetch API
global.fetch = jest.fn();
//Unit testing
describe('API Wrapper Use Case Example', () => {
    let api;

    beforeEach(() => {
        api = new APIWrapper('https://api.example.com');
        jest.clearAllMocks();

        // Default mock response
        global.fetch.mockImplementation(() =>
            Promise.resolve({
                ok: true,
                json: () => Promise.resolve({ success: true })
            })
        );
    });

    test('Complete database workflow example', async () => {
        // 1. Create a database
        await api.createDatabase('myDatabase');

        // 2. Create a collection in the database
        await api.createCollection('myDatabase', 'users');

        // 3. Add a document to the collection
        await api.createDocument('myDatabase', 'users', 'user1');

        // 4. Update the document
        const userData = { name: 'John Doe', email: 'john@example.com' };
        await api.updateDocument('myDatabase', 'users', 'user1', userData);

        // 5. Retrieve the document
        await api.getDocument('myDatabase', 'users', 'user1',"a");

        // Verify all API calls were made with correct parameters
        expect(global.fetch).toHaveBeenCalledTimes(5);

        // You can add more specific expectations for each call if needed
        const calls = global.fetch.mock.calls;

        // Verify first call (createDatabase)
        expect(calls[0][0]).toBe('https://api.example.com/databases');
        expect(JSON.parse(calls[0][1].body)).toEqual({ database: "myDatabase" });

        // Verify second call (createCollection)
        expect(calls[1][0]).toBe('https://api.example.com/databases/myDatabase/collections');

        // Verify third call (createDocument)
        expect(calls[2][0]).toBe('https://api.example.com/databases/myDatabase/collections/users/documents');

        // Verify fourth call (updateDocument)
        expect(calls[3][0]).toBe('https://api.example.com/databases/myDatabase/collections/users/documents/user1');
        expect(JSON.parse(calls[3][1].body)).toEqual(userData);

        // Verify fifth call (getDocument)
        expect(calls[4][0]).toBe('https://api.example.com/databases/myDatabase/collections/users/documents?operator=eq&pageNumber=1&limit=10');


    });

    // test('Retrieve a user by ID', async () => {
    //   const userID = '12345';
    //   await api.getUser(userID);
    //   expect(global.fetch).toHaveBeenCalled(1);

    //   expect(global.fetch).toHaveBeenCalledWith(
    //     `https://api.example.com/users${userID}`,
    //     expect.objectContaining({
    //       method: 'GET',
    //       headers: expect.objectContaining({
    //         'Content-Type': 'application/json'
    //       })
    //     })
    //   );
    // });

    test('User workflow', async () => {
        await api.registerUser({ name: 'John Doe', email: 'john@example.com' });
        await api.loginUser({ email: 'john@example.com', password: 'password123' });
        expect(global.fetch).toHaveBeenCalledTimes(2);
    });
});