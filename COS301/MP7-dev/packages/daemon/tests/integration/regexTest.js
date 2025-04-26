import { io } from "socket.io-client";

const socket = io("http://localhost:8008");
const logStep = (step, msg) => console.log(`\n[STEP ${step}] ${msg}`);

socket.on("connect", () => {
  console.log(`Connected to daemon as ${socket.id}`);

  // ----------------------------
  // Section I: Literal (Regular) Functionality Tests
  // ----------------------------

  // STEP 1: List all databases (literal search)

  logStep(1, "Listing all databases (literal search)");
  socket.emit("addCommand", "list", {});

  // STEP 2: Create a test database "TestLiteralDB" with collections "users" and "orders"

  logStep(2, "Creating fresh dataset in 'TestLiteralDB' (users & orders)");
  socket.emit("addCommand", "delete", { TestLiteralDB: {} }); // wipe if exists
  const createPayload = {
    TestLiteralDB: {
      users: {
        u1: { username: "alice", role: "admin" },
        u2: { username: "bob", role: "user" },
      },
      orders: {
        o1: { orderNumber: "1001", amount: 250 },
        o2: { orderNumber: "1002", amount: 150 },
      },
    },
  };
  socket.emit("addCommand", "create", createPayload);

  // STEP 3: Literal search for admins

  logStep(3, "Literal search in 'TestLiteralDB'->'users' for role='admin'");
  socket.emit("addCommand", "search", { data: {
    TestLiteralDB: { users: { role: "admin" } } },
  });

  // STEP 4: Literal search for orderNumber="1001"

  logStep(4, "Literal search in 'TestLiteralDB'->'orders' for orderNumber='1001'");
  socket.emit("addCommand", "search", { data: {
    TestLiteralDB: { orders: { orderNumber: "1001" } } },
  });

  // ----------------------------
  // Section II: Collection‑Level Regex Deletion Tests
  // ----------------------------

  // STEP 5: Create a database "RegexCollectionDB" with colAlpha, colBeta, colGamma

  logStep(5, "Creating 'RegexCollectionDB' with colAlpha, colBeta, colGamma");
  socket.emit("addCommand", "delete", { RegexCollectionDB: {} });
  const createPayload2 = {
    RegexCollectionDB: {
      colAlpha: { dummy: { data: "x" } },
      colAlpha2: { dummy: { data: "x" } },
      colBeta: { dummy: { data: "x" } },
      Beta: { dummy: { data: "x" } },
      Gamma: { dummy: { data: "x" } },
      colGamma: { dummy: { data: "x" } },
    },
  };
  socket.emit("addCommand", "create", createPayload2);

  // STEP 6: Delete collections starting with ^colA

  logStep(6, "Deleting collections start‑with '^colA' (→ colAlpha)");
  socket.emit("addCommand", "delete", {
    RegexCollectionDB: {
      newCollections: {
        collectionKey: "^colA",
        docsObj: {},
      },
    },
  });

  // STEP 7: Delete collections containing ~Beta

  logStep(7, "Deleting collections contains '~Beta' (→ colBeta)");
  socket.emit("addCommand", "delete", {
    RegexCollectionDB: {
      newCollections: {
        collectionKey: "~Beta",
        docsObj: {},
      },
    },
  });

  // STEP 8: Delete collections ending with 'Gamma$'

  logStep(8, "Deleting collections ends‑with 'Gamma$' (→ colGamma)");
  socket.emit("addCommand", "delete", {
    RegexCollectionDB: {
      newCollections: {
        collectionKey: "Gamma$",
        docsObj: {},
      },
    },
  });

  // STEP 9: Confirm no collections remain

  logStep(9, "Listing collections in 'RegexCollectionDB' (should be empty)");
  socket.emit("addCommand", "list", { RegexCollectionDB: {} });

  // ----------------------------
  // Section III: Document‑Level Regex Deletion & Search Tests
  // ----------------------------

  // STEP 10: Wipe "RegexDocumentTestDB"

  logStep(10, "Wiping 'RegexDocumentTestDB'");
  socket.emit("addCommand", "delete", { RegexDocumentTestDB: {} });

  // STEP 11: Create items in "RegexDocumentTestDB"

  logStep(11, "Creating items in 'RegexDocumentTestDB'");
  const createPayload3 = {
    RegexDocumentTestDB: {
      items: {
        item1: { name: "Product1", category: "Electronics" },
        item2: { name: "SuperProduct", category: "Gadgets" },
        item3: { name: "CheapProduct", category: "Discount" },
        item4: { name: "MiniProduct1", category: "Food" },
      },
    },
  };
  socket.emit("addCommand", "create", createPayload3);

  // STEP 12: Regex search ^Prod

  logStep(12, "Regex search '^Prod' in field 'name' (→ Product1)");
  socket.emit("addCommand", "search", { data: {
    RegexDocumentTestDB: {
      items: { "^Prod": {}, "$field": "name" },
    }}
  });

  // STEP 13: Regex search ~erPr

  logStep(13, "Regex search '~erPr' in field 'name' (→ SuperProduct)");
  socket.emit("addCommand", "search", { data: {
    RegexDocumentTestDB: {
      items: { "~erPr": {}, "$field": "name" },
    }}
  });

  // STEP 14: Regex search 1$

  logStep(14, "Regex search '1$' in field 'name' (→ ends‑with '1')");
  socket.emit("addCommand", "search", { data: {
    RegexDocumentTestDB: {
      items: { "1$": {}, "$field": "name" },
    }}
  });

  // STEP 15: Delete docs ^Prod

  logStep(15, "Deleting docs '^Prod' in field 'name' (→ Product1)");
  socket.emit("addCommand", "delete", {
    RegexDocumentTestDB: {
      regexDocs: {
        collectionKey: "items",
        docsObj: { "^Prod": {}, "$field": "name" },
      },
    },
  });

  // STEP 16: Delete docs ~erPr

  logStep(16, "Deleting docs '~erPr' in field 'name' (→ SuperProduct)");
  socket.emit("addCommand", "delete", {
    RegexDocumentTestDB: {
      regexDocs: {
        collectionKey: "items",
        docsObj: { "~erPr": {}, "$field": "name" },
      },
    },
  });

  // STEP 17: Delete docs '1$'

  logStep(17, "Deleting docs '1$' in field 'name' (→ MiniProduct1)");
  socket.emit("addCommand", "delete", {
    RegexDocumentTestDB: {
      regexDocs: {
        collectionKey: "items",
        docsObj: { "1$": {}, "$field": "name" },
      },
    },
  });

  // STEP 18: Final list of remaining items

  logStep(18, "Final search in 'RegexDocumentTestDB'->'items' (should only see CheapProduct)");
  socket.emit("addCommand", "search", { data: {
    RegexDocumentTestDB: { items: {} },
  } });
}); // <-- Closing bracket for socket.on("connect")

socket.on("response", (data) => {
  console.log("[CLIENT] Response:", JSON.stringify(data, null, 2));
});

socket.on("connect_error", (err) => {
  console.error("Socket connect error:", err);
});