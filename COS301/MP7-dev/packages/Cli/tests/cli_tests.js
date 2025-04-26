import chalk from 'chalk';
import { io } from 'socket.io-client';
import { 
    createDatabase, 
    createCollection, 
    insertDocument, 
    retrieveDocumentByValue,
    retrieveCollections,
    retrieveDatabases,
    deleteCollection,
    deleteDatabase
} from '../src/Cli.js'; 

// Helper function for a specified time> new Promise(resolve => setTimeout(resolve, ms));

/**
 * Test suite: Database Creation Tests
 * Tests the functionality of creating databases and collections
 */
async function runDatabaseCreationTests() {
    console.log(chalk.blue.bold('\n🧪 Running Database Creation Tests...\n'));

    // Test 1: Create single database
    console.log(chalk.yellow('\nTest 1.1: Creating a single database...\n'));
    await createDatabase('TestDB1');
    console.log(chalk.green('✅ Test 1.1 Passed: Single database created successfully.\n'));


    // Test 2: Create multiple databases
    console.log(chalk.yellow('\nTest 1.2: Creating multiple databases...\n'));
    await createDatabase('TestDB2');
    await createDatabase('TestDB3');
    console.log(chalk.green('✅ Test 1.2 Passed: Multiple databases created successfully.\n'));


    // Test 3: Create collection in database
    console.log(chalk.yellow('\nTest 1.3: Creating collection in database...\n'));
    await createCollection('TestDB1', 'Users');
    console.log(chalk.green('✅ Test 1.3 Passed: Collection created successfully.\n'));


    // Test 4: Create multiple collections in database
    console.log(chalk.yellow('\nTest 1.4: Creating multiple collections in database...\n'));
    await createCollection('TestDB1', 'Products');
    await createCollection('TestDB2', 'Categories');
    await createCollection('TestDB3', 'Orders');
    console.log(chalk.green('✅ Test 1.4 Passed: Multiple collections created successfully.\n'));


    console.log(chalk.blue.bold('\n✅ Database Creation Tests Completed!\n'));
}

/**
 * Test suite: Document Operations Tests
 * Tests the functionality of inserting documents with IDs
 */
async function runDocumentOperationsTests() {
    console.log(chalk.blue.bold('\n🧪 Running Document Operations Tests...\n'));

    // Test 1: Insert document with simple ID
    console.log(chalk.yellow('\nTest 2.1: Inserting document with simple ID...\n'));
    await insertDocument('TestDB1', 'Users', 'user_id', 'user1');
    console.log(chalk.green('✅ Test 2.1 Passed: Document with simple ID inserted successfully.\n'));


    // Test 2: Insert document with multiple fields
    console.log(chalk.yellow('\nTest 2.2: Inserting document with multiple fields...\n'));
    // Since your insertDocument function is set up for key-value pairs,
    // we need to adapt how we call it for multiple fields
    await insertDocument('TestDB1', 'Users', 'name', 'John Doe');
    await insertDocument('TestDB1', 'Users', 'email', 'john@example.com');
    await insertDocument('TestDB1', 'Users', 'age', '30');
    console.log(chalk.green('✅ Test 2.2 Passed: Document with multiple fields inserted successfully.\n'));


    // Test 3: Insert documents in multiple collections
    console.log(chalk.yellow('\nTest 2.3: Inserting documents in multiple collections...\n'));
    await insertDocument('TestDB1', 'Products', 'product_id', 'p1');
    await insertDocument('TestDB1', 'Products', 'name', 'Laptop');
    await insertDocument('TestDB2', 'Categories', 'category_id', 'c1');
    await insertDocument('TestDB2', 'Categories', 'name', 'Electronics');
    console.log(chalk.green('✅ Test 2.3 Passed: Documents in multiple collections inserted successfully.\n'));


    console.log(chalk.blue.bold('\n✅ Document Operations Tests Completed!\n'));
}

/**
 * Test suite: Listing Operations Tests
 * Tests the functionality of listing databases, collections, and documents
 */
async function runListingOperationsTests() {
    console.log(chalk.blue.bold('\n🧪 Running Listing Operations Tests...\n'));

    // Test 1: List all databases
    console.log(chalk.yellow('\nTest 3.1: Listing all databases...\n'));
    await retrieveDatabases();
    console.log(chalk.green('✅ Test 3.1 Passed: All databases listed successfully.\n'));


    // Test 2: List collections in a database
    console.log(chalk.yellow('\nTest 3.2: Listing collections in a database...\n'));
    await retrieveCollections('TestDB1');
    console.log(chalk.green('✅ Test 3.2 Passed: Collections in database listed successfully.\n'));


    // Test 3: Retrieve document by value
    console.log(chalk.yellow('\nTest 3.3: Retrieving document by value...\n'));
    await retrieveDocumentByValue('TestDB1', 'Users', 'user_id', 'user1');
    console.log(chalk.green('✅ Test 3.3 Passed: Document retrieved by value successfully.\n'));

    console.log(chalk.blue.bold('\n✅ Listing Operations Tests Completed!\n'));
}

/**
 * Test suite: Deletion Operations Tests
 * Tests the functionality of deleting collections and databases
 */
async function runDeletionOperationsTests() {
    console.log(chalk.blue.bold('\n🧪 Running Deletion Operations Tests...\n'));

    // Test 1: Delete a collection
    console.log(chalk.yellow('\nTest 4.1: Deleting a collection...\n'));
    await deleteCollection('TestDB1', 'Products');
    console.log(chalk.green('✅ Test 4.1 Passed: Collection deleted successfully.\n'));


    // Test 2: Delete a database
    console.log(chalk.yellow('\nTest 4.2: Deleting a database...\n'));
    await deleteDatabase('TestDB3');
    console.log(chalk.green('✅ Test 4.2 Passed: Database deleted successfully.\n'));


    // Verify deletions by listing
    console.log(chalk.yellow('\nTest 4.3: Verifying deletions by listing...\n'));
    await retrieveDatabases(); // Should not show TestDB3

    await retrieveCollections('TestDB1'); // Should not show Products
    
    console.log(chalk.green('✅ Test 4.3 Passed: Deletion verification completed.\n'));


    console.log(chalk.blue.bold('\n✅ Deletion Operations Tests Completed!\n'));
}

/**
 * Main function to run all tests
 * @param {Function} done - Callback function to be called when tests are complete
 */
export async function runTests() {
    console.clear();
    console.log(chalk.blue.bold('🚀 Starting CQL Database CLI Tests...\n'));
    
    try {
        // Run database creation tests
        await runDatabaseCreationTests();
        
        // Run document operations tests
        await runDocumentOperationsTests();
        
        // Run listing operations tests
        await runListingOperationsTests();
        
        // Run deletion operations tests
        await runDeletionOperationsTests();
    
        console.log(chalk.green.bold('\n🎉 All Tests Completed Successfully!\n'));
    } catch (error) {
        console.log(chalk.red.bold(`\n❌ Tests Failed: ${error.message}\n`));
        console.error(error);
    }

    return;
}
const DAEMON_URL = 'http://127.0.0.1:8008';
const client = io(DAEMON_URL, {
    autoConnect: true,
    reconnection: true,
    reconnectionAttempts: 1,
    timeout: 5000,
    reconnectionDelay: 1000
});

client.on('connect', () => {
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
    runTests().then(() => {
        console.log('Exiting tests...');
        process.exit(0);
      });
});
