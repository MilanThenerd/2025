const path = require('path');
const envPath = path.join(__dirname, '../server.env');
require('dotenv').config({ path: envPath });//handle .env files
const express = require('express'); //imports express function 
const app = express();  //and set app to use express
const io = require('socket.io-client'); //client side websocket used in API
app.use(express.json());
const cors = require('cors');
app.use(cors());
//The API must send data to daemon
//Api must send a response back to JS library
//Websocket connection for real-time updates?
const PORT = process.env.PORT; //process is a built
// in object allwign access to .env
const HOST = process.env.HOST;

//connect to DAEMON 
const DaemonServer = io('http://localhost:8008');


/**
 * registers users in order to allow access to NoSQL Database
 * 
 * @param {string} '/register' is the url endpoint
 * @param {Object} req The request from JS client library
 * @param {Object} res response to send back to js client library
 * 
 * @returns {Object} relevant status code with a message to indicate successful/unsuccessful interaction with Daemon
 */
app.post('/register', async (req,res) => {  
    const user = req.body; //stores body of request
    const {username,email,password} = req.body;
    // Basic validation
    if (!user || Object.keys(user).length===0 || !username || !email || !password) {
        return res.status(400).send({
            message: "User data required"
        });
    }

    // Email validation - must contain @
    if (!email.includes('@')) {
        console.log("Invalid email format. Must contain @");
        return res.status(400).send({
            message: "Invalid email format. Must contain @"
        });
    }
    
    // Password validation - at least 8 characters
    if (password.length < 8) {
        console.log("Password must be at least 8 characters long");
        return res.status(400).send({
            message: "Password must be at least 8 characters long"
        });
    }
    
    try {
        DaemonServer.emit("register", user);
        DaemonServer.once("response",(data) => {
            res.status(data.response).send({
                message: data.message
            });
        });
    } catch (error) {
        console.log("Daemon interaction unsuccessful :(");
        res.status(500).send({
            message: "Daemon interaction unsuccessful :("
        });
    }
});
/**
 * GET USER by credentials ->Login
 * Retrieves a specific user by name,email,password
 * 
 * @param {string} '/login/:id' is the url endpoint
 * @param {Object} req The request from JS client library
 * @param {Object} res response to send back to JS client library
 * 
 * @returns {Object} relevant status code with a message and user data if found
 */
app.post('/login', async (req, res) => {
    const user = req.body;
    const { username, password } = user;
    if (!user || Object.keys(user).length === 0 || !username || !password) {
        return res.status(400).send({
            message: "User data required"
        });
    }
    
    // Password validation - at least 8 characters
    if (password.length < 8) {
        return res.status(400).send({
            message: "Password must be at least 8 characters long"
        });
    }
    
    try {
        DaemonServer.emit("login", user);        

        DaemonServer.once("response", (data) => {
            res.status(data.response).send({
                message: data.message,
                data: data.id
            });
        });
    } catch (error) {
        res.status(500).send({
            message: "Daemon interaction unsuccessful during user retrieval"
        });
    }
});

/**
 * UPDATE USER
 * Updates a specific user by ID
 * 
 * @param {string} '/users/:id' is the url endpoint
 * @param {Object} req The request from JS client library
 * @param {Object} res response to send back to JS client library
 * 
 * @returns {Object} relevant status code with a message indicating successful/unsuccessful update
 */
/*app.put('/users/:id?', async (req, res) => {
    const userID = req.params;
    const { user } = req.body;

    if (!userID || !user) {
        return res.status(400).send({
            message: 'User ID and user data required'
        });
    }

    user.id = userID.id;

    try {
        DaemonServer.emit('addCommand', 'update', { user });

        DaemonServer.once('response', (data) => {
            res.status(data.response).send({
                message: data.message
            });
        });
    } catch (error) {
        res.status(500).send({
            message: 'Daemon interaction unsuccessful during user update'
        });
    }
});*/

