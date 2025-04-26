import updateCommand from "../../src/command/updateCommand.js";
import createCommand from "../../src/command/createCommand.js";
import listCommand from "../../src/command/listCommand.js";
import searchCommand from "../../src/command/searchCommand.js";

import fs from "fs/promises";
import path from "path";

async function cleanUp() {
    try {
        // Determine data directory path relative to current directory
        const dataDir = path.join(process.cwd(), "data");
        
        console.log("Cleaning up data directory:", dataDir);
        
        try {
            // Check if data directory exists
            await fs.access(dataDir);
        } catch (err) {
            console.log("Data directory does not exist yet:", err.message);
            return; // Exit if data directory doesn't exist
        }
        
        // Get all directories in the data folder
        const entries = await fs.readdir(dataDir);
        
        // Log what we found
        console.log("Found directories:", entries);
        
        // Delete all folders in the data directory
        for (const entry of entries) {
            const entryPath = path.join(dataDir, entry);
            
            // Check if it's a directory
            const stats = await fs.stat(entryPath);
            if (stats.isDirectory()) {
                console.log(`Deleting directory: ${entry}`);
                await fs.rm(entryPath, { recursive: true, force: true });
            }
        }
        
        console.log("Cleanup completed successfully");
    } catch (err) {
        console.error("Cleanup error:", err);
    }
}

