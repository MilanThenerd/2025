// equalityRangeTest.js
// Comprehensive verification of equality, inequality and range operators
// across database, collection and document levels.

import { io } from "socket.io-client";

const socket = io("http://localhost:8008");
const step = (n, msg) => console.log(`\n[STEP ${n}] ${msg}`);

// ----- Standard response & error handlers -----
socket.on("response", (data) => {
  console.log("[CLIENT] Response:", JSON.stringify(data, null, 2));
});
socket.on("connect_error", (err) => console.error("Connection error:", err.message));

// ------------------------------------------------
// Test sequence
// ------------------------------------------------
socket.on("connect", () => {
  console.log(`Connected to daemon as ${socket.id}`);

  /* ---------------- DATABASE‑LEVEL ---------------- */

  // Clean‑up any residue from previous runs
  socket.emit("addCommand", "delete", { "==": "TestDB" });
  socket.emit("addCommand", "delete", { "==": "LogsDB" });

  step(1, "Creating databases 'TestDB' and 'LogsDB'");
  socket.emit("addCommand", "create", { TestDB: {}, LogsDB: {} });

  step(2, "Search all DBs not named 'LogsDB'");
  socket.emit("addCommand", "search", { data: { "!=": "LogsDB" } });

  step(3, "Delete all DBs named 'TestDB'");
  socket.emit("addCommand", "delete", { "==": "TestDB" });

  /* ---------------- COLLECTION‑LEVEL ---------------- */

  step(4, "Creating collections in userDB and auditDB");
  socket.emit("addCommand", "create", {
    userDB: { logs: {}, profile: {} },
    auditDB: { audit: {}, history: {} },
  });

  step(5, "Delete 'logs' collection from userDB");
  socket.emit("addCommand", "delete", { userDB: { "==": "logs" } });

  step(6, "Search collections in auditDB not named 'audit'");
socket.emit("addCommand", "search", {data: { auditDB: { "!=": "audit" } } });

  /* ---------------- DOCUMENT‑LEVEL  (== / !=)  ---------------- */

  step(7, "Creating documents in userDB and appDB");
  socket.emit("addCommand", "create", {
    userDB: {
      users: {
        u1: { role: "admin", age: 22 },
        u2: { role: "user", age: 16 },
        u3: { role: "admin", age: 18 },
      },
    },
    appDB: {
      sessions: {
        s1: { status: "inactive" },
        s2: { status: "active" },
      },
    },
  });

  step(8, "Delete users where role == 'admin'");
  socket.emit("addCommand", "delete", {
    userDB: { users: { $field: "role", "==": "admin" } },
  });

  step(9, "Search sessions where status != 'inactive'");
  socket.emit("addCommand", "search", { data: {
        appDB: { sessions: { $field: "status", "!=": "inactive" } } },
  });

  /* ---------------- DOCUMENT‑LEVEL  (range) ---------------- */

  step(10, "Creating documents with numeric and date fields");
  socket.emit("addCommand", "create", {
    shopDB: { orders: { o1: { amount: 120 }, o2: { amount: 90 } } },
    userDB: { users: { u3: { age: 18 }, u4: { age: 18 } } },
    inventoryDB: { products: { p1: { stock: 30 }, p2: { stock: 75 } } },
    eventsDB: {
      events: {
        e1: { date: "2021-01-01" },
        e2: { date: "2022-01-01" },
        e3: { date: "1999-01-01" },
      },
    },
  });

  step(11, "Search orders where amount > 100");
  socket.emit("addCommand", "search", { data: {
    shopDB: { orders: { $field: "amount", ">": 100 } } },
  });

  step(12, "Delete users where age >= 18");
  socket.emit("addCommand", "delete", {
    userDB: { users: { $field: "age", ">=": 18 } },
  });

  step(13, "Search products where stock < 50");
  socket.emit("addCommand", "search", { data: {
    inventoryDB: { products: { $field: "stock", "<": 50 } } },
  });

  step(14, "Delete events on or before 2021‑01‑01");
  socket.emit("addCommand", "delete", {
    eventsDB: { events: { $field: "date", "<=": "2021-01-01" } },
  });
});