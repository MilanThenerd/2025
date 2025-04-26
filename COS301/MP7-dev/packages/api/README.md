# MP7: MPDB-API

### Instructions to run api  
In the terminal type: node api/mpdb-api.js  

### Instructions to run testing  
*To run all tests:* npm test  
*To run unit tests:* npx jest api/tests/mpdb-api.test.js  
*To run integration tests:* npx jest api/tests/integration.test.js  


## API URIs

### function:   URI (API Operation)
(id refers to name of the user/collection/document/database)

👤 User Operations  
createUser: /register  (POST)   
deleteUser: /users/:id (DELETE)  
updateUser: /users/:id (PUT)   
getUser: /login/:id (GET)

📄 Document Operations  
createDocument: /databases/:databaseID/collections/:collection/documents        (POST)   
deleteDocument: /databases/:databaseID/collections/:collection/documents/:id    (DELETE)  
updateDocument: /databases/:databaseID/collections/:collection/documents/:id    (PUT)     
getDocument:    /databases/:databaseID/collections/:collection/documents/:id    (GET)   
getAllDocument:    /databases/:databaseID/collections/:collection/allDocuments (GET)  -> LIST all documents in collection

📂 Collection Operations  
createCollection: /databases/:databaseID/collections     (POST)  
dropCollection: /databases/:databaseID/collections/:name (DELETE)  
getCollection:  /databases/:databaseID/collections/:name (GET)   
getAllCollections: /databases/:databaseID/allCollections (GET)                     -> LIST all collections in database  
UpdateCollection: /databases/:databaseID		             (PUT)   

🗃️ Database Operations  
createDatabase:	/databases     	         (POST)    
deleteDatabase:	/databases/:databaseID   (DELETE)  
getDatabase:	/databases/:databaseID     (GET)    
getAllDatabase: /databases               (GET)    
updateDatabase: /databases/:databaseID   (PUT)         



## Non-Functional Requirements
Performance
-95% of GET requests respond within 200ms under normal load (≤1000 concurrent users).
-real time communication occurs with the daemon
-response time improved due to pagination support which limits the amount of data sent in a single request

Scalability
-system can handle large amounts of data due to pagination support

Usability
-API documentation includes executable examples in Postman formats.
