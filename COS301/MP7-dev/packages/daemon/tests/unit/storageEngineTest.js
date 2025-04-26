// NOTE!!! to see changes made by this test script, comment out the cleanUp function
// at the end of the runTest function 

import StorageEngine from "../../src/StorageEngine.js";
import fs from "fs/promises";
import path from "path";

let passed = 0;
let failed = 0;

async function cleanUp() {
    try {
      // Make sure the data directory exists before trying to clean it
      await fs.access(StorageEngine.dataDir).catch(() => {
        return;
      });
      
      // Get all directories in the /data
      const entries = await fs.readdir(StorageEngine.dataDir);
      
      for (const entry of entries) {
        if (entry.startsWith("test_")) {
          await fs.rm(path.join(StorageEngine.dataDir, entry), { recursive: true, force: true });
        }
      }
      
      await fs.rm(path.join(StorageEngine.dataDir, "test_db"), { recursive: true, force: true }).catch(() => {});
      await fs.rm(path.join(StorageEngine.dataDir, "test_db2"), { recursive: true, force: true }).catch(() => {});
    } catch (err) {
      console.log("Cleanup warning (non-fatal):", err.message);
      // Continue execution even if cleanup fails
    }
}

function logResult(testName, success, message = "") {
    if (success) {
        console.log(`\x1b[32m✓ PASS: ${testName}\x1b[0m`);
        passed++;
    } else {
        console.log(`\x1b[31m✗ FAIL: ${testName}${message ? ` - ${message}` : ""}\x1b[0m`);
        failed++;
    }
}

// function logSearchResult(testName, passed, message, data) {
//     console.log(`${testName}: ${passed ? "PASSED" : "FAILED"} - ${message}`);
//     if (data) {
//       console.log("Result data:", JSON.stringify(data, null, 2));
//     }
// }