/**
 * DELETE USER
 * Deletes a specific user by username
 * 
 * @param {string} '/users/:id' is the url endpoint
 * @param {Object} req The request from JS client library
 * @param {Object} res response to send back to JS client library
 * 
 * @returns {Object} relevant status code with a message indicating successful/unsuccessful deletion
 */
/*app.delete('/users/:username?', async (req, res) => {
    const { username } = req.params;

    if (!username) {
        return res.status(400).send({
            message: 'User ID required'
        });
    }
    const userField = { 'userDB': { 'usersCollection': { [username]: {} } } };
    try {
        DaemonServer.emit('addCommand', 'delete', userField);

        DaemonServer.once('response', (data) => {
            res.status(data.response).send({
                message: data.message,
                data: userField
            });
        });
    } catch (error) {
        res.status(500).send({
            message: 'Daemon interaction unsuccessful during user deletion'
        });
    }
});*/

//DOCUMENTS

/**
 * CREATE DOCUMENT
 * Creates a new document
 * 
 * @param {string} '/databases/:databaseID/collections/:collection/documents' is the url endpoint
 * @param {Object} req The request from JS client library
 * @param {Object} res response to send back to js client library
 * 
 * @returns {Object} relevant status code with a message to indicate successful/unsuccessful interaction with Daemon
 */
app.post('/databases/:databaseID/collections/:collection/documents', async (req, res) => {
    const {document} = req.body; //stores body of request
    const { databaseID, collection } = req.params;

    if (!document) {
        return res.status(400).send({
            message: 'Document data required'
        });
    }
    
    if (!databaseID || !collection || Object.keys(document).length === 0) {
        return res.status(400).send({
            message: 'Collection name and Database ID and data required'
        });
    }

    const newDoc = {[databaseID]:{[collection]:{document}}};
    try {
        DaemonServer.emit('addCommand', 'create', newDoc);
        //set up event listener to get response from daemon
        DaemonServer.once('response', (data) => {
            res.status(data.response).send({
                message: data.message,
                data: document
            });
        });
    } catch (error) {
        res.status(500).send({
            message: 'Daemon interaction unsuccessful :('
        });
    }
});

/**
* READ FUNCTION
* Retrieves a document
* 
* @param {string} '/databases/:databaseID/collections/:collection/documents/:id' is the URL endpoint
* @param {Object} req The request from JS client library
* @param {Object} res Response to send back to JS client library
* 
* @returns {Object} Document or error message if document not found
*/
app.get('/databases/:databaseID/collections/:collection/documents', async (req, res) => {
    const { databaseID, collection} = req.params;
    const { regex, field, operator, value, pageNumber, limit } = req.query;
    if (!databaseID || !collection) {
        return res.status(400).send({
            message: 'Database ID, and collection name required'
        });
    }

    try {
        // let command='search'; No used, exlicitly coded in emit
        let queryDocument;
        if (regex && field){
            //Regex search e.g ^Joh, ~doh, 123$
            let regexValue = regex+value;
            if (regex === "$")
            {
                regexValue = value+regex;
            }
            queryDocument = {
                data:{[databaseID]: {
                    [collection]: {
                        [regexValue]: {},
                        "$field": field
                    }
                }},

                pageNumber,
                limit
            };
        }
        else if (field && operator && value !== undefined){
            //Equality Comparison search e.g ==, >, <=
            let op = "==";
            switch (operator)
            {
                case "ne": op = "!=";
                            break;
                case "g": op = ">";
                            break;
                case "ge": op = ">="; 
                            break;
                case "l": op = "<";
                            break;
                case "le": op = "<=";
                            break;
                default:
                    op = "==";
            }
            queryDocument = {
                data:{[databaseID]: {
                        [collection]: {
                            "$field": field,
                            [op]: value
                        }
                    }},
                pageNumber,
                limit
            };
        }

        else{
            //List everything
            queryDocument = { 
                data:{[databaseID]: { 
                    [collection]: {} 
                }},

                pageNumber,
                limit
            };
        }

        DaemonServer.emit('addCommand', 'search', queryDocument);

        DaemonServer.once('response', (data) => {
            res.status(data.response).send({
                message: data.message,
                data: data.results
            });
        });
    } catch (error) {
        res.status(500).send({
            message: 'Daemon interaction unsuccessful in read'
        });
    }
});

