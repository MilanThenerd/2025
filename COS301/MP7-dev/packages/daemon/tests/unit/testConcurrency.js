import mpdbdaemon from '../../src/daemon.js';
import { EventEmitter } from 'events';

console.log("Starting extensive concurrency tests...");

// A simple mock socket 
class MockSocket extends EventEmitter {
  constructor(id) {
    super();
    this.id = id;
    console.log(`[MockSocket] Created socket with id: ${id}`);
  }
  emit(event, ...args) {
    console.log(`[Socket ${this.id}] ${event}:`, ...args);
  }
  disconnect(force) {
    console.log(`[Socket ${this.id}] Disconnected (force=${force})`);
  }
}

// Instantiate the daemon on a different port (to avoid EADDRINUSE)
const daemon = new mpdbdaemon(8012);
daemon.start();
console.log("[Daemon] Daemon started");

// Helper to log when a command is queued
function logQueue(socket, type, data) {
  console.log(`Queuing command [${type}] from Socket ${socket.id} for resource ${data.userId} (scenario: ${data.mockScenario || "default"})`);
}

//pause function
function pause(ms) {
  return new Promise(resolve => setTimeout(resolve, ms));
}

//Extensive test cases
async function runExtensiveTests() {
  console.log("\n Extensive Concurrency Tests \n");

  //Single User, Sequential Commands (using "sequential" scenario)
  console.log(" Test 1: Single user sequential commands ");
  const socketA = new MockSocket("userA");
  logQueue(socketA, "delete", { userId: "resourceSeq", data: "Delete Data", mockScenario: "sequential" });
  daemon.addToQueue(socketA, "delete", { userId: "resourceSeq", data: "Delete Data", mockScenario: "sequential" });
  await pause(1000);
  logQueue(socketA, "update", { userId: "resourceSeq", data: "Update Data", mockScenario: "sequential" });
  daemon.addToQueue(socketA, "update", { userId: "resourceSeq", data: "Update Data", mockScenario: "sequential" });
  await pause(1500);

  //Multiple Users, Different Resources (no conflict)
  console.log("\n Test 2: Multiple users, different resources ");
  const socketB = new MockSocket("userB");
  const socketC = new MockSocket("userC");
  logQueue(socketB, "update", { userId: "resourceB", data: "Update Data B", mockScenario: "unique" });
  daemon.addToQueue(socketB, "update", { userId: "resourceB", data: "Update Data B", mockScenario: "unique" });
  logQueue(socketC, "delete", { userId: "resourceC", data: "Delete Data C", mockScenario: "unique" });
  daemon.addToQueue(socketC, "delete", { userId: "resourceC", data: "Delete Data C", mockScenario: "unique" });
  await pause(1500);

  //Multiple Users, Same Resource (conflict scenario)
  console.log("\n Test 3: Multiple users, same resource (conflict) ");
  const socketD = new MockSocket("userD");
  const socketE = new MockSocket("userE");


  // Both commands will return "conflictingResource" due to mockScenario "conflict"
  logQueue(socketD, "update", { userId: "ignored", data: "Update from D", mockScenario: "conflict" });
  daemon.addToQueue(socketD, "update", { userId: "ignored", data: "Update from D", mockScenario: "conflict" });
  await pause(200);
  logQueue(socketE, "delete", { userId: "ignored", data: "Delete from E", mockScenario: "conflict" });
  daemon.addToQueue(socketE, "delete", { userId: "ignored", data: "Delete from E", mockScenario: "conflict" });
  await pause(1500);

  //Multiple Commands in Rapid Succession (simulate near-simultaneous arrival)
  console.log("\n Test 4: Rapid commands from a single user ");
  const socketF = new MockSocket("userF");
  for (let i = 1; i <= 3; i++) {
    logQueue(socketF, "read", { userId: "resourceRapid", data: `Read Data ${i}`, mockScenario: "sequential" });
    daemon.addToQueue(socketF, "read", { userId: "resourceRapid", data: `Read Data ${i}`, mockScenario: "sequential" });
  }
  await pause(2000);

  //Invalid Command Scenario - Missing data field and wrong command type
  console.log("\n Test 5: Invalid command scenarios ");
  const socketG = new MockSocket("userG");
  // Missing data field
  logQueue(socketG, "update", { userId: "resourceInvalid", mockScenario: "invalid" });
  daemon.addToQueue(socketG, "update", { userId: "resourceInvalid", mockScenario: "invalid" });
  await pause(500);
  // Unknown command type
  logQueue(socketG, "unknownCommand", { userId: "resourceInvalid", data: "Some Data", mockScenario: "invalid" });
  daemon.addToQueue(socketG, "unknownCommand", { userId: "resourceInvalid", data: "Some Data", mockScenario: "invalid" });
  await pause(1000);

  //Socket Disconnection Mid-Queue Test
  console.log("\n Test 6: Socket disconnection mid-queue ");
  const socketH = new MockSocket("userH");
  logQueue(socketH, "update", { userId: "resourceDisconnect", data: "Pre-Disconnect Update", mockScenario: "disconnect" });
  daemon.addToQueue(socketH, "update", { userId: "resourceDisconnect", data: "Pre-Disconnect Update", mockScenario: "disconnect" });
  await pause(500);
  // Simulate disconnection
  socketH.disconnect(true);
  await pause(200);
  // Attempt to queue a command after disconnect
  logQueue(socketH, "delete", { userId: "resourceDisconnect", data: "Post-Disconnect Delete", mockScenario: "disconnect" });
  daemon.addToQueue(socketH, "delete", { userId: "resourceDisconnect", data: "Post-Disconnect Delete", mockScenario: "disconnect" });
  await pause(1000);

  //Simultaneous Commands on Same Resource with Alternating Types
  console.log("\n Test 7: Alternating commands on same resource from multiple sockets ");
  const socketI = new MockSocket("userI");
  const socketJ = new MockSocket("userJ");
  const socketK = new MockSocket("userK");
  // Socket I sends an update
  logQueue(socketI, "update", { userId: "resourceAlt", data: "Update from I", mockScenario: "alternating" });
  daemon.addToQueue(socketI, "update", { userId: "resourceAlt", data: "Update from I", mockScenario: "alternating" });
  await pause(300);
  // Socket J sends a delete
  logQueue(socketJ, "delete", { userId: "resourceAlt", data: "Delete from J", mockScenario: "alternating" });
  daemon.addToQueue(socketJ, "delete", { userId: "resourceAlt", data: "Delete from J", mockScenario: "alternating" });
  await pause(300);
  // Socket K sends a read
  logQueue(socketK, "read", { userId: "resourceAlt", data: "Read from K", mockScenario: "alternating" });
  daemon.addToQueue(socketK, "read", { userId: "resourceAlt", data: "Read from K", mockScenario: "alternating" });
  await pause(1500);

  // Crazy Commands from Multiple Sockets with Various Scenarios
  console.log("\n Test 8: Bulk commands from multiple sockets ");
  const sockets = [];
  for (let i = 0; i < 5; i++) {
    const s = new MockSocket(`bulkUser${i}`);
    sockets.push(s);
  }
  // Queue different types of commands in bulk
  for (let i = 0; i < sockets.length; i++) {
    logQueue(sockets[i], "read", { userId: `bulkResource${i}`, data: `Bulk Read ${i}`, mockScenario: "bulk" });
    daemon.addToQueue(sockets[i], "read", { userId: `bulkResource${i}`, data: `Bulk Read ${i}`, mockScenario: "bulk" });
    await pause(200);
    logQueue(sockets[i], "update", { userId: `bulkResource${i}`, data: `Bulk Update ${i}`, mockScenario: "bulk" });
    daemon.addToQueue(sockets[i], "update", { userId: `bulkResource${i}`, data: `Bulk Update ${i}`, mockScenario: "bulk" });
    await pause(200);
    logQueue(sockets[i], "delete", { userId: `bulkResource${i}`, data: `Bulk Delete ${i}`, mockScenario: "bulk" });
    daemon.addToQueue(sockets[i], "delete", { userId: `bulkResource${i}`, data: `Bulk Delete ${i}`, mockScenario: "bulk" });
    await pause(100);
  }
  await pause(2000);

  //Repeated Commands with Delays to Simulate LongRunning Tasks
  console.log("\n Test 9: Repeated commands with delays ");
  const socketL = new MockSocket("userL");
  for (let i = 1; i <= 5; i++) {
    logQueue(socketL, "update", { userId: "resourceLong", data: `Long Update ${i}`, mockScenario: "longRunning" });
    daemon.addToQueue(socketL, "update", { userId: "resourceLong", data: `Long Update ${i}`, mockScenario: "longRunning" });
    await pause(700);
  }
  await pause(1500);

  // Mixed Scenario--Combining Valid and Invalid Commands Across Several Sockets
  console.log("\n Test 10: Mixed valid/invalid commands across multiple sockets ");
  const socketM = new MockSocket("userM");
  const socketN = new MockSocket("userN");


  // Valid command from socketM
  logQueue(socketM, "read", { userId: "resourceMixed", data: "Valid Read", mockScenario: "mixed" });
  daemon.addToQueue(socketM, "read", { userId: "resourceMixed", data: "Valid Read", mockScenario: "mixed" });
  await pause(300);


  // Invalid command from socketN (missing resource field)
  logQueue(socketN, "update", { data: "No Resource", mockScenario: "mixed" });
  daemon.addToQueue(socketN, "update", { data: "No Resource", mockScenario: "mixed" });
  await pause(300);


  // Another valid command from socketN
  logQueue(socketN, "delete", { userId: "resourceMixed", data: "Valid Delete", mockScenario: "mixed" });
  daemon.addToQueue(socketN, "delete", { userId: "resourceMixed", data: "Valid Delete", mockScenario: "mixed" });
  await pause(1500);

  console.log("\n Extensive Concurrency Tests Completed \n");
}
export async function getMockResourceId(commandData) {
  await new Promise(resolve => setTimeout(resolve, 10));
  const scenario = commandData.mockScenario || "";

  switch (scenario) {
    case "conflict":
      return "conflictingResourceId";
    case "sequential":
      return "sequentialResourceId";
    case "unique":
      return "uniqueResourceId";
    default:
      return "defaultId";
  }
}
runExtensiveTests().catch(console.error);