//Creation Tests
async function runTest1() {
    try {
        await cleanUp();

        // Test 1: Initialize
        await StorageEngine.start();
        logResult("Test 1: Storage Engine Initialization", true);
      
        // Test 2: Create database
        try {
            const result = await StorageEngine.create("test_db");
            const dbExists = await fs.access(path.join(StorageEngine.dataDir, "test_db"))
              .then(() => true)
              .catch(() => false);
          
            logResult("Test 2: Create database only", 
              result.status === "success" && 
              result.created.includes("database") && 
              dbExists, 
              "Database was not created or result incorrect");
        } catch (err) {
            logResult("Test 2: Create database only", false, err.message);
        }
      
        // Test 3: Create database and collection
        try {
            const result = await StorageEngine.create("test_db", "users");
            const colExists = await fs.access(path.join(StorageEngine.dataDir, "test_db", "users"))
              .then(() => true)
              .catch(() => false);
          
            logResult("Test 3: Create database and collection", 
              result.status === "success" && 
              result.created.includes("collection") && 
              colExists, 
              "Collection was not created or result incorrect");
        } catch (err) {
            logResult("Test 3: Create database and collection", false, err.message);
        }
      
        // Test 4: Create document with auto-generated ID
        try {
            const testDoc = { name: "Test User", email: "test@example.com" };
            const result = await StorageEngine.create("test_db", "users", testDoc, "Bob");
        
            logResult("Test 4: Create document with auto-generated ID", 
              result.status === "success" && 
              result.created.includes("document") && 
              result.message.includes("Document with ID"), 
              "Document was not created or result incorrect");
        } catch (err) {
            logResult("Test 4: Create document with auto-generated ID", false, err.message);
        }
      
        // // Test 5: Create document with custom ID
        // try {
        //     const testDoc = { id: "custom123", name: "Custom ID User", email: "custom@example.com" };
        //     const result = await StorageEngine.create("test_db", "users", testDoc, "Sam");
        //     const docExists = await fs.access(path.join(StorageEngine.dataDir, "test_db", "users", "custom123$Sam.json"))
        //       .then(() => true)
        //       .catch(() => false);
          
        //     logResult("Test 5: Create document with custom ID", 
        //       result.status === "success" && 
        //       result.created.includes("document") && 
        //       docExists, 
        //       "Document with custom ID was not created");
        // } catch (err) {
        //     logResult("Test 5: Create document with custom ID", false, err.message);
        // }
      
        // // Test 6: Create document with existing ID (should fail)
        // try {
        //     const testDoc = { id: "custom123", name: "Duplicate", email: "duplicate@example.com" };
        //     await StorageEngine.create("test_db", "users", testDoc, "Sam");
        //     logResult("Test 6: Prevent duplicate document creation", false, "Should have thrown an error for duplicate ID");
        // } catch (err) {
        //     logResult("Test 6: Prevent duplicate document creation", 
        //       err.message.includes("already exists"), 
        //       "Wrong error message");
        // }
      
        // Test 7: Create document without collection (should fail)
        try {
            const testDoc = { name: "No Collection", email: "nocollection@example.com" };
            await StorageEngine.create("test_db", null, testDoc, "Tim");
            logResult("Test 7: Prevent document without collection", false, "Should have thrown an error");
        } catch (err) {
            logResult("Test 7: Prevent document without collection", 
              err.message.includes("no Collection"), 
              "Wrong error message");
        }
      
        // Test 8: Create new collection with existing db
        try {
            const result = await StorageEngine.create("test_db", "posts");
            const colExists = await fs.access(path.join(StorageEngine.dataDir, "test_db", "posts"))
              .then(() => true)
              .catch(() => false);
          
            logResult("Test 8: Use existing database with new collection", 
              result.status === "success" && 
              result.created.includes("collection") && 
              !result.created.includes("database") && 
              colExists, 
              "New collection in existing db not created properly");
        } catch (err) {
            logResult("Test 8: Use existing database with new collection", false, err.message);
        }
      
        // Test 9: Create second database
        try {
            const result = await StorageEngine.create("test_db2");
            const dbExists = await fs.access(path.join(StorageEngine.dataDir, "test_db2"))
              .then(() => true)
              .catch(() => false);
          
            logResult("Test 9: Multi-database support", 
              result.status === "success" && 
              result.created.includes("database") && 
              dbExists, 
              "Second database was not created");
        } catch (err) {
            logResult("Test 9: Multi-database support", false, err.message);
        }
      
        // Test 10: Create database with invalid name (Should fail)
        try {
            await StorageEngine.create(null);
            logResult("Test 10: Validate database name", false, "Should have rejected null database name");
        } catch (err) {
            logResult("Test 10: Validate database name", 
              err.message.includes("No valid database name"), 
              "Wrong error message"
            );
        }
      
        // Test 11: Create collection with invalid name (Should fail)
        try {
            await StorageEngine.create("test_db", 123);
            logResult("Test 11: Validate collection name", false, "Should have rejected non-string collection name");
        } catch (err) {
            logResult("Test 11: Validate collection name", 
              err.message.includes("Collection name must be a string"), 
              "Wrong error message"
            );
        }
      
        // Clean up test data. COMMENT OUT TO VIEW CHANGES
         await cleanUp();
      
    } catch (err) {
        console.error("Test suite error:", err);
        failed++;
    }
}

