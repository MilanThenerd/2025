const APIWrapper = require('../src/apiLibrary');
//const {api, server} = require('../mpdb-api');

const api = new APIWrapper('http://localhost:8080');

describe('APIWrapper', () => {
    /*test('register', async () => {
        const userData = { "username": 'TestUser10', "email": 'test@example.com' ,"password":"pass123$Word"};
        const res = await api.registerUser(userData);
        console.log("register reponse from API: ",res.message);
        
    });*/

    test('createDatabase', async () => {
        const data = "newDatabase4";
        const res = await api.createDatabase(data);
        expect(res.message).toBe("Successfully created 1 databases, 0 collections, and 0 documents");
    });
    test('Read Database ==', async () => {
        const databaseID = 'newDatabase6';
        const res = await api.getDatabase(databaseID);
        expect(res.message).toBe('Matched 1 database(s).');
    });
    test('Read Database !=', async () => {
        const databaseID = 'newDatabase2';
        const ec = "ne";
        const res = await api.getDatabase(databaseID,ec);
        expect(res.message).toBe('Matched 8 database(s).');
    });
    test('Update Database', async () => {
        const database = {newDatabase:'database4'};
        const databaseId = 'newDatabase4';
        const res = await api.updateDatabase(databaseId,database);
        expect(res.message).toBe('Successfully updated 1 records');
    });
    test('Get All Databases', async () => {
        const res = await api.getAllDatabase();
        expect(res.message).toBe('Matched 8 database(s).');
    });
    test('deleteDatabase', async () => {
        const databaseId = 'database4';
        const res = await api.deleteDatabase(databaseId);
        expect(res.message).toBe('Successfully deleted 1 databases, 0 collections, and 0 documents');
    });
    test('deleteDatabase', async () => {
        const databaseId = 'database4';
        const res = await api.deleteDatabase(databaseId);
        expect(res.message).toBe('Successfully deleted 0 databases, 0 collections, and 0 documents');
    });


});