async function createTest() {
    console.log("Testing create");
    let testComm = new createCommand(
        {
            userDB: {
                usersCollection: {
                    user1: {
                        "name": "John Doe",
                        "email": "john.doe@example.com",
                        "age": 35,
                        "status": "active",
                        "id" : "m83bs9qgq4xix"
                    },
                    user2: {
                        "name": "Jane Doe",
                        "email": "jane.doe@example.com",
                        "age": 28,
                        "status": "inactive",
                        "id" : "m83bs9qgyxr5h"
                    },
                    user3: {
                        "name": "Alice Smith",
                        "email": "alice.smith@example.com",
                        "age": 35,
                        "status": "active",
                        "address": "123 Main St"
                    },
                    user4: {
                        "name": "Bob Jones",
                        "email": "bob.jones@example.com",
                        "age": 45,
                        "status": "active",
                        "city": "New York",
                        "country": "USA"
                    }
                },
                userDetails: {
                    user1: {
                        "name": "Liam Hemsworth",
                        "email": "liam.hem@example.com",
                        "age": 32,
                        "role": "Admin"
                    },
                    user2: {
                        "name": "Olivia Munn",
                        "email": "olivia.munn@example.com",
                        "age": 29,
                        "role": "User",
                        "location": "California"
                    },
                    user3: {
                        "name": "Tyler Perry",
                        "email": "tyler.perry@example.com",
                        "age": 50,
                        "role": "Moderator",
                        "status": "active",
                        "lastLogin": "2025-03-01"
                    }
                }
            },
            anotherDB: {
                secondaryCollection: {
                    record1: {
                        "name": "Sophia Turner",
                        "email": "sophia.turner@example.com",
                        "address": "789 Oak Rd",
                        "phone": "123-456-7890"
                    },
                    record2: {
                        "name": "Nina Dobrev",
                        "email": "nina.dobrev@example.com",
                        "age": 34,
                        "address": "456 Pine St",
                        "status": "active"
                    },
                    record3: {
                        "name": "Matt Damon",
                        "email": "matt.damon@example.com",
                        "city": "Los Angeles",
                        "country": "USA",
                        "phone": "987-654-3210"
                    },
                    record4: {
                        "name": "Julia Roberts",
                        "email": "julia.roberts@example.com",
                        "age": 55,
                        "status": "inactive",
                        "lastLogin": "2025-02-25",
                        "preferredLanguage": "English"
                    }
                },
                extraCollection: {
                    item1: {
                        "name": "Jessica Alba",
                        "email": "jessica.alba@example.com",
                        "age": 38,
                        "status": "active",
                        "location": "Los Angeles"
                    },
                    item2: {
                        "name": "Ashton Kutcher",
                        "email": "ashton.kutcher@example.com",
                        "address": "222 Sunset Blvd",
                        "phone": "555-987-6543",
                        "status": "active"
                    },
                    item3: {
                        "name": "Kendall Jenner",
                        "email": "kendall.jenner@example.com",
                        "age": 28,
                        "city": "Miami"
                    },
                    item4: {
                        "name": "Zoe Saldana",
                        "email": "zoe.saldana@example.com",
                        "age": 42,
                        "address": "123 Hollywood Ave",
                        "status": "active"
                    },
                    item5: {
                        "name": "Will Smith",
                        "email": "will.smith@example.com",
                        "city": "Philadelphia",
                        "status": "active",
                        "phone": "444-555-6666"
                    }
                },
                finalCollection: {
                    record1: {
                        "name": "Margot Robbie",
                        "email": "margot.robbie@example.com",
                        "status": "active",
                        "age": 33,
                        "city": "Los Angeles",
                        "phone": "111-222-3333"
                    },
                    record2: {
                        "name": "Matt Bomer",
                        "email": "matt.bomer@example.com",
                        "address": "456 Maple St",
                        "status": "active",
                        "country": "Canada",
                        "phone": "333-444-5555"
                    },
                    record3: {
                        "name": "Leonardo DiCaprio",
                        "email": "leonardo.dicaprio@example.com",
                        "age": 50,
                        "city": "Los Angeles",
                        "country": "USA"
                    },
                    record4: {
                        "name": "Ariana Grande",
                        "email": "ariana.grande@example.com",
                        "address": "123 Oak Lane",
                        "phone": "666-777-8888",
                        "status": "active"
                    },
                    record5: {
                        "name": "Kylie Jenner",
                        "email": "kylie.jenner@example.com",
                        "status": "inactive",
                        "location": "Los Angeles",
                        "lastLogin": "2025-03-03"
                    }
                }
            },
            thirdDB: {
                mainCollection: {
                    entry1: {
                        "name": "Emma Stone",
                        "email": "emma.stone@example.com",
                        "status": "active",
                        "age": 30
                    },
                    entry2: {
                        "name": "Tom Hanks",
                        "email": "tom.hanks@example.com",
                        "city": "New York",
                        "status": "inactive"
                    },
                    entry3: {
                        "name": "Julia Roberts",
                        "email": "julia.roberts@example.com",
                        "age": 55,
                        "lastLogin": "2025-01-15",
                        "location": "California"
                    }
                },
                anotherCollection: {
                    entry1: {
                        "name": "Ben Affleck",
                        "email": "ben.affleck@example.com",
                        "status": "active",
                        "role": "Actor"
                    },
                    entry2: {
                        "name": "Jessica Chastain",
                        "email": "jessica.chastain@example.com",
                        "status": "active",
                        "phone": "444-123-7890",
                        "city": "Chicago"
                    },
                    entry3: {
                        "name": "Reese Witherspoon",
                        "email": "reese.witherspoon@example.com",
                        "age": 49,
                        "address": "678 Hollywood Blvd",
                        "status": "inactive"
                    },
                    entry4: {
                        "name": "Chris Pratt",
                        "email": "chris.pratt@example.com",
                        "phone": "555-987-6543",
                        "city": "Los Angeles",
                        "status": "active"
                    }
                },
                dataCollection: {
                    record1: {
                        "name": "Bryce Dallas Howard",
                        "email": "bryce.dallas@example.com",
                        "status": "active",
                        "location": "London",
                        "phone": "666-555-4444"
                    },
                    record2: {
                        "name": "Matthew McConaughey",
                        "email": "matthew.mcconaughey@example.com",
                        "status": "inactive",
                        "age": 55
                    },
                    record3: {
                        "name": "Meryl Streep",
                        "email": "meryl.streep@example.com",
                        "status": "active",
                        "address": "789 Park Ave",
                        "age": 75
                    },
                    record4: {
                        "name": "Nicole Kidman",
                        "email": "nicole.kidman@example.com",
                        "city": "Sydney",
                        "status": "inactive",
                        "age": 56
                    },
                    record5: {
                        "name": "Cate Blanchett",
                        "email": "cate.blanchett@example.com",
                        "status": "active",
                        "age": 54,
                        "city": "Melbourne"
                    }
                }
            }
        }
    );
    let result = await testComm.execute();
    console.log(result);
}

