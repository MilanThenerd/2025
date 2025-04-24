#!/usr/bin/env node
/**
 * @file Cli.js
 * @author Henru Matthis , Karabelo Taole
 * @version 1.0.0
 * @brief Command-line interface for the CQL database system
 * 
 * This module provides a CLI for interacting with the CQL database daemon.
 * It supports CRUD operations on databases, collections, and documents.
 * 
/*TODO: 
  - Integrate other list based commands
  - ID instead of key,value for find document
  - Passing int, floats and not just string values

  - updateDoc
  - showAll
  - Seperate UI and Logic for reusability? Remove recursion? Optional
  - Return all docs with certain field regardless of value
*/
import { io } from 'socket.io-client';
import readline from 'readline';
import boxen from 'boxen';
import chalk from 'chalk';
import Table from 'cli-table3';
import { stdin as input, stdout as output } from 'process';
import fs from 'fs';
import path from 'path';
import { fileURLToPath } from 'url';
import { dirname } from 'path';

// Workaround for __dirname in ES modules
const __filename = fileURLToPath(import.meta.url);
const __dirname = dirname(__filename);

/**
 * @brief Daemon configuration parameters
 * 
 * Defines the connection parameters for the CQL daemon
 */
const DAEMON_URL = 'http://127.0.0.1:8008';
let daemonActive = false;
const client = io(DAEMON_URL, {
    autoConnect: true,
    reconnection: true,
    reconnectionAttempts: 1,
    timeout: 5000,
    reconnectionDelay: 1000
});

// Export necessary variables for use in other modules
export { client, daemonActive };

/**
 * @brief Timeout to detect if initial connection to the daemon fails
 * 
 * Sets a timer to check if the connection to the daemon was successful after 3 seconds.
 * If not, it displays the connection failure menu.
 */
const connectionTimeout = setTimeout(() => {
    if (!client.connected) {
        console.log(chalk.red('❌ Failed to connect to daemon'));
        daemonActive = false;
        client.disconnect();
        showConnectionFailureMenu();
    }
}, 3000); // Wait 3 seconds for connection

/**
 * @brief Event handler for successful connection to the daemon
 * 
 * Sets daemon status to active and displays the main menu
 */
client.on('connect', () => {
    clearTimeout(connectionTimeout);
    daemonActive = true;
    //Register
    let dataToSend = {
        userName: "cli_user",
        email: "cli@email.com",
        password: "cli1234" 
    };
    client.emit('register', dataToSend);
    //Login
    dataToSend = {
        userName: "cli_user",
        email: "cli@email.com",
        password: "cli1234" 
    };
    client.emit('login', dataToSend);
    console.log(chalk.green('✅ Connected to daemon at ' + DAEMON_URL));
    showMainMenu();
});

/**
 * @brief Event handler for connection errors
 * 
 * @param error The error object from the connection attempt
 */
client.on('connect_error', (error) => {
    clearTimeout(connectionTimeout);
    console.log(chalk.red(`❌ Failed to connect to daemon: ${error.message}`));
    daemonActive = false;
    client.disconnect();
    showConnectionFailureMenu();
});

// Add a flag to track exit state
let isExiting = false;

/**
 * @brief Event handler for disconnection from the daemon
 * 
 * @param reason The reason for disconnection
 */
client.on('disconnect', (reason) => {
    if (isExiting) {
        // If we're already exiting, just log the message but don't try to show menu
        console.log(chalk.yellow(`📢 Disconnected from daemon: ${reason}`));
        return;
    }
    
    console.log(chalk.red(`❌ Disconnected from daemon: ${reason}`));
    daemonActive = false;
    showConnectionFailureMenu();
});

/**
 * @brief Event handler for failed reconnection attempts
 * 
 * Displays the connection failure menu after all reconnection attempts fail
 */
client.on('reconnect_failed', () => {
    console.log(chalk.red('❌ Reconnection attempts failed'));
    daemonActive = false;
    client.disconnect();
    showConnectionFailureMenu();
});

/**
 * @brief Event handler for responses from the daemon
 * 
 * Processes various response types from the daemon and displays appropriate UI
 * 
 * @param response The response object from the daemon
 */
client.on('response', (response) => {
    if (response.response === 200) {
        // console.log(chalk.green(`✅ ${response.message}`));
    } else {
        // console.log(chalk.red(`❌ ${response.message}`));
    }

    switch (response.type) {
        case 'list':
            console.log(chalk.blue.bold('\n📂 Available Databases:'));
            if (response.data && Array.isArray(response.data) && response.data.length > 0) {
                response.data.forEach(db => {
                    console.log(chalk.magenta(`  ├── 📁 ${db}`));
                });
            }

            if (response.data.length <= 0) {
                console.log(chalk.yellow('  └── (No Databases)'));
                console.log('\n');
            }

            setTimeout(showReadMenu, 1000);
            break;

        case 'create':
            if (response.response === 200) {
                console.log(chalk.green(`✅ ${response.message}`));
            } else {
                console.log(chalk.red(`❌ ${response.message}`));
            }

            setTimeout(showCreateMenu, 1000);
            break;

        case 'delete':
            if (response.response === 200) {
                console.log(chalk.green(`✅ ${response.message}`));
            } else {
                console.log(chalk.red(`❌ ${response.message}`));
            }

            setTimeout(showDeleteMenu, 1000);
            break;

        case 'update':
            if (response.response === 200) {
                console.log(chalk.green(`✅ ${response.message}`));
            } else {
                console.log(chalk.red(`❌ ${response.message}`));
            }

            setTimeout(showMainMenu, 1000);
            break;

        case 'search':
            if (response.response === 200) {
                console.log(chalk.green(`✅ ${response.message}`));
                //console.log(JSON.stringify(response.results));

                if (response.results && response.results.length > 0) {
                    console.log(chalk.blue.bold('\n🔍 Search Results:'));

                    response.results.forEach(result => {
                        console.log(chalk.magenta(`\nDatabase: ${result.database}, Collection: ${result.collection}`));
                        console.log(chalk.yellow(`Found ${result.count} document(s)`));

                        if (result.documents && result.documents.length > 0) {
                            const table = new Table({
                                head: [chalk.cyan.bold('Key'), chalk.yellow.bold('Value')],
                                colWidths: [30, 50],
                                style: { head: [], border: [] }
                            });

                            result.documents.forEach(doc => {
                                Object.entries(doc).forEach(([key, value]) => {
                                    const displayValue = typeof value === 'object'
                                        ? JSON.stringify(value)
                                        : value;
                                    
                                            
                                    table.push([
                                        chalk.blue(key),
                                        chalk.green(displayValue)
                                    ]);
                                    if (key === "_createdAt") {
                                        table.push(['', '']);}
                                });
                            });

                            console.log(table.toString());
                         
                        }
                    });
                } else {
                    console.log(chalk.yellow('No results found.'));
                }
            } else {
                console.log(chalk.red(`❌ ${response.message}`));
            }

            setTimeout(showReadMenu, 3000); // Give more time to see results
            break;
        default:
            break;
    }
});