/**
* READ ALL FUNCTION
* Retrieves a document
* 
* @param {string} '/databases/:databaseID/collections/:collection/documents/:id' is the URL endpoint
* @param {Object} req The request from JS client library
* @param {Object} res Response to send back to JS client library
* 
* @returns {Object} Document or error message if document not found
*/
app.get('/databases/:databaseID/collections/:collection/AllDocuments', async (req, res) => {
    const { databaseID, collection } = req.params;
    const { pageNumber, limit } = req.query;
    if (!databaseID || !collection) {
        return res.status(400).send({
            message: 'Document ID and collection required'
        });
    }

    const document={
    [databaseID]: {
            [collection]: {
            }
        },
    pageNumber,
    limit
    }
    try{
        DaemonServer.emit('addCommand', 'list', document);

        DaemonServer.once('response', (data) => {
            res.status(data.response).send({
                message: data.message,
                data: data.data
            });
        });
    } catch (error) {
        res.status(500).send({
            message: 'Daemon interaction unsuccessful in read'
        });
    }
});

/**
 * UPDATE FUNCTION
 * Updates an existing document
 * 
 * @param {string} '/update' is the URL endpoint
 * @param {Object} req The request from JS client library
 * @param {Object} res Response to send back to JS client library
 * 
 * @returns {Object} Success message with updated document or error message
 */
app.put('/databases/:databaseID/collections/:collection/documents/:id?', async (req, res) => {
    const { databaseID, collection ,id} = req.params;
    const { updatedDoc } = req.body;
    if (!databaseID || !collection || !updatedDoc || Object.keys(updatedDoc).length === 0) {
        return res.status(400).send({
            message: 'Collection name, Database ID,document id and data required'
        });
    }

    try {
        const doc = {
            [databaseID]:
                {
                    [collection]:
                    //{ updatedDoc }
                    {[`${id}#reference`]: updatedDoc }
                },
            };
        DaemonServer.emit('addCommand', 'update', doc);
        DaemonServer.once('response', (data) => {
            res.status(data.response).send({
                message: data.message,
                data: doc
            });
        });
    } catch (error) {
        res.status(500).send({
            message: 'Daemon interaction unsuccessful'
        });
    }
});

/**
 * DELETE FUNCTION
 * Deletes a document
 * 
 * @param {string} '/delete' is the URL endpoint
 * @param {Object} req The request from JS client library
 * @param {Object} res Response to send back to JS client library
 * 
 * @returns {Object} Success message or error message
 */
app.delete('/databases/:databaseID/collections/:collection/documents', async (req, res) => {
    const { databaseID, collection} = req.params;
    const { regex, field, operator, value } = req.query;
    
    if (!databaseID || !collection) {
        return res.status(400).send({
            message: 'Document ID,databaseID and collection name required'
        });
    }

    try {
        let document;

        if (regex && field){
            //Regex search e.g ^Joh, ~doh, 123$
            var regexValue = regex+value;
            if (regex === "$")
            {
                regexValue = value+regex;
            }
            document = {
                [databaseID]: {
                    [collection]: {
                        document:{[regexValue]: {},
                        "$field": field
                    }}
                }
            };
        }

        else if (field && operator && value !== undefined){
            //Equality Comparison search e.g ==, >, <=
            let op = "==";
            switch (operator)
            {
                case "ne": op = "!=";
                            break;
                case "g": op = ">";
                            break;
                case "ge": op = ">="; 
                            break;
                case "l": op = "<";
                            break;
                case "le": op = "<=";
                            break;
                default:
                    op = "==";
            }
            document = {
               [databaseID]: {
                        [collection]: {
                            "$field": field,
                            [op]: value
                        }
                    }
            };
        }
        
        DaemonServer.emit('addCommand', 'delete', document);
        DaemonServer.once('response', (data) => {
            res.status(data.response).send({
                message: data.message,
                data: document
            });
        });
    } catch (error) {
        res.status(500).send({
            message: 'Daemon interaction unsuccessful'
        });
    }
});



