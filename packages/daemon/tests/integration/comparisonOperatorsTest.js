// comparisonOperatorsTest.js
// Test script to verify equality and comparison operators (==, !=, <, >, <=, >=)
// across database and document levels

import { io } from "socket.io-client";

const socket = io("http://localhost:8008");
const logStep = (step, msg) => console.log(`\n[STEP ${step}] ${msg}`);

socket.on("connect", () => {
    console.log(`Connected to daemon as ${socket.id}`);

    // ----------------------------
    // Section I: Database-Level Equality Operators
    // ----------------------------

    // STEP 1: Create two test databases to work with
    setTimeout(() => {
        logStep(1, "Creating test databases 'TestDB1' and 'TestDB2'");
        socket.emit("addCommand", "create", {
            TestDB1: { dummy: { doc1: { value: "x" } } },
            TestDB2: { dummy: { doc1: { value: "x" } } }
        });
    }, 500);

    // STEP 2: List all databases to confirm creation
    setTimeout(() => {
        logStep(2, "Listing all databases");
        socket.emit("addCommand", "list", {});
    }, 1000);

    // STEP 3: Search for all databases equal to "TestDB1"
    setTimeout(() => {
        logStep(3, "Search for databases where name == 'TestDB1'");
        socket.emit("addCommand", "search", {
            data: { "==": "TestDB1" }
        });
    }, 1500);

    // STEP 4: Search for all databases not equal to "TestDB1"
    setTimeout(() => {
        logStep(4, "Search for databases where name != 'TestDB1'");
        socket.emit("addCommand", "search", {
            data: { "!=": "TestDB1" }
        });
    }, 2000);

    // ----------------------------
    // Section II: Document-Level Equality/Inequality
    // ----------------------------

    // STEP 5: Create ComparisonDB with documents having numeric values
    setTimeout(() => {
        logStep(5, "Creating 'ComparisonDB' with documents having numeric values");
        socket.emit("addCommand", "delete", { ComparisonDB: {} }); // Clean first
        socket.emit("addCommand", "create", {
            ComparisonDB: {
                numbers: {
                    n1: { value: 10, name: "ten" },
                    n2: { value: 20, name: "twenty" },
                    n3: { value: 30, name: "thirty" },
                    n4: { value: 40, name: "forty" },
                    n5: { value: 50, name: "fifty" }
                },
                users: {
                    u1: { age: 15, name: "Alice" },
                    u2: { age: 18, name: "Bob" },
                    u3: { age: 21, name: "Charlie" },
                    u4: { age: 30, name: "David" },
                    u5: { age: 40, name: "Eve" }
                }
            }
        });
    }, 2500);

    // STEP 6: Search for documents where value == 30
    setTimeout(() => {
        logStep(6, "Search for documents where value == 30");
        socket.emit("addCommand", "search",
            {
                data: {
                    ComparisonDB: {
                        numbers: {
                            $field: "value",
                            "==": 30
                        }
                    }
                }
            });
    }, 3500);

    // STEP 7: Search for documents where value != 30
    setTimeout(() => {
        logStep(7, "Search for documents where value != 30");
        socket.emit("addCommand", "search", {
            data: {
                ComparisonDB: {
                    numbers: {
                        $field: "value",
                        "!=": 30
                    }
                }
            }

        });
    }, 4000);

    // STEP 8: Search for documents where value > 30
    setTimeout(() => {
        logStep(8, "Search for documents where value > 30");
        socket.emit("addCommand", "search", {
            data : {
                ComparisonDB: {
                    numbers: {
                        $field: "value",
                        ">": 30
                    }
                }    
            }
        });
    }, 4500);

    // STEP 9: Search for documents where value < 30
    setTimeout(() => {
        logStep(9, "Search for documents where value < 30");
        socket.emit("addCommand", "search", {
            data: {
                ComparisonDB: {
                    numbers: {
                        $field: "value",
                        "<": 30
                    }
                }
            }
        });
    }, 5000);

    // STEP 10: Search for documents where value >= 30
    setTimeout(() => {
        logStep(10, "Search for documents where value >= 30");
        socket.emit("addCommand", "search", {
            data: {
                ComparisonDB: {
                    numbers: {
                        $field: "value",
                        ">=": 30
                    }
                }    
            }
        });
    }, 5500);

    // STEP 11: Search for documents where value <= 30
    setTimeout(() => {
        logStep(11, "Search for documents where value <= 30");
        socket.emit("addCommand", "search", {
            data: {
                ComparisonDB: {
                    numbers: {
                        $field: "value",
                        "<=": 30
                    }
                }
    
            }
        });
    }, 6000);

    // ----------------------------
    // Section III: Mixed Equality & String Comparisons
    // ----------------------------

    // STEP 12: Test equality with string values (ages)
    setTimeout(() => {
        logStep(12, "Search for users where age >= 18 (adults)");
        socket.emit("addCommand", "search", {
            data: {
                ComparisonDB: {
                    users: {
                        $field: "age",
                        ">=": 18
                    }
                }
            }
        });
    }, 6500);

    // STEP 13: Test less than with string values (ages)
    setTimeout(() => {
        logStep(13, "Search for users where age < 18 (minors)");
        socket.emit("addCommand", "search", {
            data: {
                ComparisonDB: {
                    users: {
                        $field: "age",
                        "<": 18
                    }
                }
            }
        });
    }, 7000);

    // STEP 14: Create date comparison tests
    setTimeout(() => {
        logStep(14, "Creating 'DatesDB' with date values");
        socket.emit("addCommand", "create", {
            DatesDB: {
                events: {
                    e1: { date: "2023-01-01", name: "New Year" },
                    e2: { date: "2023-06-15", name: "Mid Year" },
                    e3: { date: "2023-12-31", name: "Year End" },
                    e4: { date: "2024-01-01", name: "New Year 2024" }
                }
            }
        });
    }, 7500);

    // STEP 15: Test date comparison (less than)
    setTimeout(() => {
        logStep(15, "Search for events before 2023-07-01");
        socket.emit("addCommand", "search", {
            data: {
                DatesDB: {
                    events: {
                        $field: "date",
                        "<": "2023-07-01"
                    }
                }    
            }
        });
    }, 8500);

    // STEP 16: Test date comparison (greater than or equal)
    setTimeout(() => {
        logStep(16, "Search for events on or after 2023-07-01");
        socket.emit("addCommand", "search", {
            data: {  
                DatesDB: {
                    events: {
                        $field: "date",
                        ">=": "2023-07-01"
                    }
                }
            }
        });
    }, 9000);

  // STEP 17: Clean up
  setTimeout(() => {
    logStep(17, "Cleaning up test databases");
    socket.emit("addCommand", "delete", { ComparisonDB: {} });
    socket.emit("addCommand", "delete", { DatesDB: {} });
    socket.emit("addCommand", "delete", { TestDB1: {} });
    socket.emit("addCommand", "delete", { TestDB2: {} });
  }, 9500);
  
  // STEP 18: Terminate the script
  setTimeout(() => {
    logStep(18, "Test completed. Disconnecting...");
    socket.disconnect();
    console.log("Socket disconnected. Script terminating.");
    // Optional: Force exit if socket doesn't close cleanly
    setTimeout(() => process.exit(0), 500);
  }, 10000);
});

socket.on("response", (data) => {
  console.log("[CLIENT] Response:", JSON.stringify(data, null, 2));
});

socket.on("connect_error", (err) => {
  console.error("Socket connect error:", err);
});