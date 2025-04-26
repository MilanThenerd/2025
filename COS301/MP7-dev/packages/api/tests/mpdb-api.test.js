const request = require('supertest');
const { api, server } = require('../src/mpdb-api');
const io = require('socket.io-client');

jest.mock('socket.io-client', () => {
    const once = jest.fn();
    const emit = jest.fn();
    const mockDaemon = {
        once,
        emit
    };
    return jest.fn(() => mockDaemon);
});
let mockDaemon;

beforeAll(() => {
    mockDaemon = io();
});

afterAll(() => {
    server.close();
});

describe('USER CRUD TESTS:', () => {
    //Test /register
    describe('Register tests', () => {
        test('Register user (No data sent)', async () => {
            const res = await request(api).post('/register').send({});
            expect(res.status).toBe(400);
            expect(res.body.message).toBe('User data required');
        });

        test('Register user (Valid)', async () => {
            const user = { userName: 'MP 7', email: '1@test.com', password: '10192012' };
            mockDaemon.once.mockImplementation((event, callback) => {
                if (event === 'response') {
                    callback(
                        {
                            message: 'Daemon interaction successful',
                            response: 200
                        });
                }
            });
            //mockDaemon.emit.mockReturnValueOnce();
            const res = await request(api).post('/register').send(user);
            expect(res.status).toBe(200);
            expect(res.body.message).toBe('Daemon interaction successful');
        });

        test('Register user (Daemon interaction unsuccessful)', async () => {
            const user = { userName: 'MP 7', email: '1@test.com', password: '10192012' };
            mockDaemon.once.mockImplementation((event, callback) => {
                if (event === 'response') {
                    callback(
                        {
                            message: 'Daemon interaction unsuccessful :(',
                            response: 500
                        });
                }
            });
            //mockDaemon.emit.mockReturnValueOnce();
            const res = await request(api).post('/register').send(user);
            expect(res.status).toBe(500);
            expect(res.body.message).toBe('Daemon interaction unsuccessful :(');
        });
    });
    describe('Login function tests', () => {
        //Test for no user ID
        test('Login user (No data sent)', async () => {
            const res = await request(api).post('/login').send({});
            expect(res.status).toBe(400);
            expect(res.body.message).toBe('User data required');
        });

        //Test for valid user retrieval
        test('Login user (Valid data, user found)', async () => {
            const user = { userName: 'MP 7', password: '10192012' };

            mockDaemon.once.mockImplementation((event, callback) => {
                if (event === 'response') {
                    callback({
                        message: 'Daemon interaction successful, user found',
                        response: 200
                    });
                }
            });

            const res = await request(api).post('/login').send(user);
            expect(res.status).toBe(200);
            expect(res.body.message).toBe('Daemon interaction successful, user found');
        });

        //Test for valid ID but no user found
        test('Login user (Valid ID, no user)', async () => {
            const user = { userName: 'MP 7', password: '10192012' };

            mockDaemon.once.mockImplementation((event, callback) => {
                if (event === 'response') {
                    callback(
                        {
                            message: 'User not found',
                            response: 404
                        });
                }
            });

            const res = await request(api).post(`/login`).send(user);
            expect(res.status).toBe(404);
            expect(res.body.message).toBe('User not found');
        });

        //Test for daemon interaction error
        test('Login user (Server/Daemon error)', async () => {
            const user = { userName: 'MP 7', password: '10192012' };

            mockDaemon.emit.mockImplementation(() => {
                throw new Error('Daemon connection failed');
            });

            const res = await request(api).post('/login').send(user);
            expect(res.status).toBe(500);
            expect(res.body.message).toBe('Daemon interaction unsuccessful during user retrieval');
        });
    });

});