//COLLECTIONS

/**
 * CREATE COLLECTION
 * 
 * 
 * @param {string} '/databases/:databaseID/collections' is the url endpoint
 * @param {Object} req The request from JS client library
 * @param {Object} res response to send back to JS client library
 * 
 * @returns {Object} relevant status code with a message to indicate successful/unsuccessful login
 */

app.post('/databases/:databaseID/collections', async (req, res) => {
    const {databaseID} = req.params;
    const {collection} = req.body;

    if (!collection || Object.keys(collection).length === 0) {
        return res.status(400).send({
            message: 'Collection data required'
        });
    }
    if (!databaseID)
    {
        return res.status(400).send({
            message: 'Database name required'
        });
    }
    const newCollection = {[databaseID]:{[collection]:{}}};
    try {
        DaemonServer.emit('addCommand', 'create', newCollection);

        DaemonServer.once('response', (data) => {
            res.status(data.response).send({
                message: data.message,
                data: collection
            });
        });
    } catch (error) {
        res.status(500).send({
            message: 'Daemon interaction unsuccessful.',
        });
    }
});

/**
 * UPDATE COLLECTION
 * Use name parameter from URI to indicate which collection to update
 * socket.emit('update', {userDB: { 'oldCollection#reference': 'newCollection' },});
 * 
 * @param {string} '/databases/:databaseID/collections/:name' is the url endpoint
 * @param {Object} req The request from JS client library
 * @param {Object} res response to send back to JS client library
 * 
 * @returns {Object} relevant status code with a message to indicate successful/unsuccessful login
 */

app.put('/databases/:databaseID/collections/:name?', async (req, res) => {
    const { databaseID, name } = req.params;
    const { updatedCollection } = req.body;
    if (!databaseID || !name || !updatedCollection) {
        return res.status(400).send({
            message: 'Collection name and Database ID required'
        });
    }

    try {
        const collection = { [databaseID]: { [`${name}#reference`]: updatedCollection } };
        DaemonServer.emit('addCommand', 'update', collection);

        DaemonServer.once('response', (data) => {
            res.status(data.response).send({
                message: data.message,
                data: collection
            });
        });
    } catch (error) {
        res.status(500).send({
            message: 'Daemon interaction unsuccessful.',
        });
    }
});

/**
 * DELETE COLLECTION
 * Use name parameter from URI to indicate which collection to delete
 * Supports regex for more flexible deletion
 * 
 * @param {string} '/databases/:databaseID/collections/:name' is the url endpoint
 * @param {Object} req The request from JS client library
 * @param {Object} res response to send back to JS client library
 * 
 * @returns {Object} relevant status code with a message to indicate successful/unsuccessful deletion
 */

app.delete('/databases/:databaseID/collections/:name?', async (req, res) => {
    const {databaseID, name} = req.params;
    const {regex,equalityComparison} = req.query; // Check if regex parameter is provided

    if (!databaseID || (!name && !regex)) {
        return res.status(400).send({
            message: "Collection name or regex pattern and Database ID required"
        });
    }


    try {
        let collection;
        if (regex) {
            // Handle regex-based deletion
            const regexSent = regex + name;
            collection = {
                [databaseID]: {
                    //[name]:{
                        [regexSent]:{}
                    //} // Use regex for collection name pattern matching
                }
            };
        } else if (!equalityComparison && (!regex)){
            // Standard deletion by exact name
            collection = {[databaseID]:{"==":name}};
        }else{
                if (equalityComparison === "ne")
                    {
                        collection = {[databaseID]:{"!=":name}};
                    }else{
                        collection = {[databaseID]:{"==":name}};
                    }
            }
                
        
        DaemonServer.emit("addCommand", "delete", collection);

        DaemonServer.once("response", (data) => {
            res.status(data.response).send({
                message: data.message,
                data: collection
            });
        });
    } catch (error) {
        res.status(500).send({
            message: "Daemon interaction unsuccessful.",
            error: error.message
        });
    }
});