//List Tests
async function runTest2() {
    try {
        await cleanUp();
        await StorageEngine.start();

        // Setup: Create test databases and collections with documents
        await StorageEngine.create("test_db");
        await StorageEngine.create("test_db2");
        
        // Create collections in test_db
        const collectionPath1 = path.join(StorageEngine.dataDir, "test_db", "users");
        const collectionPath2 = path.join(StorageEngine.dataDir, "test_db", "posts");
        const collectionPath3 = path.join(StorageEngine.dataDir, "test_db2", "products");
        const emptyCollectionPath = path.join(StorageEngine.dataDir, "test_db", "empty");
        
        await fs.mkdir(collectionPath1, { recursive: true });
        await fs.mkdir(collectionPath2, { recursive: true });
        await fs.mkdir(collectionPath3, { recursive: true });
        await fs.mkdir(emptyCollectionPath, { recursive: true });
        
        // Add some test documents
        await fs.writeFile(
            path.join(collectionPath1, "user1.json"),
            JSON.stringify({ name: "User 1", email: "user1@example.com" }),
            "utf8"
        );
        await fs.writeFile(
            path.join(collectionPath1, "user2.json"),
            JSON.stringify({ name: "User 2", email: "user2@example.com" }),
            "utf8"
        );
        await fs.writeFile(
            path.join(collectionPath2, "post1.json"),
            JSON.stringify({ title: "Post 1", content: "Content 1" }),
            "utf8"
        );
        await fs.writeFile(
            path.join(collectionPath3, "product1.json"),
            JSON.stringify({ name: "Product 1", price: 99.99 }),
            "utf8"
        );

        // Also create a non-JSON file
        await fs.writeFile(
            path.join(collectionPath1, "not-json.txt"), 
            "This is not a JSON file", 
            "utf8"
        );

        // Test 1: List all databases
        try {
            const databases = await StorageEngine.list();
            logResult("Test 1: List all databases", 
                Array.isArray(databases) && 
                databases.includes("test_db") && 
                databases.includes("test_db2") &&
                databases.length >= 2,
                "Should contain at least test_db and test_db2"
            );
        } catch (err) {
            logResult("Test 1: List all databases", false, err.message);
        }

        // Test 2: List non-existent database directory
        try {
            // Temporarily save and modify the data directory
            const originalDataDir = StorageEngine.dataDir;
            StorageEngine.dataDir = path.join(originalDataDir, "nonexistent");
            
            const databases = await StorageEngine.list();
            StorageEngine.dataDir = originalDataDir; // Restore the original data directory
            
            logResult("Test 2: Handle non-existent database directory", 
                Array.isArray(databases) && databases.length === 0,
                "Should return an empty array"
            );
        } catch (err) {
            logResult("Test 2: Handle non-existent database directory", false, err.message);
        }

        // Test 3: List collections in a database
        try {
            const collections = await StorageEngine.list("test_db");
            logResult("Test 3: List collections in a database", 
                Array.isArray(collections) && 
                collections.includes("users") && 
                collections.includes("posts") &&
                collections.includes("empty") &&
                collections.length === 3,
                "Should contain users, posts, and empty collections"
            );
        } catch (err) {
            logResult("Test 3: List collections in a database", false, err.message);
        }

        // Test 4: List collections in non-existent database
        try {
            const collections = await StorageEngine.list("nonexistent_db");
            logResult("Test 4: Handle non-existent database for collections", 
                Array.isArray(collections) && collections.length === 0,
                "Should return an empty array"
            );
        } catch (err) {
            logResult("Test 4: Handle non-existent database for collections", false, err.message);
        }

        // Test 5: List documents in a collection
        try {
            const userDocs = await StorageEngine.list("test_db", "users");
            logResult("Test 5: List documents in a collection", 
                Array.isArray(userDocs) && 
                userDocs.length === 2 &&
                userDocs.some(doc => doc.name === "User 1") &&
                userDocs.some(doc => doc.name === "User 2"),
                "Should contain two user documents"
            );
        } catch (err) {
            logResult("Test 5: List documents in a collection", false, err.message);
        }

        // Test 6: List documents from non-existent collection path
        try {
            const emptyDocs = await StorageEngine.list("test_db","nonexistent");
            logResult("Test 6: Handle non-existent collection path", 
                Array.isArray(emptyDocs) && emptyDocs.length === 0,
                "Should return an empty array"
            );
        } catch (err) {
            logResult("Test 6: Handle non-existent collection path", false, err.message);
        }

        // Test 7: List documents from other collections
        try {
            const postDocs = await StorageEngine.list("test_db", "posts");
            logResult("Test 7: List documents in a different collection", 
                Array.isArray(postDocs) && 
                postDocs.length === 1 &&
                postDocs[0].title === "Post 1",
                "Should contain one post document"
            );
        } catch (err) {
            logResult("Test 7: List documents in a different collection", false, err.message);
        }

        // Test 8: List documents from empty collection
        try {
            const emptyDocs = await StorageEngine.list("test_db","empty");
            logResult("Test 8: Handle empty collection", 
                Array.isArray(emptyDocs) && emptyDocs.length === 0,
                "Should return an empty array"
            );
        } catch (err) {
            logResult("Test 8: Handle empty collection",
                err.message.includes("Database name, Collection name and document ID are required"), 
              "Wrong error message");
        }

        // Clean up test data. UNCOMMENT TO CLEAN UP
        await cleanUp();
        
    } catch (err) {
        console.error("Test suite error:", err);
        failed++;
    }
}