client.on('export', (response) => {
    console.log(JSON.stringify(response.buffer));
    const arrayBuffer = response.buffer;
    const buffer = Buffer.from(arrayBuffer);
    //console.log('\n');
    const filePath = path.join(__dirname, 'exportedData.json');
    fs.writeFile(filePath, buffer, (err) => {
        if (err) {
            console.error('Error saving file:', err);
        } else {
            console.log('Exported data received and saved to file.');
        }
    });
});




/**
 * @brief Create readline interface for user input
 */
const rl = readline.createInterface({ input, output });

/**
 * @brief Layout function for displaying menus
 * 
 * Creates a boxed UI element with the specified content and styling
 * 
 * @param {string} menuText - The text content to display inside the menu box
 * @param {string} menuType - The title text for the menu box
 * @param {string} bCol - The border color for the menu box
 */
function layout(menuText, menuType, bCol) {
    console.log(boxen(menuText, {
        float: 'center',
        padding: { top: 1, bottom: 1, left: 1, right: 1 },
        title: menuType,
        titleAlignment: 'center',
        borderColor: bCol,
        borderStyle: 'round'
    }));
}

/**
 * @brief Displays the main menu and prompts user for choice
 */
export function showMainMenu() {
    const menuText = `
1. Create 
2. Read
3. Update
4. Delete
5. Exit
    `;
    layout(menuText, chalk.blue('📦 CQL Database CLI - Main Menu'), 'blue');

    rl.question(chalk.yellow('Choose an option: '), (choice) => {
        handleMainMenuChoice(choice);
    });
}

/**
 * @brief Processes user's selection from the main menu
 * 
 * Routes the application flow based on the user's choice
 * 
 * @param {string} choice - The user's selected option
 */
function handleMainMenuChoice(choice) {
    switch (choice.trim()) {
        case '1':
            showCreateMenu();
            break;
        case '2':
            showReadMenu();
            break;
        case '3':
            showUpdateMenu();
            break;
        case '4':
            showDeleteMenu();
            break;
        case '5':
            //showExportImportMenu();
            //runTests(showMainMenu);// Call the tests
            exit();
            break;
        // case '6':
        //     exit();
        //     break;
        default:
            console.log(chalk.red('⚠️ Invalid option! Please try again.'));
            showMainMenu();
    }
}

/**
 * @brief Displays the create menu with options for creating database objects
 */
function showCreateMenu() {
    const menuText = `
1. New Database
2. New Collection
3. New Document
4. Return to previous menu
    `;
    layout(menuText, chalk.green('📦 CQL Database CLI - Create Menu'), 'green');

    rl.question(chalk.yellow('Choose an option: '), (choice) => {
        handleCreateMenuChoice(choice);
    });
}

/**
 * @brief Processes user's selection from the create menu
 * 
 * @param {string} choice - The user's selected option
 */
function handleCreateMenuChoice(choice) {
    switch (choice.trim()) {
        case '1':
            createDatabase();
            break;
        case '2':
            createCollection();
            break;
        case '3':
            insertDocument();
            break;
        case '4':
            showMainMenu();
            break;
        default:
            console.log(chalk.red('⚠️ Invalid option! Please try again.'));
            showCreateMenu();
    }
}

/**
 * @brief Displays the read menu with options for querying database objects
 */
function showReadMenu() {
    const menuText = `
1. Retrieve Document/s by Value
2. Retrieve Document/s by Query
3. List Collection/s
4. List Database/s
5. Return to previous menu
    `;
    layout(menuText, chalk.yellow('📦 CQL Database CLI - Read Menu'), 'green');

    rl.question(chalk.yellow('Choose an option: '), (choice) => {
        handleReadMenuChoice(choice);
    });
}

/**
 * @brief Processes user's selection from the read menu
 * 
 * @param {string} choice - The user's selected option
 */
function handleReadMenuChoice(choice) {
    switch (choice.trim()) {
        case '1':
            retrieveDocumentByValue();
            break;
        case '2':
            retrieveDocumentByQuery();
            break;
        case '3':
            retrieveCollections();
            break;
        case '4':
            retrieveDatabases();
            break;
        case '5':
            showMainMenu();
            break;
        default:
            console.log(chalk.red('⚠️ Invalid option! Please try again.'));
            showReadMenu();
    }
}

/**
 * @brief Displays the delete menu with options for removing database objects
 */
function showDeleteMenu() {
    const menuText = `
1. Delete Database
2. Delete Collection
3. Delete Document by ID
4. Return to previous menu
    `;
    layout(menuText, chalk.yellow('📦 CQL Database CLI - Delete Menu'), 'red');

    rl.question(chalk.yellow('Choose an option: '), (choice) => {
        handleDeleteMenuChoice(choice);
    });
}

/**
 * @brief Processes user's selection from the delete menu
 * 
 * @param {string} choice - The user's selected option
 */
function handleDeleteMenuChoice(choice) {
    switch (choice.trim()) {
        case '1':
            deleteDatabase();
            break;
        case '2':
            deleteCollection();
            break;
        case '3':
            deleteDocument();
            break;
        case '4':
            showMainMenu();
            break;
        default:
            console.log(chalk.red('⚠️ Invalid option! Please try again.'));
            showDeleteMenu();
    }
}