// async function deleteTest() {
//     console.log("Testing delete document");
//     let testComm = new deleteCommand({ userDB: { usersCollection: { m83bs9qgq4xix: {}, m83bs9qgyxr5h: {} } } });
//     let result = await testComm.execute();
//     console.log(result);
//     console.log("-----------------------------------------------------------------------");
//     console.log("Testing delete collection");
//     testComm = new deleteCommand({ userDB: { detailsCollection: {} } });
//     result = await testComm.execute();
//     console.log(result);
//     console.log("-----------------------------------------------------------------------");
//     console.log("Testing delete collection");
//     testComm = new deleteCommand({ thirdDB: { mainCollection: {} } });
//     result = await testComm.execute();
//     console.log(result);
//     console.log("-----------------------------------------------------------------------");
//     console.log("Testing delete database");
//     testComm = new deleteCommand({ fourthDB: {} });
//     result = await testComm.execute();
//     console.log(result);
//     console.log("-----------------------------------------------------------------------");
// }

async function listTest() {
    console.log("Testing list databases");
    let testComm = new listCommand("");
    let result = await testComm.execute();
    console.log(result);
    console.log("-----------------------------------------------------------------------");
    console.log("Testing list collections");
    testComm = new listCommand({ userDB: {} });
    result = await testComm.execute();
    console.log(result);
    console.log("-----------------------------------------------------------------------");
}

async function updateTest() {
    console.log("Testing update collection name");
    let testComm = new updateCommand({ userDB: { "userDetails#reference": "detailsCollection" } });
    let result = await testComm.execute();
    console.log(result);
    console.log("-----------------------------------------------------------------------");
    console.log("Testing update database name");
    testComm = new updateCommand({ "anotherDB#reference": "fourthDB" });
    result = await testComm.execute();
    console.log(result);
    console.log("-----------------------------------------------------------------------");
}

async function searchTest(){
    console.log("Testing search on 1 field");
    let testComm = new searchCommand({ data: {
            userDB: { 
                usersCollection: { 
                    "name": "John Doe" 
                } 
            } 
        }
    });

    let result = await testComm.execute();
    console.log(result);
    console.log("-----------------------------------------------------------------------");
    console.log("Testing search on $eq");
    testComm = new searchCommand({ data: {
                                        userDB: { 
                                            usersCollection: { 
                                                "age": {"$eq": 35}
                                            } 
                                        } 
                                    }
                                });
    result = await testComm.execute();
    console.log(result);
    console.log("-----------------------------------------------------------------------");
    console.log("Testing search on $ne");
    testComm  = new searchCommand({ data: {
                                        userDB: { 
                                            usersCollection: { 
                                                "age": {"$ne": 35}
                                            } 
                                        } 
                                    }
                                });
    result = await testComm.execute();
    console.log(result);
    console.log("-----------------------------------------------------------------------");
    console.log("Testing search with no result");
    testComm  = new searchCommand({ data: {
                                        userDB: { 
                                            usersCollection: { 
                                                "name": "Johns Doe" 
                                            } 
                                        } 
                                    }
                                });
    result = await testComm.execute();
    console.log(result);
    console.log("-----------------------------------------------------------------------");
}

//createTest().then(() => { updateTest().then(() => { readTest().then(() => { listTest().then(() => { deleteTest(); }); }); }); });

async function runTests() {
    await cleanUp();

    await createTest();
    await searchTest();
    await updateTest();
    await listTest();
  //  await deleteTest();
}
runTests().catch(console.error);