//Deletion Tests
async function runTest3() {
    try {
        await cleanUp();
        await StorageEngine.start();

        // Setup: Create test databases and collections with documents
        await StorageEngine.create("test_db");
        await StorageEngine.create("test_db2");
        
        // Create collections in test_db
        const collectionPath1 = path.join(StorageEngine.dataDir, "test_db", "users");
        const collectionPath2 = path.join(StorageEngine.dataDir, "test_db", "posts");
        const collectionPath3 = path.join(StorageEngine.dataDir, "test_db2", "products");
        const emptyCollectionPath = path.join(StorageEngine.dataDir, "test_db", "empty");
        
        await fs.mkdir(collectionPath1, { recursive: true });
        await fs.mkdir(collectionPath2, { recursive: true });
        await fs.mkdir(collectionPath3, { recursive: true });
        await fs.mkdir(emptyCollectionPath, { recursive: true });

        // Add some test documents
        await fs.writeFile(
            path.join(collectionPath1, "user1.json"),
            JSON.stringify({ name: "User 1", email: "user1@example.com" }),
            "utf8"
        );
        await fs.writeFile(
            path.join(collectionPath1, "user2.json"),
            JSON.stringify({ name: "User 2", email: "user2@example.com" }),
            "utf8"
        );
        await fs.writeFile(
            path.join(collectionPath2, "post1.json"),
            JSON.stringify({ title: "Post 1", content: "Content 1" }),
            "utf8"
        );
        await fs.writeFile(
            path.join(collectionPath3, "product1.json"),
            JSON.stringify({ name: "Product 1", price: 99.99 }),
            "utf8"
        );

        // Test 1: Delete a document
        try {
            const result = await StorageEngine.delete("test_db", "users", "user1");
            logResult("Test 1: Delete document", 
                result && 
                typeof result === 'object' &&
                result.deleted === true &&
                result.documentId === "user1",
                "Should return an object with {deleted: true}"
            );
        } catch (err) {
            logResult("Test 1: Delete document", false, err.message);
        }

        // Test 2: Delete an empty collection
        try {
            const result = await StorageEngine.delete("test_db", "empty");
            logResult("Test 2: Delete empty collection", 
                result && 
                typeof result === 'object' &&
                result.deleted === true &&
                result.collectionName === "empty",
                "Should return an object with {deleted: true}"
            );
        } catch (err) {
            logResult("Test 2: Delete empty collection", false, err.message);
        }

        // Test 3: Delete an non-empty collection
        try {
            const result = await StorageEngine.delete("test_db", "users");
            logResult("Test 3: Delete an non-empty collection", 
                result && 
                typeof result === 'object' &&
                result.deleted === true &&
                result.collectionName === "users",
                "Should return an object with {deleted: true}"
            );
        } catch (err) {
            logResult("Test 3: Delete an non-empty collection", false, err.message);
        }

        // Test 4: Delete a database
        try {
            const result = await StorageEngine.delete("test_db2");
            logResult("Test 4: Delete a database", 
                result && 
                typeof result === 'object' &&
                result.deleted === true &&
                result.databaseName === "test_db2",
                "Should return an object with {deleted: true}"
            );
        } catch (err) {
            logResult("Test 4: Delete a database", false, err.message);
        }

        // Test 5: No paramters provided (Should fail)
        try {
            await StorageEngine.delete();
            logResult("Test 5: No parameters provided", false,"Should have thrown error");
        } catch (err) {
            logResult("Test 5: No parameters provided", true, err.message);
        }

        // Clean up test data. COMMENT OUT TO VIEW CHANGES
        await cleanUp();
        
    } catch (err) {
        console.error("Test suite error:", err);
        failed++;
    }
}