/**
 * @brief Displays the update menu with options for renaming/modifying database objects
 */
function showUpdateMenu() {
    const menuText = `
1. Rename Database
2. Rename Collection
3. Update Document
4. Return to previous menu
    `;
    layout(menuText, chalk.yellow('📦 CQL Database CLI - Update Menu'), 'green');

    rl.question(chalk.yellow('Choose an option: '), (choice) => {
        handleUpdateMenuChoice(choice);
    });
}

/**
 * @brief Processes user's selection from the update menu
 * 
 * @param {string} choice - The user's selected option
 */
function handleUpdateMenuChoice(choice) {
    switch (choice.trim()) {
        case '1':
            renameDatabase();
            break;
        case '2':
            renameCollection();
            break;
        case '3':
            updateDocument();
            break;
        case '4':
            showMainMenu();
            break;
        default:
            console.log(chalk.red('⚠️ Invalid option! Please try again.'));
            showUpdateMenu();
    }
}

/**
 * Displays connection failure menu with options when daemon is unavailable
 * @param {string} errorMessage - The error message from the connection failure
 */
function showConnectionFailureMenu(errorMessage) {
    const menuText = `
${chalk.red('Connection to Daemon Failed')}
${chalk.yellow('Error: ' + errorMessage)}

1. Retry Connection
2. Exit
    `;
    layout(menuText, chalk.red('⚠️ Connection Error'), 'red');

    rl.question(chalk.yellow('Choose an option: '), (choice) => {
        handleConnectionFailureChoice(choice, errorMessage);
    });
}

/**
 * Processes user's selection from the connection failure menu
 * @param {string} choice - The user's selected option
 * @param {string} errorMessage - The original error message
 */
function handleConnectionFailureChoice(choice, errorMessage) {
    switch (choice.trim()) {
        case '1':
            console.log(chalk.blue('Attempting to reconnect...'));
            // Try to reconnect to the daemon
            attemptReconnection();
            break;
        case '2':
            exit();
            break;
        default:
            console.log(chalk.red('⚠️ Invalid option! Please try again.'));
            showConnectionFailureMenu(errorMessage);
    }
}

/**
 * Attempts to reconnect to the daemon
 */
function attemptReconnection() {
    console.log(chalk.blue('🔄 Reconnecting to daemon...'));

    if (client.disconnected) {
        client.connect();
    }
}

/**
 * Creates a new database in either the daemon or in-memory storage
 * 
 * @param {string} [name] - Optional name of the database to create. If not provided, function prompts user for input.
 * @returns {void}
 * 
 * @description
 * This function creates a new database with the specified name. If no name is provided, it prompts the user
 * to enter a name. The function behaves differently based on whether the daemon is active:
 * 
 * - If daemon is active: Sends a creation command to the daemon service
 * - If daemon is inactive: Creates the database in the local in-memory storage
 * 
 * The function prevents creation of duplicate databases in the in-memory mode and handles
 * empty name validation when prompting for user input.
 **/
export function createDatabase(name) {
    if (!name) {
        rl.question(chalk.yellow('Enter database name: '), (name) => {
            if (!name) {
                console.log(chalk.red('⚠️ Database name cannot be empty.'));
                showMainMenu();
                return;
            }

            if (daemonActive) {
                const dataToSend = { [name]: {} };
                // Emit the 'create' command to the daemon
                client.emit('addCommand', 'create', dataToSend);
            } else {
                console.log(chalk.yellow('⚠️ No connection to Daemon.'));
                showConnectionFailureMenu();
            }
        });
    } else {
        if (daemonActive) {
            const dataToSend = { [name]: {} };
            client.emit('addCommand', 'create', dataToSend);
        } else {
            console.log(chalk.yellow('⚠️ No connection to Daemon.'));
            showConnectionFailureMenu();
        }
    }
}

/**
 * Creates a new collection in either the daemon or in-memory storage
 * 
 * @param {string} [dbName] - Optional name of the database to create. If not provided, function prompts user for input.
 * @param {string} [collectionName] - Optional name of the collection to create. If not provided, function prompts user for input.
 * @returns {void}
 * 
 * @description
 * This function creates a new collection and database with the specified name. If no name is provided, it prompts the user
 * to enter a name. The function behaves differently based on whether the daemon is active:
 * 
 * - If daemon is active: Sends a creation command to the daemon service
 * - If daemon is inactive: Creates the collection in the local in-memory storage
 * - If the database does not exist, it creates the database first
 * 
 * The function prevents creation of duplicate collections in the in-memory mode and handles
 * empty name validation when prompting for user input.
 **/
export function createCollection(dbName, collectionName) {
    if (!dbName || !collectionName) {
        rl.question(chalk.yellow('Enter database name: '), (dbName) => {
            rl.question(chalk.yellow('Enter collection name: '), (collectionName) => {
                if (!dbName || !collectionName) {
                    console.log(chalk.red('⚠️ Database and collection names cannot be empty.'));
                    showMainMenu();
                    return;
                }
                if (daemonActive) {
                    const dataToSend = { [dbName]: { [collectionName]: {} } };
                    client.emit('addCommand', 'create', dataToSend);
                } else {
                    console.log(chalk.yellow('⚠️ No connection to Daemon.'));
                    showConnectionFailureMenu();
                }
            });
        });
    } else {
        if (daemonActive) {
            const dataToSend = { [dbName]: { [collectionName]: {} } };
            client.emit('addCommand', 'create', dataToSend);
        } else {
            console.log(chalk.yellow('⚠️ No connection to Daemon.'));
            showConnectionFailureMenu();
        }
    }
}