/**
 * GET COLLECTION
 * Use name parameter from URI to indicate which collection to read
 * Supports comparison operators for more advanced querying
 * 
 * @param {string} '/databases/:databaseID/collections/:name' is the url endpoint
 * @param {Object} req The request from JS client library
 * @param {Object} res response to send back to JS client library
 * 
 * @returns {Object} relevant status code with a message to indicate successful/unsuccessful query
 */

app.get('/databases/:databaseID/collections/:name?', async (req, res) => {
    const {databaseID, name} = req.params;
    const query = req.query; // For comparison operators
    const {regex,equalityComparison} = query;
    if (!databaseID || !name) {
        return res.status(400).send({
            message: "Collection name or DatabaseID required"
        });
    }

    try {
        let collection = {[databaseID]: {"==":name}};
        
        // Handle only equality operators if present
        if (Object.keys(query).length > 0) {
            const operators = {};
            
            // Process each query parameter for equality operators only
            Object.keys(query).forEach(key => {
                const [field, operator] = key.split('=');
                
                if (operator) {
                    // Only handle == and != operators
                    switch(operator) {
                        case 'eq': // For equality (==)
                            operators[field] = {"==": query[key]};
                            collection = {[databaseID]: {"==":name}};
                            break;
                        case 'ne': // For inequality (!=)
                            operators[field] = {"!=": query[key]};
                            collection = {[databaseID]: {"!=":name}};
                            break;
                        default:
                            // Ignore unsupported operators
                            break;
                    }
                } else {
                    // Direct equality comparison (==)
                    operators[key] = {"==": query[key]};
                }
            });
            
            // Add operators to collection query if any were processed
            /*if (Object.keys(operators).length > 0) {
                const index1 = Object.keys(operators)[0];
                const index = operators[index1];
                //collection = {[databaseID]: {[index]:name}};
            }*/
        }
        if (regex)
        {
            var rString = regex+name;
            collection = {[databaseID]: {[name]:{[rString]:{}}}};
        }else if (equalityComparison)
        {
            switch(equalityComparison) {
                case 'eq': // For equality (==)
                    collection = {[databaseID]: {"==":name}};
                    break;
                case 'ne': // For inequality (!=)
                    collection = {[databaseID]: {"!=":name}};
                    break;
                default:
                    // Ignore unsupported operators
                    break;
            }
            //collection = {[databaseID]: {[equalityComparison]:name}};
        }
        let data = {
            data: collection,
                    pageNumber: 1,
                    limit: 10
                    };
        DaemonServer.emit("addCommand", "search", data);

        DaemonServer.once("response", (data) => {
            res.status(data.response).send({
                message: data.message,
                data: data.results || collection
            });
        });
    } catch (error) {
        res.status(500).send({
            message: "Daemon interaction unsuccessful.",
            error: error.message
        });
    }
});