describe('DATABASE CRUD TESTS:', () => {

    //test CREATE DATABASE
    describe('Testing CREATE DATABASE', () => {
        test('Create Database (No data sent)', async () => {
            //mockDaemon.emit.mockImplementation(() => {});
            const res = await request(api).post('/databases').send({});
            expect(res.status).toBe(400);
            expect(res.body.message).toBe('Database data required');
        });
        test('Create Database (valid)', async () => {
            mockDaemon.emit.mockImplementation(() => { });
            const data = { 'database': 'testDB' };
            mockDaemon.once.mockImplementation((event, callback) => {
                if (event === 'response') {
                    callback(
                        {
                            message: 'Daemon interaction successful, database created',
                            response: 200,
                            data: data
                        });
                }
            });
            const res = await request(api).post('/databases').send(data);
            expect(res.status).toBe(200);
            expect(res.body.message).toBe('Daemon interaction successful, database created');
            //expect(res.body.data).toStrictEqual({'database':{}});
        });
        test('Create Database (unsuccessful)', async () => {
            const data = { id: {} };
            mockDaemon.once.mockImplementation((event, callback) => {
                if (event === 'response') {
                    callback(
                        {
                            message: 'Daemon interaction successful,however Invalid database',
                            response: 400,
                        });
                }
            });
            const res = await request(api).post('/databases').send({ database: data });
            expect(res.status).toBe(400);
            expect(res.body.message).toBe('Daemon interaction successful,however Invalid database');
        });
    });

    //test UPDATE DATABASE
    describe('Testing UPDATE DATABASE', () => {
        test('Update Database (No data sent)', async () => {
            const databaseId = 10;
            const res = await request(api).put(`/databases/${databaseId}`).send({});
            expect(res.status).toBe(400);
            expect(res.body.message).toBe('Database data required');
        });
        test('Update Database (No ID)', async () => {
            const databaseId = '';
            const res = await request(api).put(`/databases/${databaseId}`).send({});
            expect(res.status).toBe(400);
            expect(res.body.message).toBe('Database data required');
        });
        test('Update Database (valid)', async () => {
            const database = { database: 'MP7' };
            const databaseId = 10;
            mockDaemon.once.mockImplementation((event, callback) => {
                if (event === 'response') {
                    callback(
                        {
                            message: 'Daemon interaction successful, database updated',
                            response: 200,
                            data: { [`${databaseId}#reference`]: database }
                        });
                }
            });
            const res = await request(api).put(`/databases/${databaseId}`).send({ newDatabase: database });
            expect(res.status).toBe(200);
            expect(res.body.message).toBe('Daemon interaction successful, database updated');
            expect(res.body.data).toStrictEqual({ [`${databaseId}#reference`]: database });
        });
        test('Update Database (unsuccessful)', async () => {
            const database = { name: 'MP7' };
            const databaseId = 10;
            mockDaemon.once.mockImplementation((event, callback) => {
                if (event === 'response') {
                    callback(
                        {
                            message: 'Daemon interaction successful,however Invalid database',
                            response: 400
                        });
                }
            });
            const res = await request(api).put(`/databases/${databaseId}`).send({ newDatabase: database });
            expect(res.status).toBe(400);
            expect(res.body.message).toBe('Daemon interaction successful,however Invalid database');
        });
    });


    //Test READ DATABASE
    describe('Testing READ DATABASE', () => {
        //Test for no database ID
        test('Read Database (No ID)', async () => {
            const res = await request(api).get('/databases/');
            expect(res.status).toBe(400);
            expect(res.body.message).toBe('Database ID required');
        });

        //Test for valid database read
        test('Read Database (Valid ID, database read)', async () => {
            const databaseID = 'db1';
            mockDaemon.once.mockImplementation((event, callback) => {
                if (event === 'response') {
                    callback(
                        {
                            message: 'Daemon interaction successful, database read',
                            response: 200,
                            results: { [databaseID]: {} }
                        });
                }
            });
            const res = await request(api).get(`/databases/${databaseID}`);
            expect(res.status).toBe(200);
            expect(res.body.message).toBe('Daemon interaction successful, database read');
            expect(res.body.results).toStrictEqual({ [databaseID]: {} });
        });

        test('Read Database (Invalid ID, Database Not Found)', async () => {
            const databaseID = '999';

            mockDaemon.once.mockImplementation((event, callback) => {
                if (event === 'response') {
                    callback({
                        message: 'Database not found',
                        response: 404
                    });
                }
            });

            const res = await request(api).get(`/databases/${databaseID}`);
            expect(res.status).toBe(404);
            expect(res.body.message).toBe('Database not found');
        });

        test('Read Database (Daemon Interaction Error)', async () => {
            const databaseID = '428';

            mockDaemon.emit.mockImplementationOnce(() => {
                throw new Error('Daemon connection failed');
            });

            const res = await request(api).get(`/databases/${databaseID}`);
            expect(res.status).toBe(500);
            expect(res.body.message).toBe('Daemon interaction unsuccessful in get');
        });
    });
    //Test READ DATABASE
    describe('Testing GET ALL DATABASE', () => {
        //Test for no database ID
        test('Read All Databases (invalid)', async () => {
            const databaseID = 'testDatabase';
            mockDaemon.once.mockImplementation((event, callback) => {
                if (event === 'response') {
                    callback(
                        {
                            message: 'Daemon interaction successful, databases listed',
                            response: 200,
                            results: { [databaseID]: {} }
                        });
                }
            });
            const res = await request(api).get('/allDatabases');
            expect(res.status).toBe(500);
        });
        test('Read All Databases (valid)', async () => {
            //mockDaemon.emit.mockImplementation(() => { });
            mockDaemon.once.mockImplementation((event, callback) => {
                if (event === 'response') {
                    callback(
                        {
                            message: 'Daemon interaction successful, databases listed',
                            response: 200,
                            results: { 
                                db1: {}, 
                                db2: {} 
                            }
                        });
                }
            });
            const res = await request(api).get('/allDatabases');
            expect(res.status).toBe(200);
        });
    });
    //Test READ DATABASE
    describe('Testing DELETE DATABASE', () => {
        //Test for no database ID
        test('Delete Database (No ID)', async () => {
            const res = await request(api).delete('/databases/');
            expect(res.status).toBe(400);
            expect(res.body.message).toBe('Database ID required');
        });

        //Test for valid database read
        test('Delete Database (Valid)', async () => {
            const databaseID = 'db1';
            mockDaemon.once.mockImplementation((event, callback) => {
                if (event === 'response') {
                    callback(
                        {
                            message: 'Daemon interaction successful, database read',
                            response: 200,
                            results: { [databaseID]: {} }
                        });
                }
            });
            const res = await request(api).delete(`/databases/${databaseID}`);
            expect(res.status).toBe(200);
            expect(res.body.message).toBe('Daemon interaction successful, database read');
        });

        test('Delete Database (Invalid ID, Database Not Found)', async () => {
            const databaseID = '999';

            mockDaemon.once.mockImplementation((event, callback) => {
                if (event === 'response') {
                    callback({
                        message: 'Database not found',
                        response: 404
                    });
                }
            });

            const res = await request(api).delete(`/databases/${databaseID}`);
            expect(res.status).toBe(404);
            expect(res.body.message).toBe('Database not found');
        });

        test('Delete Database (Daemon Interaction Error)', async () => {
            const databaseID = '428';

            mockDaemon.emit.mockImplementationOnce(() => {
                throw new Error('Daemon connection failed');
            });

            const res = await request(api).delete(`/databases/${databaseID}`);
            expect(res.status).toBe(500);
            expect(res.body.message).toBe('Daemon interaction unsuccessful');
        });
    });
});