/**
 * Creates a new document in either the daemon or in-memory storage
 * 
 * @param {string} [dbName] - Optional name of the database to create. If not provided, function prompts user for input.
 * @param {string} [collectionName] - Optional name of the collection to create. If not provided, function prompts user for input.
 * @param {string} [key] - Optional name of the key field to create. If not provided, function prompts user for input.
 * @param {string} [value] - Optional name of the value to create. If not provided, function prompts user for input.
 * @returns {void}
 * 
 * @description
 * This function creates a new document with the key and value. If no parameters are provided, it prompts the user
 * to enter a them. The function behaves differently based on whether the daemon is active:
 * 
 * - If daemon is active: Sends a creation command to the daemon service
 * - If daemon is inactive: Creates the collection in the local in-memory storage
 * - If the database and or collection does not exist, it creates them first
 **/
export function insertDocument(dbName, collectionName) {
    if (!dbName || !collectionName) {
        rl.question(chalk.yellow('Enter database name: '), (dbName) => {
            rl.question(chalk.yellow('Enter collection name: '), (collectionName) => {
                repetition(dbName, collectionName);
            });
        });
    } else {
        repetition(dbName, collectionName);
    }
}
// Function that does the looping
function repetition(dbName, collectionName) {
    rl.question(chalk.yellow('Number of Key Value Pairs to be added:  '), (count) => {
        const numPairs = parseInt(count);
        if (isNaN(numPairs) || numPairs <= 0) {
            console.log(chalk.red('⚠️ Please enter a valid positive number.'));
            showCreateMenu();
            return;
        }

        let pairs = {};
        let current = 0;

        function askPair() {
            if (current >= numPairs) {
                // End loop
                if (daemonActive) {
                    const dataToSend = { [dbName]: { [collectionName]: [pairs] } };
                    client.emit('addCommand', 'create', dataToSend);
                } else {
                    console.log(chalk.yellow('⚠️ No connection to Daemon.'));
                    showConnectionFailureMenu();
                }
                return;
            }

            rl.question(chalk.yellow(`Enter key #${current + 1}: `), (key) => {
                rl.question(chalk.yellow(`Enter value for "${key}": `), (value) => {
                    if (!key || !value || value === '') {
                        console.log(chalk.red('⚠️ Key and value cannot be empty.'));
                        askPair(); // Retry current pair
                        return;
                    }
                    pairs[key] = detectAndConvertType(value);
                    current++;
                    askPair();
                });
            });
        }

        askPair(); // Start loop
    });
}

/**
 * Detects and converts input string to appropriate data type
 * 
 * @param {string} input - The input string to convert
 * @returns {any} - The converted value with appropriate type
 */
function detectAndConvertType(input){
    if (typeof input === 'string') {
        //Declared as String
        if (input.startsWith('s:')) return input.substring(2);
        //Declared as Number
        if (input.startsWith('n:')) return Number(input.substring(2));
        //Declared as Boolean
        if (input.startsWith('b:')) {
            const boolValue = input.substring(2).toLowerCase();
            return boolValue === 'true' || boolValue === '1' || boolValue === 'yes';
        }
        //Declared as Date
        if (input.startsWith('d:')) return new Date(input.substring(2));
        //Declared as Array
        if (input.startsWith('a:')) { 
            try {
                return JSON.parse(input.substring(2));
            } catch (e) {
                console.log(chalk.red(`⚠️ Invalid array format: ${e.message}`));
                return input;
            }
        }
        //Declared as JSON Object
        if (input.startsWith('o:')) {
            try {
                return JSON.parse(input.substring(2));
            } catch (e) {
                console.log(chalk.red(`⚠️ Invalid object format: ${e.message}`));
                return input;
            }
        }

        //Boolean
        if (input.toLowerCase() === 'true') return true;
        if (input.toLowerCase() === 'false') return false;

        //Null type
        if (input.toLowerCase() === 'null') return null;

        //Number int or float
        if (/^-?\d+$/.test(input)) return parseInt(input, 10);
        if (/^-?\d+\.\d+$/.test(input)) return parseFloat(input);

        //Date
        if (/^\d{4}-\d{2}-\d{2}(T\d{2}:\d{2}:\d{2}(\.\d{3})?Z?)?$/.test(input)) {
            const dateObj = new Date(input);
            if (!isNaN(dateObj.getTime())) return dateObj;
        }

        //JSON array/object
        if ((input.startsWith('[') && input.endsWith(']')) || 
            (input.startsWith('{') && input.endsWith('}'))) {
            try {
                return JSON.parse(input);
            } catch (e) {
                // Invalid , treat as string
            }
        }
    }
    //Default ie String
    return input;
}


/**
 * Retrieves a document by key/value pair from a specified database and collection
 * 
 * @param {string} [dbName] - Database name to search in
 * @param {string} [collectionName] - Collection name to search in
 * @param {string} [key] - Document key to search for
 * @param {string} [value] - Value to match (only used in daemon mode)
 * @returns {void}
 * 
 * @description
 * This function retrieves a document from a specified database and collection based on
 * a key/value pair. If any required parameters are missing, it prompts the user for input.
 * The function behaves differently based on whether the daemon is active:
 * 
 * - If daemon is active: Sends a search command to the daemon with the specified criteria
 * - If daemon is inactive: Retrieves the document directly from the in-memory storage
 * 
 * The function handles cases where the database, collection, or document doesn't exist
 * and provides appropriate feedback to the user.
 **/

export function retrieveDocumentByValue(dbName, collectionName, key, value) {
    // If any of the parameters are missing, prompt the user for them
    if (!dbName || !collectionName || !key || !value) {
        rl.question(chalk.yellow('Enter database name: '), (dbNameInput) => {
            // If the database name is empty, go back to Read Menu
            if (!dbNameInput) {
                console.log(chalk.red('⚠️ Database name cannot be empty.'));
                showReadMenu();
                return;
            }
            rl.question(chalk.yellow('Enter collection name: '), (colNameInput) => {
                // If the collection name is empty, go back to Read Menu
                if (!colNameInput) {
                    console.log(chalk.red('⚠️ Collection name cannot be empty.'));
                    showReadMenu();
                    return;
                }

                rl.question(chalk.yellow('Enter key: '), (keyInput) => {
                    // If the key is empty, go back to Read Menu
                    if (!keyInput) {
                        console.log(chalk.red('⚠️ Key cannot be empty.'));
                        showReadMenu();
                        return;
                    }

                    rl.question(chalk.yellow('Enter value: '), (valueInput) => {
                        // If the value is empty, go back to Read Menu
                        if (!valueInput) {
                            console.log(chalk.red('⚠️ Value  cannot be empty.'));
                            showReadMenu();
                            return;
                        }

                        // Call retrieveDocument recursively with user inputs if all fields are valid
                        retrieveDocumentByValue(dbNameInput, colNameInput, keyInput, valueInput);
                    });
                });
            });
        });
        return; // Exit early to await user input
    }

    // Check if the database exists in the loaded databases
    if (daemonActive) {
        const dataToSend = { data: {[dbName]: { [collectionName]: { [key]: value } } } };
        client.emit('addCommand', 'search', dataToSend);
    } else {
        console.log(chalk.yellow('⚠️ No connection to Daemon.'));
        showConnectionFailureMenu();
    }
}