//Read Tests
async function runTest4() {
    try {
        await cleanUp();
        await StorageEngine.start();

        // Setup: Create test databases and collections with documents
        await StorageEngine.create("test_db");
        await StorageEngine.create("test_db2");
        
        // Create collections in test_db
        const collectionPath1 = path.join(StorageEngine.dataDir, "test_db", "users");
        const collectionPath2 = path.join(StorageEngine.dataDir, "test_db", "posts");
        const collectionPath3 = path.join(StorageEngine.dataDir, "test_db2", "products");
        const emptyCollectionPath = path.join(StorageEngine.dataDir, "test_db", "empty");
        
        await fs.mkdir(collectionPath1, { recursive: true });
        await fs.mkdir(collectionPath2, { recursive: true });
        await fs.mkdir(collectionPath3, { recursive: true });
        await fs.mkdir(emptyCollectionPath, { recursive: true });

        // Add some test documents
        await fs.writeFile(
            path.join(collectionPath1, "user1.json"),
            JSON.stringify({ id: "user1", name: "User 1", email: "user1@example.com" }),
            "utf8"
        );
        await fs.writeFile(
            path.join(collectionPath1, "user2.json"),
            JSON.stringify({ id: "user2", name: "User 2", email: "user2@example.com" }),
            "utf8"
        );
        await fs.writeFile(
            path.join(collectionPath2, "post1.json"),
            JSON.stringify({ id: "post1", title: "Post 1", content: "Content 1" }),
            "utf8"
        );
        await fs.writeFile(
            path.join(collectionPath3, "product1.json"),
            JSON.stringify({ id: "product1", name: "Product 1", price: 99.99 }),
            "utf8"
        );

        // Test 1: Read a document
        try {
            const result = await StorageEngine.read("test_db", "users", "user1");
            logResult("Test 1: Read document", 
                result && 
                typeof result === 'object' &&
                result.id === "user1" &&
                result.name === "User 1" &&
                result.email === "user1@example.com",
                "Should return a json object"
            );
        } catch (err) {
            logResult("Test 1: Read document", false, err.message);
        }

        // Test 2: Read nonexistent document (Should fail)
        try {
            await StorageEngine.read("test_db", "users", "user3");
            logResult("Test 2: Read nonexistent document", false,"Should have thrown error");
        } catch (err) {
            logResult("Test 2: Read nonexistent document", 
                err.message.includes("Document does not exist"), 
                "Wrong error message"
            );
        }

        // Test 3: Read from incorrect collection (Should fail)
        try {
            await StorageEngine.read("test_db", "things", "user1");
            logResult("Test 3: Read from incorrect collection", false,"Should have thrown error");
        } catch (err) {
            logResult("Test 3: Read from incorrect collection", 
                err.message.includes("Collection does not exist"), 
                "Wrong error message");
        }

        // Test 4: Read from incorrect database (Should fail)
        try {
            await StorageEngine.read("incorrect_db", "users", "user3");
            logResult("Test 4: Read from incorrect database", false,"Should have thrown error");
        } catch (err) {
            logResult("Test 4: Read from incorrect databasen", 
                err.message.includes("Database does not exist"), 
                "Wrong error message");
        }

        // Test 5: Missing database name (Should fail)
        try {
            await StorageEngine.read("users", "user3");
            logResult("Test 5: Missing database name", false,"Should have thrown error");
        } catch (err) {
            logResult("Test 5: Missing database name", 
                err.message.includes("Database, collection name and documentID required"), 
                "Wrong error message");
        }        

        // Clean up test data. COMMENT OUT TO VIEW CHANGES
        await cleanUp();
        
    } catch (err) {
        console.error("Test suite error:", err);
        failed++;
    }
}