describe('COLLECTION CRUD TESTS:', () => {
    describe('Testing CREATE COLLECTION', () => {
        test('Create Collection (No data sent)', async () => {
            const database = 'database';
            const res = await request(api).post(`/databases/${database}/collections/`).send();
            expect(res.status).toBe(400);
            expect(res.body.message).toBe('Collection data required');
        });
        test('Create Collection (valid)', async () => {
            const database = 'database';
            const collection = 'collection';
            const data = { collection };
            mockDaemon.once.mockImplementation((event, callback) => {
                if (event === 'response') {
                    callback(
                        {
                            message: 'Daemon interaction successful, collection created',
                            response: 200,
                            data: { [database]: { [collection]: {} } }
                        });
                }
            });
            const res = await request(api).post(`/databases/${database}/collections/`).send(data);
            expect(res.status).toBe(200);
            expect(res.body.message).toBe('Daemon interaction successful, collection created');
            expect(res.body.data).toBe(collection);
        });
        test('Create Database (unsuccessful)', async () => {
            const data = { collection: 'MP 7' };
            const database = 'database';
            mockDaemon.once.mockImplementation((event, callback) => {
                if (event === 'response') {
                    callback(
                        {
                            message: 'Daemon interaction successful,however Invalid database',
                            response: 400
                        });
                }
            });
            const res = await request(api).post(`/databases/${database}/collections/`).send(data);
            expect(res.status).toBe(400);
            expect(res.body.message).toBe('Daemon interaction successful,however Invalid database');
        });
    });

    describe('Testing READ COLLECTION', () => {
        test('Get Collection (No data sent)', async () => {
            const res = (await request(api).get('/databases/:databaseID/collections/'));
            expect(res.status).toBe(400);
            expect(res.body.message).toBe('Collection name or DatabaseID required');
        });
        test('Get Collection (valid)', async () => {
            const database = 'database';
            const name = 'collection';
            mockDaemon.once.mockImplementation((event, callback) => {
                if (event === 'response') {
                    callback(
                        {
                            message: 'Daemon interaction successful, collection found',
                            response: 200,
                            data: { [database]: { [name]: {} } }
                        });
                }
            });
            const res = await request(api).get(`/databases/${database}/collections/${name}`);
            expect(res.status).toBe(200);
            expect(res.body.message).toBe('Daemon interaction successful, collection found');
        });
        test('Get Database (unsuccessful)', async () => {
            //const data = { name: 'MP 7' };
            mockDaemon.once.mockImplementation((event, callback) => {
                if (event === 'response') {
                    callback(
                        {
                            message: 'Daemon interaction successful,however Invalid database',
                            response: 400
                        });
                }
            });
            const res = await request(api).get('/databases/:databaseID/collections/:name');
            expect(res.status).toBe(400);
            expect(res.body.message).toBe('Daemon interaction successful,however Invalid database');
        });
    });
    test('Get All Collections (valid)', async () => {
        const databaseID = 'database';
        const collection = { [databaseID]: {} };
        mockDaemon.once.mockImplementation((event, callback) => {
            if (event === 'response') {
                callback(
                    {
                        message: 'Daemon interaction successful, collections found',
                        response: 200,
                        data: collection
                    });
            }
        });
        const res = await request(api).get(`/databases/${databaseID}/collections/All`);
        expect(res.status).toBe(200);
        expect(res.body.message).toBe('Daemon interaction successful, collections found');
        //expect(res.body.data).toStrictEqual('All collections');
    });

    describe('Testing DELETE COLLECTION', () => {
        test('Delete Collection (No data sent)', async () => {
            const res = (await request(api).delete('/databases/testDB/collections/'));
            expect(res.status).toBe(400);
            expect(res.body.message).toBe('Collection name or regex pattern and Database ID required');
        });
        test('Delete Collection (valid)', async () => {
            const database = 'database';
            const name = 'collection';
            mockDaemon.once.mockImplementation((event, callback) => {
                if (event === 'response') {
                    callback(
                        {
                            message: 'Daemon interaction successful, collection found',
                            response: 200,
                            data: { [database]: { [name]: {} } }
                        });
                }
            });
            const res = await request(api).delete(`/databases/${database}/collections/${name}`);
            expect(res.status).toBe(200);
            expect(res.body.message).toBe('Daemon interaction successful, collection found');
        });
        test('Delete Collection (unsuccessful)', async () => {
            //const data = { name: 'MP 7' };
            mockDaemon.once.mockImplementation((event, callback) => {
                if (event === 'response') {
                    callback(
                        {
                            message: 'Daemon interaction successful,however Invalid database',
                            response: 400
                        });
                }
            });
            const res = await request(api).delete('/databases/:databaseID/collections/:name');
            expect(res.status).toBe(400);
            expect(res.body.message).toBe('Daemon interaction successful,however Invalid database');
        });
    });

    describe('Testing UPDATE COLLECTION', () => {
        test('Update Collection (No data sent)', async () => {
            const res = (await request(api).put('/databases/:databaseID/collections/'));
            expect(res.status).toBe(400);
            expect(res.body.message).toBe('Collection name and Database ID required');
        });
        test('Update Collection (valid)', async () => {
            const database = 'database1';
            const name = 'collection';
            const updatedCollection = 'NewDB';
            const collection = { [database]: { [`${name}#reference`]: updatedCollection } };
            mockDaemon.once.mockImplementation((event, callback) => {
                if (event === 'response') {
                    callback(
                        {
                            message: 'Daemon interaction successful, collection found',
                            response: 200,
                            data: collection
                        });
                }
            });
            const res = (await request(api).put(`/databases/${database}/collections/${name}`).send({ updatedCollection }));
            expect(res.status).toBe(200);
            expect(res.body.message).toBe('Daemon interaction successful, collection found');
            expect(res.body.data).toStrictEqual({ [database]: { [`${name}#reference`]: updatedCollection } });
        });
        test('Update Database (unsuccessful)', async () => {
            //const data = { name: 'MP 7' };
            mockDaemon.once.mockImplementation((event, callback) => {
                if (event === 'response') {
                    callback(
                        {
                            message: 'Daemon interaction successful,however Invalid database',
                            response: 400
                        });
                }
            });
            const res = await request(api).put('/databases/:databaseID/collections/:name');
            expect(res.status).toBe(400);
            expect(res.body.message).toBe('Collection name and Database ID required');
        });
    });
});