/**
 * Retrieve documents by query
 * @param {string} dbName - Database name
 * @param {string} collectionName - Collection name
 * @param {string} field - Field name to query on
 * @param {string} operator - Operator to use ("==", "!=", ">", "<", ">=", "<=")
 * @param {string} value - Value to compare against
 */
export function retrieveDocumentByQuery(dbName, collectionName, field, operator, value) {
    // If any of the parameters are missing, prompt the user for them
    if (!dbName || !collectionName || !field || !operator || value === undefined) {
        console.log(chalk.yellow('⚠️ Please provide the required details to retrieve the document.'));
        rl.question(chalk.yellow('Enter database name: '), (dbNameInput) => {
            // If the database name is empty, go back to Read Menu
            if (!dbNameInput) {
                console.log(chalk.red('⚠️ Database name cannot be empty.'));
                showReadMenu();
                return;
            }

            rl.question(chalk.yellow('Enter collection name: '), (colNameInput) => {
                // If the collection name is empty, go back to Read Menu
                if (!colNameInput) {
                    console.log(chalk.red('⚠️ Collection name cannot be empty.'));
                    showReadMenu();
                    return;
                }

                rl.question(chalk.yellow('Enter field name: '), (fieldInput) => {
                    // If the field is empty, go back to Read Menu
                    if (!fieldInput) {
                        console.log(chalk.red('⚠️ Field name cannot be empty.'));
                        showReadMenu();
                        return;
                    }

                    const validOperators = ["==", "!=", ">", "<", ">=", "<="];
                    console.log(chalk.yellow(`Available operators: ${validOperators.join(', ')}`));

                    rl.question(chalk.yellow('Enter operator: '), (operatorInput) => {
                        // Check if the operator is valid
                        if (!validOperators.includes(operatorInput)) {
                            console.log(chalk.red(`⚠️ Invalid operator. Must be one of: ${validOperators.join(', ')}`));
                            showReadMenu();
                            return;
                        }

                        rl.question(chalk.yellow('Enter value: '), (valueInput) => {
                            // If the value is empty, go back to Read Menu
                            if (valueInput === undefined || valueInput === '') {
                                console.log(chalk.red('⚠️ Value cannot be empty.'));
                                showReadMenu();
                                return;
                            }

                            // Call retrieveDocument recursively with user inputs if all fields are valid
                            retrieveDocumentByQuery(dbNameInput, colNameInput, fieldInput, operatorInput, valueInput);
                        });
                    });
                });
            });
        });
        return; // Exit early to await user input
    }

    // Check if daemon is active
    if (daemonActive) {
        // Format the query according to the expected structure
        // For comparison operators, we need { $field: field, [operator]: value }
        const queryObj = {
            "$field": field,
            [operator]: value
        };

        // Create the complete query structure
        const dataToSend = {
            data: {
                [dbName]: {
                    [collectionName]: queryObj
                }
            }
            //limit: 1,
            //pageNumber: 1
        };

        // Send the search command to the daemon
        client.emit('addCommand', 'search', dataToSend);

    } else {
        console.log(chalk.yellow('⚠️ No connection to Daemon.'));
        showConnectionFailureMenu();
    }
}

/**
 * Retrieve documents by regex pattern
 * 
 * @param {string} dbName - Database name
 * @param {string} collectionName - Collection name
 * @param {string} field - Field name to query on
 * @param {string} pattern - Regex pattern (e.g., "^John" for names starting with John)
 * @param {string} value - Value to match with pattern
 * @returns {void}
 */
export function retrieveDocumentsByRegex(dbName, collectionName, field, pattern) {
    if (!dbName || !collectionName || !field || !pattern) {
        console.log(chalk.yellow('⚠️ Please provide the required details to retrieve the document.'));
        rl.question(chalk.yellow('Enter database name: '), (dbNameInput) => {
            // If the database name is empty, go back to Read Menu
            if (!dbNameInput) {
                console.log(chalk.red('⚠️ Database name cannot be empty.'));
                showReadMenu();
                return;
            }

            rl.question(chalk.yellow('Enter collection name: '), (colNameInput) => {
                // If the collection name is empty, go back to Read Menu
                if (!colNameInput) {
                    console.log(chalk.red('⚠️ Collection name cannot be empty.'));
                    showReadMenu();
                    return;
                }

                rl.question(chalk.yellow('Enter field name: '), (fieldInput) => {
                    // If the field is empty, go back to Read Menu
                    if (!fieldInput) {
                        console.log(chalk.red('⚠️ Field name cannot be empty.'));
                        showReadMenu();
                        return;
                    }

                    const validPatterns = ["~", "$", "^",];
                    console.log(chalk.yellow(`Available pattern match operators: ${validPatterns.join(', ')}`));

                    rl.question(chalk.yellow('Enter operator: '), (operatorInput) => {
                        // Check if the operator is valid
                        if (!validPatterns.includes(operatorInput)) {
                            console.log(chalk.red(`⚠️ Invalid operator. Must be one of: ${validPatterns.join(', ')}`));
                            showReadMenu();
                            return;
                        }

                        rl.question(chalk.yellow('Enter value: '), (valueInput) => {
                            // If the value is empty, go back to Read Menu
                            if (valueInput === undefined || valueInput === '') {
                                console.log(chalk.red('⚠️ Value cannot be empty.'));
                                showReadMenu();
                                return;
                            }

                            // Call retrieveDocument recursively with user inputs if all fields are valid
                            retrieveDocumentByQuery(dbNameInput, colNameInput, fieldInput, operatorInput, valueInput);
                        });
                    });
                });
            });
        });
        return; // Exit early to await user input
    }

    // Check if daemon is active
    if (daemonActive) {
        // Format the query for regex search
        const queryObj = {
            "$field": field,
            [pattern]: {} // Empty object for the regex pattern
        };

        // Create the complete query structure
        const dataToSend = {
            [dbName]: {
                [collectionName]: queryObj
            }
        };

        // Send the search command to the daemon
        client.emit('addCommand', 'search', dataToSend);

    } else {
        console.log(chalk.yellow('⚠️ No connection to Daemon.'));
        showConnectionFailureMenu();
    }
}

