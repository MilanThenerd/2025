const request = require('supertest');
const {api, server} = require('../mpdb-api');

//let daemon;
//const daemon = new mpdbdaemon();
/*beforeAll( () => {
    daemon = spawn('node',['daemon/daemon.js'], {
        detached: true,
        windowsHide: true,
        stdio: ['ignore'] 
      });
}); */

afterAll(async() => {
    server.close();
    //daemon.stop();
    /*if (daemon && daemon.pid) {
        try {
          if (process.platform === 'win32') {
            exec(`taskkill /pid ${daemon.pid} /f /t`);
          } else {
            process.kill(-daemon.pid, 'SIGKILL');
          }
        } catch (err) {
          if (err.code !== 'ESRCH') throw err;
        }
    }*/
  });

describe ('DATABASE CRUD TESTS:', () => {
    
    //test CREATE DATABASE
    describe('Testing CREATE DATABASE', () => {
        test('Create Database (No data sent)', async () => {
            const res = await request(api).post('/databases').send({});
            expect(res.status).toBe(400);
            expect(res.body.message).toBe('Database data required');
        });
        test('Create Database (valid)', async () => {
            const data = {"database": "database4"};
            const res = await request(api).post('/databases').send(data);
            expect(res.status).toBe(200);
            expect(res.body.message).toBe('Successfully created 1 databases, 0 collections, and 0 documents');
        });
    });

    //test UPDATE DATABASE
    describe('Testing UPDATE DATABASE', () => {
        test('Update Database (No data sent)', async () => {
            const databaseId = 'database4';
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
            const database = {newDatabase:'databaseNew'};
            const databaseId = 'database4';
            const res = await request(api).put(`/databases/${databaseId}`).send(database);
            expect(res.status).toBe(200);
            expect(res.body.message).toBe('Successfully updated 1 records');
            expect(res.body.data).toStrictEqual({'database4#reference': 'databaseNew'});
        });
        test('Update Database (unsuccessful)', async () => {
            const database = {database:'MP7'};
            const databaseId = 10;
            const res = await request(api).put(`/databases/${databaseId}`).send({database});
            expect(res.status).toBe(500);
            expect(res.body.message).toBe('Database/Collection/Document not found');
        });
    });

    //Test READ DATABASE
    describe('Testing READ DATABASE', () => {
        //Test for no database ID
        test('Read Database (No Name)', async () => {
            const res = await request(api).get('/databases/');
            expect(res.status).toBe(400);
            expect(res.body.message).toBe('Database name required');
        });

        //Test for valid database read
        test('Read Database (Valid ID, database read)', async () => {
            const databaseID = 'database1';
            const res = await request(api).get(`/databases/${databaseID}`);
            expect(res.status).toBe(200);
            expect(res.body.message).toBe('Successfully read from documents: in db database1: ');
        });
    });

    describe('Testing DELETE DATABASE', () => {
        test('Delete Database (No data sent)', async () => {
            const databaseID = '';
            const res = (await request(api).delete(`/databases/${databaseID}`));
            expect(res.status).toBe(400);
            expect(res.body.message).toBe('Database ID required');
        });

        test('Delete Database (valid)', async () => {
            const databaseID = 'databaseNew';
            const res = (await request(api).delete(`/databases/${databaseID}`));
            expect(res.status).toBe(200);
            expect(res.body.message).toBe('Successfully deleted 1 databases, 0 collections, and 0 documents');
            expect(res.body.data).toStrictEqual({'databaseNew': {}});
        });
    });

});