//Update Tests
async function runTest5() {
    try {
        await cleanUp();

        //Setup db
        await StorageEngine.create("await cleanUp();test_db");

        //Setup collection
        const collectionPath1 = path.join(StorageEngine.dataDir, "test_db", "users");
        const collectionPath2 = path.join(StorageEngine.dataDir, "test_db", "posts");

        await fs.mkdir(collectionPath1, { recursive: true });
        await fs.mkdir(collectionPath2, { recursive: true });

        //Add documents
        await fs.writeFile(
            path.join(collectionPath1, "user1.json"),
            JSON.stringify({ id: "user1", name: "User 1", email: "user1@example.com" }),
            "utf8"
        );
        await fs.writeFile(
            path.join(collectionPath1, "user2.json"),
            JSON.stringify({ id: "user2", name: "User 2", email: "user2@example.com" }),
            "utf8"
        );
        await fs.writeFile(
            path.join(collectionPath2, "post1.json"),
            JSON.stringify({ id: "post1", title: "Post 1", content: "Content 1" }),
            "utf8"
        );

        //Test 1: Update existing Document 
        try {
            const testDoc = { email: "test@example.com", phone: "0123456789"};
            const result = await StorageEngine.update(testDoc, "test_db", "users", "user1");
            logResult("Test 1: Update existing Document", 
                result &&
                typeof result === 'object' &&
                result.id === "user1" &&
                result.name === "User 1" &&
                result.email === "test@example.com" &&
                result.phone === "0123456789",
                "Should return updated document"
            );
        } catch (err) {
            logResult("Test 1: Update existing Document", false, err.message);
        }

        //Test 2: Update non-existing Document (Should fail)
        try {
            const testDoc = { id: "user1", name: "Test User", email: "test@example.com" };
            await StorageEngine.update(testDoc, "test_db", "users", "user10");
            logResult("Test 2: Update non-existing Document", false, "Should throw error for non-existent document");
        } catch (err) {
            logResult("Test 2: Update non-existing Document", 
                err.message.includes("Document does not exist"), 
                "Wrong error message"
            );
        }
        
        //Test 3: Update with no database provided (Should fail)
        try {
            const testDoc = { id: "user1", name: "Test User", email: "test@example.com" };
            await StorageEngine.update(testDoc, null, "users", "user1");
            logResult("Test 3: Update with no database provided", false, "Should throw error for missing database");
        } catch (err) {
            logResult("Test 3: Update with no database provided", 
                err.message.includes("Database name required"), 
                "Wrong error message"
            );
        }

        // //Test 4: Update where document id changes (Should fail)
        // try {
        //     const testDoc = { id: "user8", name: "Test User", email: "test@example.com" };
        //     const result = await StorageEngine.update(testDoc, "test_db", "users", "user1");
        //     logResult("Test 4: Update where document id changes", false, "Should throw error for document mismatch");
        // } catch (err) {
        //     logResult("Test 4: Update where document id changes", 
        //         err.message.includes("Object ID mismatch"), 
        //         "Wrong error message"
        //     );
        // }

        //Test 5: Update with no new data provided (Should fail)
        try {
            await StorageEngine.update(null, "test_db", "users", "user1");
            logResult("Test 5: Update with no new data provided", false, "Should throw error for missing data");
        } catch (err) {
            logResult("Test 5: Update with no new data provided", 
                err.message.includes("New Data missing"), 
                "Wrong error message"
            );
        }
        
        //Test 6: Rename database
        try {
            await StorageEngine.update("test_db_renamed", "test_db");
            
            // Check if database was renamed
            const newDbExists = await fs.access(path.join(StorageEngine.dataDir, "test_db_renamed"))
                .then(() => true)
                .catch(() => false);
                
            logResult("Test 6: Rename database", 
                newDbExists, 
                "Database should be renamed"
            );
            
            // Rename back for other tests
            if (newDbExists) {
                await StorageEngine.update("test_db", "test_db_renamed");
            }
        } catch (err) {
            logResult("Test 6: Rename database", false, err.message);
        }
        
        //Test 7: Rename collection
        try {
            await StorageEngine.update("users_renamed", "test_db", "users");
            
            // Check if collection was renamed
            const newCollectionExists = await fs.access(path.join(StorageEngine.dataDir, "test_db", "users_renamed"))
                .then(() => true)
                .catch(() => false);
                
            logResult("Test 7: Rename collection", 
                newCollectionExists, 
                "Collection should be renamed"
            );
            
            // Rename back for other tests
            if (newCollectionExists) {
                await StorageEngine.update("users", "test_db", "users_renamed");
            }
        } catch (err) {
            logResult("Test 7: Rename collection", false, err.message);
        }

        // Clean up test data
        await cleanUp();

    } catch (err) {
        console.error("Test suite error:", err);
        failed++;
    }
}