/**
* Retrieves and displays all collections (tables) in a specified database
* 
* @param {string} [dbName] - Optional name of the database to retrieve collections from. If not provided, function prompts user for input.
* @returns {void}
* 
* @description
* This function retrieves and displays all collections (tables) within a specified database.
* If no database name is provided, it prompts the user to enter one. The function's behavior
* depends on whether the daemon is active:
* 
* - If daemon is active: Sends a list command to the daemon service to retrieve collections
* - If daemon is inactive: Retrieves collections from the local in-memory storage and displays them
* 
* The function handles cases where the specified database doesn't exist and provides appropriate feedback.
* When using the daemon, response handling is managed through the socket.io 'response' event handler.
**/

export function retrieveCollections(dbName) {
    if (!dbName) {
        rl.question(chalk.yellow('Enter database name: '), (dbName) => {
            if (!dbName) {
                console.log(chalk.red('⚠️ Database name required.'));
                showReadMenu();
                return;
            }
            if (daemonActive) {
                const dataToSend = { [dbName]: {} };
                client.emit('addCommand', 'list', dataToSend);
            } else {
                console.log(chalk.yellow('⚠️ No connection to Daemon.'));
                showConnectionFailureMenu();
            }
        });
    } else {
        if (daemonActive) {
            const dataToSend = { [dbName]: {} };
            client.emit('addCommand', 'list', dataToSend);
        } else {
            console.log(chalk.yellow('⚠️ No connection to Daemon.'));
            showConnectionFailureMenu();
        }
    }
}

/**
* Retrieves and displays all databases
* 
* @returns {void}
* 
* @description
* This function retrieves and displays all databases. The function's behavior
* depends on whether the daemon is active:
* 
* - If daemon is active: Sends a list command to the daemon service to retrieve collections
* - If daemon is inactive: Retrieves collections from the local in-memory storage and displays them
* 
* The function handles cases where no databases exist and provides appropriate feedback.
* When using the daemon, response handling is managed through the socket.io 'response' event handler.
**/
export function retrieveDatabases() {
    if (daemonActive) {
        client.emit('addCommand', 'list', '');
    } else {
        console.log(chalk.yellow('⚠️ No connection to Daemon.'));
        showConnectionFailureMenu();
    }
}

/**
 * @function updateDocument
 * @async
 * @param {string} [dbName] - Name of the database to update. If not provided, function prompts user for input.
 * @param {string} [collectionName] - Name of the collection containing the document. If not provided, function prompts user for input.
 * @param {string} [key] - Key of the document to update. If not provided, function prompts user for input.
 * @param {*} [newValue] - New value to assign to the document. If not provided, function prompts user for input.
 * @returns {void}
 * 
 * @description
 * Updates a document in the specified database collection. If any parameters are missing, 
 * the function prompts the user to provide them through an interactive CLI.
 * 
 * The function delegates the actual update operation to the processUpdate function
 * which handles the communication with the daemon service.
 */
export async function updateDocument(dbName, collectionName, key, newValue) {  //TODO
    if (!dbName || !collectionName || !key || !newValue) {
        rl.question(chalk.yellow('Enter database name: '), (dbName) => {
            rl.question(chalk.yellow('Enter collection name: '), (collectionName) => {
                rl.question(chalk.yellow('Enter key: '), (key) => {
                    rl.question(chalk.yellow('Enter new value: '), (newValue) => {
                        if (!dbName || !collectionName || !key || !newValue) {
                            console.log(chalk.red('⚠️ All fields are required.'));
                            return showMainMenu();
                        }
                        processUpdate(dbName, collectionName, key, newValue);
                    });
                });
            });
        });
    } else {
        processUpdate(dbName, collectionName, key, newValue);
    }
}

/**
 * @function renameCollection
 * @async
 * @param {string} [dbName] - Name of the database containing the collection. If not provided, function prompts user for input.
 * @param {string} [collectionName] - Name of the collection to rename. If not provided, function prompts user for input.
 * @param {string} [newName] - New name for the collection. If not provided, function prompts user for input.
 * @returns {void}
 * 
 * @description
 * Renames a collection within the specified database. If any parameters are missing,
 * the function prompts the user to provide them through an interactive CLI.
 * 
 * The function delegates the actual rename operation to the processRename function
 * which handles the communication with the daemon service.
 */
export async function renameCollection(dbName, collectionName, newName) {  //TODO
    if (!dbName || !collectionName || !newName) {
        rl.question(chalk.yellow('Enter database name: '), (dbName) => {
            rl.question(chalk.yellow('Enter collection name: '), (collectionName) => {
                rl.question(chalk.yellow('Enter new value: '), (newName) => {
                    if (!dbName || !collectionName || !newName) {
                        console.log(chalk.red('⚠️ All fields are required.'));
                        return showMainMenu();
                    }
                    const dataToSend = { [dbName]: { [`${collectionName}#reference`]: `${newName}` } };
                    client.emit('addCommand', 'update', dataToSend);
                });
            });
        });
    } else {
        const dataToSend = { [dbName]: { [`${collectionName}#reference`]: `${newName}` } };
        client.emit('addCommand', 'update', dataToSend);
    }
}