describe ('COLLECTION CRUD TESTS:', () => {
    describe('Testing CREATE COLLECTION', () => {
        test('Create Collection (No data sent)', async () => {
            const database = 'database1';
            const res = await request(api).post(`/databases/${database}/collections/`).send();
            expect(res.status).toBe(400);
            expect(res.body.message).toBe('Collection data required');
        });
        test('Create Collection (valid)', async () => {
            const data2 = {"database": "databaseTest"};
            const res2 = await request(api).post('/databases').send(data2);
            expect(res2.status).toBe(200); // Added to use res2 var 
            const database = 'databaseTest';
            const data = {'collection': "collection1"};
            const res = await request(api).post(`/databases/${database}/collections/`).send(data);
            expect(res.status).toBe(200);
            expect(res.body.message).toBe('Successfully created 0 databases, 1 collections, and 0 documents');
        });
    });

    describe('Testing READ COLLECTION', () => {
        test('Get Collection (No data sent)', async () => {
            const database = 'database1';
            const name='';
            const res = await request(api).get(`/databases/${database}/collections/${name}`);
            expect(res.status).toBe(400);
            expect(res.body.message).toBe('Collection name or DatabaseID required required');
        });
        test('Get Collection (valid)', async () => {
            const database = 'database1';
            const name='collection1';
            const res = await request(api).get(`/databases/${database}/collections/${name}`);
            expect(res.status).toBe(200);
            expect(res.body.message).toBe('Successfully read from documents: in db database1: \'\' in collection1');
            expect(res.body.data).toStrictEqual({'database1': {'collection1': {}}});
        });
        /*test('Get Collection (unsuccessful)', async () => {
            const database = 'MP 7';
            const name='Team';
            const res = await request(api).get(`/databases/${database}/collections/${name}`);
            expect(res.status).toBe(200);
            expect(res.body.data).toStrictEqual({'MP 7': {'Team': {}}});
            expect(res.body.message).toBe('Database/Collection/Document not found');
        });*/
    });

    test('Get All Collections (valid)', async () => {
        const databaseID = 'database';
        const res = await request(api).get(`/databases/${databaseID}/AllCollections`);
        expect(res.status).toBe(200);
        expect(res.body.message).toBe('Successfully read from documents: in db database: \'\' in All');
        //expect(res.body.data).toStrictEqual('All collections');
    });
    
    describe('Testing DELETE COLLECTION', () => {
        test('Delete Collection (No data sent)', async () => {
            const databaseID = '';
            const res = (await request(api).delete(`/databases/${databaseID}/collections/`));
            expect(res.status).toBe(404);
        });
        test('Delete Collection (valid)', async () => {
            const database = 'database1';
            const name='collection2';
            const res = await request(api).delete(`/databases/${database}/collections/${name}`);
            expect(res.status).toBe(200);
            expect(res.body.message).toBe('Successfully deleted 0 databases, 1 collections, and 0 documents');
            expect(res.body.data).toStrictEqual({'database1': {'collection2': {}}});
        });
        test('Delete Database (unsuccessful)', async () => {
            const database = 'databa';
            const name='collect';
            const res = await request(api).delete(`/databases/${database}/collections/${name}`);
            expect(res.status).toBe(500);
            expect(res.body.message).toBe('Command execution failed');
        });
    });

    describe('Testing UPDATE COLLECTION', () => {
        test('Update Collection (No data sent)', async () => {
            const databaseID = 'database';
            const res = (await request(api).put(`/databases/${databaseID}/collections/`).send({}));
            expect(res.status).toBe(400);
            expect(res.body.message).toBe('Collection name and Database ID required');
        });
        test('Update Collection (valid)', async () => {
            const database = 'database1';
            const name='collection1';
            const updatedCollection = 'NewDB'; //Define var to be used below
            const collection = { updatedCollection : updatedCollection};
            const res = (await request(api).put(`/databases/${database}/collections/${name}`).send({updatedCollection}));
            expect(res.status).toBe(200);
            expect(res.body.message).toBe('Successfully updated 1 records');
            expect(res.body.data).toStrictEqual(collection);
        });
    });
    //server.close();
});


describe ('DOCUMENT CRUD TESTS:', () => {
    describe('Testing CREATE DOCUMENT', () => {
        test('Create Document (No data sent)', async () => {
            const database = 'database9';
            const collection='collection1';
            const res = await request(api).post(`/databases/${database}/collections/${collection}/documents/`).send({});
            expect(res.status).toBe(400);
            expect(res.body.message).toBe('Collection name and Database ID and data required');
        });
        test('Create Document (valid)', async () => {
            const database = 'database9';
            const data = { "document" : {
                                        "field1":"a"
                                        }
            }
            const collection='collection1';
            const res = await request(api).post(`/databases/${database}/collections/${collection}/documents/`).send(data);
            expect(res.status).toBe(200);
            expect(res.body.message).toBe('Successfully created 0 databases, 0 collections, and 1 documents');
            expect(res.body.data).toStrictEqual({'database9': 
                {'collection1': 
                    {'johnDoe': { name: 'John Doe', email: 'john.doe@example.com', id:'10210310'}}, 
                }
            });
        
        });
    });

    describe('Testing READ DOCUMENT', () => {
        test('Get Document (No data sent)', async () => {
            const database = 'database9';
            const collection='collection1';
            const id = '';
            const res = await request(api).get(`/databases/${database}/collections/${collection}/documents/${id}`);
            expect(res.status).toBe(404);
        });
        test('Get Document (valid)', async () => {
            const database = 'database9';
            const collection='collection1';
            const id = '10210310$undefined';
            const res = await request(api).get(`/databases/${database}/collections/${collection}/documents/${id}`);
            expect(res.status).toBe(200);
        });
    });
    
    describe('Testing DELETE DOCUMENT', () => {
        test('Delete Document (No data sent)', async () => {
            const database = 'database9';
            const collection='collection1';
            const id = '';
            const res = await request(api).delete(`/databases/${database}/collections/${collection}/documents/${id}`);
            expect(res.status).toBe(404);
        });
        test('Delete Document (valid)', async () => {
            const database = 'database9';
            const collection='collection1';
            const id = '10210310$undefined';//'johnDoe';
            const res = await request(api).delete(`/databases/${database}/collections/${collection}/documents/${id}`);
            expect(res.body.data).toStrictEqual({'database9':
                                                    {'collection1':
                                                        {'10210310$undefined': {}}
                                                    }
            });
            expect(res.status).toBe(200);
            expect(res.body.message).toBe('Successfully deleted 0 databases, 0 collections, and 1 documents');
        });
        test('Delete Database (unsuccessful)', async () => {
            const database = 'database9';
            const collection='collection1';
            const id = 'bob';
            const res = await request(api).delete(`/databases/${database}/collections/${collection}/documents/${id}`);
            expect(res.status).toBe(500);
            expect(res.body.message).toBe('Command execution failed');
        });
    });

    describe('Testing UPDATE DOCUMENT', () => {
        test('Update Document (No data sent)', async () => {
            const database = 'database9';
            const collection='collection1';
            const id = '10210311$undefined';
            const res = (await request(api).put(`/databases/${database}/collections/${collection}/documents/${id}`).send({}));
            expect(res.status).toBe(400);
            expect(res.body.message).toBe('Collection name, Database ID,document id and data required');
        });
        test('Update Collection (valid)', async () => {
            const database = 'database9';
            const collection='collection1';
            const id = '10210311$undefined';
            const updatedDoc = 'NewDB';
            const doc = {[database]:{[collection]:{[`${id}#reference`]:updatedDoc}},};
            const res = (await request(api).put(`/databases/${database}/collections/${collection}/documents/${id}`).send({updatedDoc}));
            expect(res.status).toBe(200);
            expect(res.body.message).toBe('Successfully updated 1 records');
            expect(res.body.data).toStrictEqual(doc);
        });
    });
    server.close();
});