// Search Tests
async function runTest6() {
    try {
        await cleanUp();

        // Setup db
        await StorageEngine.create("search_db");
        
        // Setup collections
        const usersCollection = path.join(StorageEngine.dataDir, "search_db", "users");
        const postsCollection = path.join(StorageEngine.dataDir, "search_db", "posts");
        const productsCollection = path.join(StorageEngine.dataDir, "search_db", "products");
        
        await fs.mkdir(usersCollection, { recursive: true });
        await fs.mkdir(postsCollection, { recursive: true });
        await fs.mkdir(productsCollection, { recursive: true });

        // Add test data
        // users
        await fs.writeFile(
            path.join(usersCollection, "user1.json"),
            JSON.stringify({ 
                id: "user1", 
                name: "John Doe", 
                email: "john@example.com", 
                age: 25,
                tags: ["admin", "active"]
            }),
            "utf8"
        );
        await fs.writeFile(
            path.join(usersCollection, "user2.json"),
            JSON.stringify({ 
                id: "user2", 
                name: "Jane Smith", 
                email: "jane@example.com", 
                age: 32,
                tags: ["active"]
            }),
            "utf8"
        );
        await fs.writeFile(
            path.join(usersCollection, "user3.json"),
            JSON.stringify({ 
                id: "user3", 
                name: "Bob Johnson", 
                email: "bob@example.com", 
                age: 41,
                tags: ["inactive"]
            }),
            "utf8"
        );

        // posts
        await fs.writeFile(
            path.join(postsCollection, "post1.json"),
            JSON.stringify({ 
                id: "post1", 
                title: "First Post", 
                content: "This is the first post content", 
                author: "user1",
                published: true,
                views: 120
            }),
            "utf8"
        );
        await fs.writeFile(
            path.join(postsCollection, "post2.json"),
            JSON.stringify({ 
                id: "post2", 
                title: "Second Post", 
                content: "This is the second post content", 
                author: "user1",
                published: true,
                views: 50
            }),
            "utf8"
        );
        await fs.writeFile(
            path.join(postsCollection, "post3.json"),
            JSON.stringify({ 
                id: "post3", 
                title: "Draft Post", 
                content: "This is a draft post", 
                author: "user2",
                published: false,
                views: 5
            }),
            "utf8"
        );

        // products
        await fs.writeFile(
            path.join(productsCollection, "product1.json"),
            JSON.stringify({ 
                id: "product1", 
                name: "Laptop", 
                price: 999.99,
                inStock: true,
                category: "electronics"
            }),
            "utf8"
        );
        await fs.writeFile(
            path.join(productsCollection, "product2.json"),
            JSON.stringify({ 
                id: "product2", 
                name: "Phone", 
                price: 499.99,
                inStock: true,
                category: "electronics"
            }),
            "utf8"
        );
        await fs.writeFile(
            path.join(productsCollection, "product3.json"),
            JSON.stringify({ 
                id: "product3", 
                name: "Chair", 
                price: 149.99,
                inStock: false,
                category: "furniture"
            }),
            "utf8"
        );

        // Test 1: Search with empty query (should return all documents)
        try {
            const result = await StorageEngine.search("search_db", "users", {});
            logResult("Test 1: Search with empty query", 
                Array.isArray(result) && result.length === 3,
                `Expected 3 results, got ${result.length}`
            );
        } catch (err) {
            logResult("Test 1: Search with empty query", false, err.message);
        }

        // Test 2: Equality search 
        try {
            const result = await StorageEngine.search("search_db", "users", { name: "Jane Smith" });
            logResult("Test 2: Simple equality search", 
                Array.isArray(result) && 
                result.length === 1 &&
                result[0].id === "user2",
                `Expected 1 result with id 'user2', got ${result.length} results`
            );
        } catch (err) {
            logResult("Test 2: Simple equality search", false, err.message);
        }

        // Test 3: Search with operator $eq
        try {
            const result = await StorageEngine.search("search_db", "users", { age: { $eq: 25 } });
            logResult("Test 3: Search with $eq operator", 
                Array.isArray(result) && 
                result.length === 1 &&
                result[0].id === "user1",
                `Expected 1 result with id 'user1', got ${result.length} results`
            );
        } catch (err) {
            logResult("Test 3: Search with $eq operator", false, err.message);
        }

        // Test 4: Search with operator $ne
        try {
            const result = await StorageEngine.search("search_db", "users", { age: { $ne: 25 } });
            logResult("Test 4: Search with $ne operator", 
                Array.isArray(result) && 
                result.length === 2 &&
                !result.some(doc => doc.id === "user1"),
                `Expected 2 results excluding 'user1', got ${result.length} results`
            );
        } catch (err) {
            logResult("Test 4: Search with $ne operator", false, err.message);
            
        }

        // Test 5: Search non-existent collection
        try {
            await StorageEngine.search("search_db", "nonexistent", {});
            logResult("Test 5: Search non-existent collection", false, "Should have thrown an error");
        } catch (err) {
            logResult("Test 5: Search non-existent collection", true, err.message);
        }

        // Test 6: Missing collection name
        try {
            await StorageEngine.search("search_db");
            logResult("Test 6: Missing collection name", false, "Should have thrown an error");
        } catch (err) {
            logResult("Test 6: Missing collection name", true, err.message);
        }

        // Test 7: Missing database name
        try {
            await StorageEngine.search();
            logResult("Test 7: Missing database name", false, "Should have thrown an error");
        } catch (err) {
            logResult("Test 7: Missing database name", true, err.message);
        }

        // Test 8: Search for non-existing value
        try {
            const result = await StorageEngine.search("search_db", "users", { name: "Nonexistent User" });
            logResult("Test 8: Search for non-existing value", 
                Array.isArray(result) && result.length === 0,
                `Expected 0 results, got ${result.length}`
            );
        } catch (err) {
            logResult("Test 8: Search for non-existing value", false, err.message);
        }

        // Clean up test data
        await cleanUp();

    } catch (err) {
        console.error("Test suite error:", err);
        failed++;
    }
}

async function storageTesting() {
    console.log('Creation Checks\n');
    await runTest1();
    console.log('\nList checks\n');
    await runTest2();
    console.log('\nDelete checks\n');
    await runTest3();
    console.log('Read Checks\n');
    await runTest4();
    console.log('Update Checks\n');
    await runTest5();
    console.log('Search Checks\n');
    await runTest6();
    console.log(`\x1b[32mPassed: ${passed}, Failed: ${failed}\x1b[0m`);
}

storageTesting();