/**
 * @function renameDatabase
 * @async
 * @param {string} [dbName] - Name of the database to rename. If not provided, function prompts user for input.
 * @param {string} [newName] - New name for the database. If not provided, function prompts user for input.
 * @returns {void}
 * 
 * @description
 * Renames a database. If any parameters are missing, the function prompts the user
 * to provide them through an interactive CLI.
 * 
 * The function delegates the actual rename operation to the processUpdate function
 * which handles the communication with the daemon service.
 */
export async function renameDatabase(dbName, newName) {
    if (!dbName || !newName) {
        rl.question(chalk.yellow('Enter database name: '), (dbName) => {
            rl.question(chalk.yellow('Enter new value: '), (newName) => {
                if (!dbName || !newName) {
                    console.log(chalk.red('⚠️ All fields are required.'));
                    return showUpdateMenu();
                }
                const dataToSend = { [`${dbName}#reference`] : `${newName}` };
                client.emit('addCommand', 'update', dataToSend);
            });
        });
    } else {
        const dataToSend = { [`${dbName}#reference`] : [`${newName}`] };
        client.emit('addCommand', 'update', dataToSend);
    }
}

/**
 * @function processUpdate
 * @private
 * @param {string} dbName - Name of the database to update.
 * @param {string} collectionName - Name of the collection containing the document.
 * @param {string} key - Key of the document to update.
 * @param {*} newValue - New value to assign to the document.
 * @returns {void}
 * 
 * @description
 * Processes a document update request. This function logs the update attempt, 
 * converts the new value to an appropriate type, and communicates with the daemon
 * service if active. If there's no connection to the daemon, it shows a connection
 * failure menu.
 */
function processUpdate(dbName, collectionName, key, newValue) {
    //console.log(chalk.blue(`Attempting to update document in ${dbName}.${collectionName}: ${key} = ${newValue}`));
    const updateValue = detectAndConvertType(newValue);

    if (daemonActive) {
        const dataToSend = { [dbName]: { [collectionName]: { [key]: [updateValue] } } };
        client.emit('addCommand', 'update', dataToSend);
    } else {
        console.log(chalk.yellow('⚠️ No connection to Daemon.'));
        showConnectionFailureMenu();
    }
}

/**
 * @function processRename
 * @private
 * @param {string} newName - New name for the database or collection.
 * @param {string} dbName - Name of the database.
 * @param {string} [colName] - Name of the collection (if renaming a collection).
 * @returns {void}
 * 
 * @description
 * Processes a rename request for either a database or a collection. This function
 * logs the rename attempt and communicates with the daemon service if active.
 * If there's no connection to the daemon, it shows a connection failure menu.
 * 
 * The function handles two scenarios:
 * 1. Rename a collection (when dbName and colName are provided)
 * 2. Rename a database (when only dbName is provided)
 */
function processRename(newName, dbName, colName) {
    if (dbName && colName) {
        console.log(chalk.blue(`Attempting to rename collection in ${colName} in ${dbName} to: ${newName}`));
    } else {
        console.log(chalk.blue(`Attempting to rename database ${dbName} to: ${newName}`));
    }

    if (daemonActive) {
        if (dbName && colName) {
            const dataToSend = { [`${dbName}#reference`]: [newName] };
            client.emit('addCommand', 'update', dataToSend);
        } else {
            const dataToSend = { [dbName]: { [`${colName}#reference`]: [newName] } };
            client.emit('addCommand', 'update', dataToSend);
        }

    } else {
        console.log(chalk.yellow('⚠️ No connection to Daemon.'));
        showConnectionFailureMenu();
    }
}

/**
 * @function deleteDocument
 * @param {string} [dbName] - Name of the database containing the document. If not provided, function prompts user for input.
 * @param {string} [collectionName] - Name of the collection containing the document. If not provided, function prompts user for input.
 * @param {string} [key] - Key of the document to delete. If not provided, function prompts user for input.
 * @returns {void}
 * 
 * @description
 * Deletes a document in either the daemon or in-memory storage. If any parameters are missing,
 * the function prompts the user to provide them through an interactive CLI.
 * 
 * The function behaves differently based on whether the daemon is active:
 * - If daemon is active: Sends a deletion command to the daemon service
 * - If daemon is inactive: Shows connection failure menu
 * 
 * When using the daemon, response handling is managed through the socket.io 'response' event handler.
 */
export function deleteDocument(dbName, collectionName, key) {
    if (!dbName || !collectionName || !key) {
        rl.question(chalk.yellow('Enter database name: '), (dbName) => {
            rl.question(chalk.yellow('Enter collection name: '), (collectionName) => {
                rl.question(chalk.yellow('Enter key: '), (key) => {
                    if (!dbName || !collectionName || !key) {
                        console.log(chalk.red('⚠️ All fields are required.'));
                        showDeleteMenu();
                        return;
                    }
                    if (daemonActive) {
                        const dataToSend = { [dbName]: { [collectionName]: { [key]: [] } } }  //TODO
                        client.emit('addCommand', 'delete', dataToSend);
                    } else {
                        console.log(chalk.yellow('⚠️ No connection to Daemon.'));
                        showConnectionFailureMenu();
                    }
                });
            });
        });
    } else {
        if (daemonActive) {
            const dataToSend = { [dbName]: { [collectionName]: { [key]: [] } } }  //TODO
            client.emit('addCommand', 'delete', dataToSend);
        } else {
            console.log(chalk.yellow('⚠️ No connection to Daemon.'));
            showConnectionFailureMenu();
        }
    }
}


/**
* Deletes a Database in either the daemon or in-memory storage
* 
* @param {string} [dbName] - Optional name of the database to delete. If not provided, function prompts user for input.
* @returns {void}
* 
* @description
* This function deletes a database. If no parameters are provided, it prompts the user
 * to enter a them. The function behaves differently based on whether the daemon is active:
* 
* - If daemon is active: Sends a deletion command to the daemon service
* - If daemon is inactive: Deletes the database in the local in-memory storage
* 
* The function handles cases where no databases exist and provides appropriate feedback.
* When using the daemon, response handling is managed through the socket.io 'response' event handler.
**/