describe('USER CRUD TESTS:', ()=> {
    describe ('Create user database', () => {
        test('Create UserDatabase', async () => {
            const data = {'userDB':{}};
            const res = await request(api).post('/databases').send(data);
            expect(res.status).toBe(200);
            expect(res.body.message).toBe('Successfully created 1 databases, 0 collections, and 0 documents');
        });
        test('Create Collection (valid)', async () => {
            const database = 'userDB';
            const data = {'userDB': {'usersCollection': {}}};
            const res = await request(api).post(`/databases/${database}/collections/`).send(data);
            expect(res.status).toBe(200);
            expect(res.body.message).toBe('Successfully created 0 databases, 1 collections, and 0 documents');
            expect(res.body.data).toStrictEqual({'userDB': {'usersCollection': {}}});
        });
    });

    //Test /register
    describe('Register tests', () => {
        test('Register user (No data sent)', async () => {
            const res = await request(api).post('/register').send({});
            expect(res.status).toBe(400);
            expect(res.body.message).toBe('User data required');
        });

        test('Register user (Valid)', async () => {
            const user = {name:'MP 7',email:'1', id:'1234567890'};
            const res = await request(api).post('/register').send(user);
            expect(res.status).toBe(200);
            expect(res.body.message).toBe('Successfully created 0 databases, 0 collections, and 1 documents');
        });
    });

    describe('Login function tests', () => {
        //Test for no user ID
        test('Login user (No ID)', async () => {
            const res = await request(api).get('/login/');
            expect(res.status).toBe(400);
            expect(res.body.message).toBe('User ID required.');
        });
    
        //Test for valid user retrieval
        test('Login user (Valid ID, user found)', async () => {
            const userId = '1234567890$undefined';
            const res = await request(api).get(`/login/${userId}`);
            expect(res.status).toBe(200);
            expect(res.body.message).toBe('Successfully read from documents: in db userDB: \'1234567890$undefined\' in usersCollection');
        });
    
        //Test for valid ID but no user found
        test('Login user (Valid ID, no user)', async () => {
            const userId = '478';
            const res = await request(api).get(`/login/${userId}`);
            expect(res.status).toBe(404);
            expect(res.body.message).toBe('Document does not exist');
        });
    });

    describe('Testing UPDATE USER', () => {
        test('Update User (No User ID or Data)', async () => {
            const res = await request(api).put('/users/').send({});
            expect(res.status).toBe(400);
            expect(res.body.message).toBe('User ID and user data required');
        });

        /*test('Update User (Valid)', async () => {
            const userID = '123';
            const user = {id: userID, name: 'Updated User', email: 'updated@example.com' };
            const res = await request(api).put(`/users/${userID}`).send({ user: user });
            expect(res.status).toBe(200);
            expect(res.body.message).toBe('User updated successfully');
            expect(mockDaemon.emit).toHaveBeenCalledWith('addCommand', 'update', { user });
        });*/
    });

    describe('Testing DELETE USER', () => {
        test('Delete User (No User ID)', async () => {
            const res = await request(api).delete('/users/');
            expect(res.status).toBe(400);
            expect(res.body.message).toBe('User ID required');
        });
    
        test('Delete User (Valid)', async () => {
            const userID = '1234567890$undefined';
            const res = await request(api).delete(`/users/${userID}`);
            expect(res.status).toBe(200);
            expect(res.body.message).toBe('Successfully deleted 0 databases, 0 collections, and 1 documents');
        });
    });
});