app.get('/databases/:databaseID/allCollections', async (req, res) => {
    const {databaseID} = req.params;
    const query = req.query; // For potential filtering
    
    if (!databaseID) {
        return res.status(400).send({
            message: "Database ID required"
        });
    }
    
    try {
        let collection = {[databaseID]: {}};
        
        // Add query filters if provided - only equality operators
        if (Object.keys(query).length > 0) {
            const filters = {};
            
            Object.keys(query).forEach(key => {
                const [field, operator] = key.split('_');
                
                if (operator) {
                    // Only handle == and != operators
                    switch(operator) {
                        case 'eq': // For equality (==)
                            filters[field] = {"==": query[key]};
                            break;
                        case 'ne': // For inequality (!=)
                            filters[field] = {"!=": query[key]};
                            break;
                        default:
                            // Ignore unsupported operators
                            break;
                    }
                } else {
                    // Direct equality comparison (==)
                    filters[key] = {"==": query[key]};
                }
            });
            
            // Apply filters to the query if any were processed
            if (Object.keys(filters).length > 0) {
                collection = {[databaseID]: filters};
            }
        }
        collection = {[databaseID]: {}};
        DaemonServer.emit("addCommand", "list", collection);

        DaemonServer.once("response", (data) => {
            res.status(data.response).send({
                message: data.message,
                data: data.data || collection
            });
        });
    } catch (error) {
        res.status(500).send({
            message: "Daemon interaction unsuccessful.",
            error: error.message
        });
    }
});


//DATABASES

/**
 * CREATE Database
 * 
 * 
 * @param {string} '/databases' is the url endpoint
 * @param {Object} req The request from JS client library
 * @param {Object} res response to send back to JS client library
 * 
 * @returns {Object} relevant status code with a message to indicate successful/unsuccessful login
 */

app.post('/databases', async (req, res) => {
    const {database} = req.body;

    if (!database || Object.keys(database).length === 0) {
        return res.status(400).send({
            message: 'Database data required'
        });
    }
    const db = { [database] : {}};
    
    try {
        DaemonServer.emit('addCommand', 'create', db);

        DaemonServer.once('response', (data) => {
            res.status(data.response).send({
                message: data.message,
                data: db
            });
        });
    } catch (error) {
        res.status(500).send({
            message: 'Daemon interaction unsuccessful.',
        });
    }
});

/**
 * UPDATE Database
 * 
 * 
 * @param {string} '/databases/:databaseID' is the url endpoint
 * @param {Object} req The request from JS client library
 * @param {Object} res response to send back to JS client library
 * 
 * @returns {Object} relevant status code with a message to indicate successful/unsuccessful login
 */

app.put('/databases/:databaseID?', async (req, res) => {
    const { newDatabase } = req.body;
    const { databaseID } = req.params;

    if (!newDatabase) {
        return res.status(400).send({
            message: 'Database data required'
        });
    }
    if (!databaseID) {
        return res.status(400).send({
            message: 'Database ID data required'
        });
    }

    try {
        DaemonServer.emit('addCommand', 'update', { [`${databaseID}#reference`]: newDatabase });

        DaemonServer.once('response', (data) => {
            res.status(data.response).send({
                message: data.message,
                data: { [`${databaseID}#reference`]: newDatabase }
            });
        });
    } catch (error) {
        res.status(500).send({
            message: 'Daemon interaction unsuccessful.',
        });
    }
});

/**
 * READ Database
 * 
 * 
 * @param {string} '/databases/:databaseID' is the url endpoint
 * @param {Object} req The request from JS client library
 * @param {Object} res response to send back to JS client library
 * 
 * @returns {Object} relevant status code with a message to indicate successful/unsuccessful login
 */
app.get('/databases/:databaseID?', async (req, res) => {
    const { databaseID } = req.params;
    var {equalityComparison,pageNumber,limit} = req.query; //GET /databases/mydb?equalityComparison=!=

    if (!databaseID) {
        return res.status(400).send({
            message: 'Database ID required'
        });
    }
     if (!limit)
    {
        limit =10;
    }
    if (!pageNumber)
    {
        pageNumber = 1;
    }
    var database;
    if (!equalityComparison)
    {
        database = {
            data: {"==":databaseID},
                    pageNumber: 1,
                    limit: 10
                    };
    }else{
        if (equalityComparison === "eq")
        {
            database = {
                data: {"==":databaseID},
                         pageNumber: pageNumber,
                        limit: limit
                        };
        }else{
            database = {
                data: {"!=":databaseID},
                         pageNumber: pageNumber,
                        limit: limit
                        };
        }
    }
    try {
        DaemonServer.emit('addCommand', 'search', database);

        DaemonServer.once('response', (daemonRes) => {
            res.status(daemonRes.response).send({
                message: daemonRes.message,
                results: daemonRes.results
            });
        });
    } catch (error) {
        res.status(500).send({
            message: 'Daemon interaction unsuccessful in get',
        });
    }
});