export function deleteDatabase(dbName) {
    if (!dbName ) {
        rl.question(chalk.yellow('Enter database name: '), (dbName) => {
            if (!dbName) {
                console.log(chalk.red('⚠️ All fields are required.'));
                showMainMenu();
                return;
            }
            if (daemonActive) {
                const dataToSend = { [dbName]: {} };
                client.emit('addCommand', 'delete', dataToSend);
            } else {
                console.log(chalk.yellow('⚠️ No connection to Daemon.'));
                showConnectionFailureMenu();
            }
        });
    } else {
        if (daemonActive) {
            const dataToSend = { [dbName]: {} };
            client.emit('addCommand', 'delete', dataToSend);
        } else {
            console.log(chalk.yellow('⚠️ No connection to Daemon.'));
            showConnectionFailureMenu();
        }
    }
}

/**
* Deletes a Database in either the daemon or in-memory storage
* 
* @param {string} [dbName] - Optional name of the database that contains the collection. If not provided, function prompts user for input.
* @param {string} [collectionName] - Optional name of the collection to delete. If not provided, function prompts user for input.
* @returns {void}
* 
* @description
* This function deletes a collection. If no parameters are provided, it prompts the user
 * to enter a them. The function behaves differently based on whether the daemon is active:
* 
* - If daemon is active: Sends a deletion command to the daemon service
* - If daemon is inactive: Deletes the collection in the local in-memory storage
* 
* The function handles cases where no such collection exists and provides appropriate feedback.
* When using the daemon, response handling is managed through the socket.io 'response' event handler.
**/

export function deleteCollection(dbName, collectionName) {
    if (!dbName || !collectionName) {
        rl.question(chalk.yellow('Enter database name: '), (dbName) => {
            rl.question(chalk.yellow('Enter collection name: '), (collectionName) => {
                if (!dbName || !collectionName) {
                    console.log(chalk.red('⚠️ All fields are required.'));
                    showDeleteMenu();
                    return;
                }
                if (daemonActive) {
                    const dataToSend = { [dbName]: { [collectionName]: {} } };
                    client.emit('addCommand', 'delete', dataToSend);
                } else {
                    console.log(chalk.yellow('⚠️ No connection to Daemon.'));
                    showConnectionFailureMenu();

                }
            });
        });
    } else {
        if (daemonActive) {
            const dataToSend = { [dbName]: { [collectionName]: {} } };
            client.emit('addCommand', 'delete', dataToSend);
        } else {
            console.log(chalk.yellow('⚠️ No connection to Daemon.'));
            showConnectionFailureMenu();
        }
    }
}

/**
 * @brief Shows an overview of all databases and their collections
 * 
 * When connected to the daemon, retrieves and displays a hierarchical view of all databases
 * and their collections.
 * 
 * @returns {void}
 */

export function showAll() {  //TODO
    if (daemonActive) {

        console.log(chalk.blue.bold('\n📂 Databases Overview\n'));
showMainMenu();
    }
    else {
        console.log(chalk.yellow('⚠️ No connection to Daemon.'));
        showConnectionFailureMenu();
    }
}

/**
 * @brief Displays the export/import menu with options
 * 
 * Provides options for exporting and importing databases
 */

function showExportImportMenu(){
    const menuText = `
1. Export Database
2. Import Database
3. Back to Main Menu
    `;
    layout(menuText, '📦 Export/Import Menu:', 'cyan');
    rl.question(chalk.yellow('Select an option: '), (option) => {
        switch (option) {
            case '1':
                exportDatabase();
                break;
            case '2':
                importDatabase();
                break;
            case '3':
                showMainMenu();
                break;
            default:
                console.log(chalk.red('⚠️ Invalid option. Please try again.'));
                showExportImportMenu();
        }
    });
}

/**
 * @brief Exports a database to a file
 * 
 * @returns {void}
 */
function exportDatabase(){
    rl.question(chalk.yellow('Enter database name to export: '), (dbName) => {
        if (!dbName) {
            console.log(chalk.red('⚠️ Database name cannot be empty.'));
            showExportImportMenu();
            return;
        }
        if (daemonActive) {
            client.emit('export', `${dbName}`);
            setTimeout(showExportImportMenu, 2000);
        } else {
            console.log(chalk.yellow('⚠️ No connection to Daemon.'));
            showConnectionFailureMenu();
        }
    });
}
/**
 * @brief Imports a database from a file or data string
 * 
 * @param {string} [dbName] - Optional database name to import into
 * @param {string} [data] - Optional data to import
 * @returns {void}
 */

function importDatabase(dbName, data){
    if (!dbName || !data) {
        rl.question(chalk.yellow('Enter database name to import: '), (dbName) => {
            rl.question(chalk.yellow('Enter data to import: '), (data) => {
                if (!dbName || !data) {
                    console.log(chalk.red('⚠️ Database name and data cannot be empty.'));
                    showExportImportMenu();
                    return;
                }
                if (daemonActive) {
                    client.emit('import', `${dbName}`);
                    showExportImportMenu();
                } else {
                    console.log(chalk.yellow('⚠️ No connection to Daemon.'));
                    showConnectionFailureMenu();
                }
            });
        });
    } else {
        if (daemonActive) {
            const dataToSend = { [dbName]: { data } };
            client.emit('addCommand', 'import', dataToSend);
        } else {
            console.log(chalk.yellow('⚠️ No connection to Daemon.'));
            showConnectionFailureMenu();
        }
    }
}

/**
 * @brief Safely exits the application
 * 
 * Closes connections, stops event listeners, and exits the process
 * 
 * @returns {void}
 */
function exit() {
    if (isExiting) return;
    isExiting = true;
    
    console.log(chalk.red('\n🚪 Exiting CQL CLI...'));
    
    // Remove disconnect event listener to prevent it from being called
    // after we've already started the exit process
    client.off('disconnect');
    
    // Close the readline interface
    rl.close();
    
    // Then disconnect the client
    if (client.connected) {
        client.disconnect();
    }
    
    // Set a timeout to ensure we exit even if something hangs
    setTimeout(() => {
        process.exit(0);
    }, 500);
}