describe('DOCUMENT CRUD TESTS:', () => {
    describe('Testing CREATE DOCUMENT', () => {
        test('Create Document (No database ID or collection name sent)', async () => {
            const databaseID = 'database';
            const collection = '';
            const res = await request(api).post(`/databases/${databaseID}/collections/${collection}/documents`).send();
            expect(res.status).toBe(404);
        });
        test('Create Document (No data sent)', async () => {
            const databaseID = 'database1';
            const collection = 'collection1';
            mockDaemon.once.mockImplementation((event, callback) => {
                if (event === 'response') {
                    callback(
                        {
                            message: 'Daemon interaction successful,however Invalid document',
                            response: 400
                        });
                }
            });
            const res = await request(api).post(`/databases/${databaseID}/collections/${collection}/documents`).send({});
            expect(res.status).toBe(400);
            expect(res.body.message).toBe('Document data required');
        });
        test('Create Document (valid)', async () => {
            const databaseID = 'database';
            const collection = 'collection';
            const document = { field: 'value' };
            mockDaemon.once.mockImplementation((event, callback) => {
                if (event === 'response') {
                    callback(
                        {
                            message: 'Daemon interaction successful, document created',
                            response: 200,
                            data: document
                        });
                }
            });
            const res = await request(api).post(`/databases/${databaseID}/collections/${collection}/documents`).send({document});
            expect(res.status).toBe(200);
            expect(res.body.message).toBe('Daemon interaction successful, document created');
            expect(res.body.data).toStrictEqual(document);
        });
        test('Create Document (unsuccessful)', async () => {
            const databaseID = 'database';
            const collection = 'nm';
            const document = { name: 'MP 7' };
            mockDaemon.once.mockImplementation((event, callback) => {
                if (event === 'response') {
                    callback(
                        {
                            message: 'Daemon interaction successful,however Invalid database',
                            response: 400
                        });
                }
            });
            const res = await request(api).post(`/databases/${databaseID}/collections/${collection}/documents`).send({document});
            expect(res.status).toBe(400);
            expect(res.body.message).toBe('Daemon interaction successful,however Invalid database');
        });
    });

    describe('Testing READ DOCUMENT', () => {
        test('Read Document (No Document ID)', async () => {
            const databaseID = 'testDB';
            const collection = 'testCollection';
            const res = await request(api).get(`/databases/${databaseID}/collections/${collection}/documents`);
            expect(res.status).toBe(400);
            expect(res.body.message).toBe('Daemon interaction successful,however Invalid database');
        });

        test('Read Document (Valid ID, document found)', async () => {
            const databaseID = 'testDB';
            const collection = 'testCollection';
            const id = '123';
            const documentData = { field: 'value' };

            mockDaemon.once.mockImplementation((event, callback) => {
                if (event === 'response') {
                    callback({
                        message: 'Document retrieved successfully',
                        response: 200,
                        data: documentData
                    });
                }
            });

            const res = await request(api).get(`/databases/${databaseID}/collections/${collection}/documents?field=id&value=${id}&operator=eq`);
            expect(res.status).toBe(200);
            expect(res.body.message).toBe('Document retrieved successfully');
        });

        test('Read Document (Valid ID, document not found)', async () => {
            const databaseID = 'testDB';
            const collection = 'testCollection';
            const id = '123';

            mockDaemon.once.mockImplementation((event, callback) => {
                if (event === 'response') {
                    callback({
                        message: 'Document not found',
                        response: 404
                    });
                }
            });

            const res = await request(api).get(`/databases/${databaseID}/collections/${collection}/documents?rfield=id&value=${id}&operator=eq`);
            expect(res.status).toBe(404);
            expect(res.body.message).toBe('Document not found');
        });

        test('Read Document (Server/Daemon error)', async () => {
            const databaseID = 'testDB';
            const collection = 'testCollection';
            const id = '123';

            mockDaemon.emit.mockImplementation(() => {
                throw new Error('Daemon connection failed');
            });

            const res = await request(api).get(`/databases/${databaseID}/collections/${collection}/documents/${id}?read=${id}`);
            expect(res.status).toBe(404);
        });
    });
    describe('Testing UPDATE DOCUMENT', () => {
        test('Update Document (invalid)', async () => {
            const databaseID = 'testDB';
            const collection = 'testCollection';
            const res = await request(api).put(`/databases/${databaseID}/collections/${collection}/documents`);
            expect(res.status).toBe(400);
            expect(res.body.message).toBe('Collection name, Database ID,document id and data required');
        });
        test('Update document(valid)', async () => {
            mockDaemon.emit.mockImplementation(() => {});
            const databaseID = 'testDB';
            const collection = 'testCollection';
            const id = '123';
            const documentData = {updatedDoc:{ field: 'value' }};
            const doc = {
                [databaseID]:
                    {
                        [collection]:
                        //{ updatedDoc }
                        {[`${id}#reference`]:{ field: 'value' } }
                    },
                };
            mockDaemon.once.mockImplementation((event, callback) => {
                if (event === 'response') {
                    callback({
                        message: 'Document retrieved successfully',
                        response: 200,
                        data: doc
                    });
                }
            });

            const res = await request(api).put(`/databases/${databaseID}/collections/${collection}/documents/${id}`).send(documentData);
            expect(res.status).toBe(200);
        });
    });
    describe('Testing DELETE DOCUMENT', () => {
        test('Delete document(valid)', async () => {
            mockDaemon.emit.mockImplementation(() => {});
            const databaseID = 'testDB';
            const collection = 'testCollection';
            const id = '123';
            const documentData = { field: 'value' };

            mockDaemon.once.mockImplementation((event, callback) => {
                if (event === 'response') {
                    callback({
                        message: 'Document deleted successfully',
                        response: 200,
                        data: {id}
                    });
                }
            });

            const res = await request(api).delete(`/databases/${databaseID}/collections/${collection}/documents`).query({ field: 'id', value: id, operator: 'eq' });
            expect(res.status).toBe(200);
            expect(res.body.message).toBe('Document deleted successfully');
        });
    });
});