/**
 * READ all Databases
 * 
 * 
 * @param {string} '/databases/:databaseID' is the url endpoint
 * @param {Object} req The request from JS client library
 * @param {Object} res response to send back to JS client library
 * 
 * @returns {Object} relevant status code with a message to indicate successful/unsuccessful login
 */

app.get('/allDatabases', async (req, res) => {
    var {pageNumber,limit} = req.query;
    if (!limit)
    {
        limit =10;
    }
    if (!pageNumber)
    {
        pageNumber = 1;
    }

    var database;
    {
        database = {data: {"!=": ""},
                    pageNumber: pageNumber,
                    limit: limit
                    };
    }
    try {
        DaemonServer.emit('addCommand', 'search', database);

        DaemonServer.once('response', (daemonRes) => {
            res.status(daemonRes.response).send({
                message: daemonRes.message,
                results: daemonRes.results
            });
        });
    } catch (error) {
        res.status(500).send({
            message: 'Daemon interaction unsuccessful in get',
        });
    }
});


/**
 * DELETE Database
 * 
 * @param {string} '/databases/:databaseID' is the URL endpoint
 * @param {Object} req The request from the JS client library
 * @param {Object} res The response to send back to the JS client library
 * 
 * @returns {Object} Status code and message indicating the outcome of the deletion
 */
app.delete('/databases/:databaseID?', async (req, res) => {
    const { databaseID } = req.params;
    const {equalityComparison} = req.query;
    if (!databaseID) {
        return res.status(400).send({ message: 'Database ID required' });
    }
    var database;
    if (!equalityComparison)
    {
        database = { "==":databaseID };
    }else{
        if (equalityComparison === "eq")
            {
                database = {"==":databaseID};
            }else{
                database = {"!=":databaseID};
            }
    }
    try {
        DaemonServer.emit('addCommand', 'delete', database);
        DaemonServer.once('response', (data) => {
            res.status(data.response).send({
                message: data.message,
                data: database
            });
        });
    } catch (error) {
        res.status(500).send({ message: 'Daemon interaction unsuccessful' });
    }
});



app.post("/import", async (req, res) => {
    const {data} = req.body;
    if (!data)
    {
        return res.status(400).send({
            message: 'Data required'
        });
    }
    try {
        DaemonServer.emit('import', data);

        DaemonServer.once('response', (daemonRes) => {
            res.status(daemonRes.response).send({
                message: daemonRes.message,
                results: daemonRes.results
            });
        });
    } catch (error) {
        res.status(500).send({
            message: 'Daemon interaction unsuccessful for importing'
        });
    }
});
app.get("/export", async (req, res) => {
    const {data} = req.query;
    if (!data)
        {
            return res.status(400).send({
                message: 'Data required'
            });
        }
    try {
        DaemonServer.emit('export', data);

        DaemonServer.once('response', (daemonRes) => {
            res.status(daemonRes.response).send({
                message: daemonRes.message,
                results: daemonRes.results
            });
        });
    } catch (error) {
        res.status(500).send({
            message: 'Daemon interaction unsuccessful for importing'
        });
    }
});



//make server go live on host ->express handles HTTP 
const server = app.listen(
    PORT,
    () => {
        console.log(`API alive on http://${HOST}:${PORT}`);
    });

module.exports = { api: app, server }; //needed for jest unit testing