describe('IMPORT/EXPORT TESTS:', () => {
    describe('Testing IMPORT', () => {
        test('Import (no data)', async () => {
            const res = await request(api).post('/import').send();
            expect(res.status).toBe(400);
        });
        test('Import (valid)', async () => {
            mockDaemon.emit.mockImplementation(() => {});
            const tdata = { database: 'testDB', collection: 'testCollection', documents: [{ id: 1 }] };
            mockDaemon.once.mockImplementation((event, callback) => {
                if (event === 'response') {
                    callback(
                        {
                            message: 'Daemon interaction successful, database imported',
                            response: 200,
                            results: tdata
                        });
                }
            });
            const res = await request(api).post('/import').send({data:tdata});
            expect(res.status).toBe(200);
        });
    });

    describe('Testing EXPORT', () => {
            test('Export (no data)', async () => {
                const res = await request(api).get('/export');
                expect(res.status).toBe(400);
            });
            test('Export (valid)', async () => {
                //jest.clearAllMocks();
                mockDaemon.emit.mockImplementation(() => {});
                const data = {data: { database: {collection:{document:{ id:1}}}}};
                mockDaemon.once.mockImplementation((event, callback) => {
                    if (event === 'response') {
                        callback(
                            {
                                message: 'Daemon interaction successful, database exported',
                                response: 200,
                                results: data
                            });
                    }
                });
                const res = await request(api).get('/export').query({data:"db1"});
                expect(res.status).toBe(200);
            });
    });
});