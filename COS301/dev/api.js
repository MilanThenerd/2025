class APIWrapper {
  constructor(baseURL) {
      this.baseURL = baseURL; // API base URL
  }

  // Helper function to send requests
  /**
  //  * Sends an HTTP request to the specified API endpoint.
  //  *
  //  * @param {string} endpoint - The API endpoint to request.
  //  * @param {string} [method='GET'] - The HTTP method (GET, POST, PUT, DELETE, etc.).
  //  * @param {object|null} [body=null] - The request body, if applicable.
  //  * @returns {Promise<object|null>} - The API response as a JSON object or null on failure.
  //  *
  //  * @throws {Error} If the request fails or the response is not OK.
  //  */
  async request(endpoint, method = 'GET', body = null) { // required parameter, all fields are optional except endpoint. THe default values for method is "GET" and the default value for body is NULL
      //configure the settigns for the fetch api call. It defines how the HTTP request should be made including method (GET POST) and headers if nececary
      const options = {
          method, // HTTP method (GET, POST, etc.)
          headers: {
              'Content-Type': 'application/json' // this can stay
          }
      };

      if (body) options.body = JSON.stringify(body); //body if there is any is sent as a string, this converts the body into a JSON string

      try {
          //await fetch(..) HTTP request but wait for a response before completing, since we are dealing with a URL it is good to check first
          //baseURL is base address of api
          //endpoint is tells us exactly what the user will want to get e.g users, description of database, 
          //option is an object that configures how the request is made

          const response = await fetch(`${this.baseURL}${endpoint}`, options);


          if (!response.ok) throw new Error(`API Error: ${response.statusText}`);
          return await response.json(); // Convert response to JSON

      } catch (error) {
          console.error('Request failed:', error);
          return null;
      }
  }

  //error messages
  //describe database. output into command


  //these helpers might need to be changed depending on the way user data will be stored


  // Get a user by ID
  // getUser: /users/:userID (GET) ->
  /**
   * Retrieves a user by their unique ID.
   * 
   * @param {string} userID - The ID of the user to retrieve.
   * @returns {Promise<object>} - The user data returned from the server.
   * 
   * @throws {Error} If no userID is provided.
   * 
   * Example response:
   * {
   *   "id": "12345",
   *   "name": "John Doe",
   *   "email": "john@example.com",
   *   "createdAt": "2024-01-01T12:00:00Z"
   * }
   */
  async getUser(userID) {
      if(!userID){
          throw new Error('User ID is required')
      }
      return this.request(`/users/${userID}`);
  }
  
  // Get all users
  // getUsers: /users (GET) ->
  /**
   * Retrieves all users from the API.
   *
   * @returns {Promise<object[]>} - A list of users.
   */
  async getUsers() {
      return this.request('/users');
  }
  // Create a new user 
  // registerUser: /register (POST) ->
  /**
   * Registers a new user.
   *
   * @param {object} userData - The user data for registration.
   * @returns {Promise<object>} - The response from the API.
   *
   * @throws {Error} If userData is not an object.
   */
  async registerUser(userData) {
      if(!userData || typeof userData !== 'object'){
          throw new Error('Valid user data is required');
      }
      return this.request('/register', 'POST', userData);
  }

  //Login
  /**
   * Logs in a user.
   *
   * @param {object} userData - The login credentials.
   * @returns {Promise<object>} - The login response from the API.
   *
   * @throws {Error} If userData is not an object.
   */
  async loginUser(userData) {
      if(!userData || typeof userData !== 'object'){
          throw new Error('Valid login data is required');
      }
      return this.request('/login', 'POST', userData);
  }

  // Update a user
  // updateUser: /users/:id (PUT) ->
  /**
   * Updates an existing user's data.
   *
   * @param {string} userID - The ID of the user to update.
   * @param {object} updatedData - The updated user data.
   * @returns {Promise<object>} - The updated user response.
   *
   * @throws {Error} If no ID or invalid data is provided.
   */
  async updateUser(userID, updatedData) {
      if(!userID) {
          throw new Error('User ID is required');
      }
      if(!updatedData || typeof updatedData !== 'object'){
          throw new Error('Updated user data is required')
      }
      return this.request(`/users/${userID}`, 'PUT', updatedData);
  }


  // Delete a user
  // deleteUser: /users/:id (DELETE) ->
  /**
   * Deletes a user by their ID.
   *
   * @param {string} userID- The ID of the user to delete.
   * @returns {Promise<object>} - The API response.
   *
   * @throws {Error} If no user ID is provided.
   */
  async deleteUser(userID) {
      if(!userID){
          throw new Error('User ID is required');
      }
      return this.request(`/users/${userID}`, 'DELETE');
  }

  //honestly no clue what this read was for so I got rid of it

//more helper functions
// describeDatabase: /databases/:databaseID/info            (GET)  ->
  /**
   * returns the structure of the database - for sprint 2?
   * @param {string} databaseID - name of the database described 
   * @returns{Promise<object>} - the response from the server
   */
  async describeDatabase(databaseID) 
  {

      if (!databaseID)
      {
         throw new Error('Database id is required');
      }
      return this.request('/database/info');
      // returns ex
      //{
      //  "name": "MyNoSQLDB",
      //  "collections": ["users", "orders", "logs"],
      //  "size": "15GB",
      //  "documents_count": 523456
      //}
  }

//queryDocuments: /databases/:databaseID/collections/:collection/documents/query (POST) -> .emit("addCommand", "create", data)
  async queryDocuments(databaseID, collectionName,query) 
  {
      return this.request(`/collections/${collection}/documents/query`, 'POST', filter);
  }

//listRecords: /databases/:databaseID/collections/:collection/indexes            (GET)  -> .emit("addCommand", "list", data)
  async listRecords(databaseID,collectionName) 
  {
      return this.request(`/collections/${collection}/indexes`);
  }
      

//all the CRUD operations

//Database operations
// createDatabase:	/databases     	         (POST)   -> .emit("addCommand", "create", data)
  /**
   * Creates a new database
   * 
   * @param {string} databaseID - The id of the database to create the collection in
   * @returns {Promise<Object>} - The response from the server
   */
  async createDatabase(databaseID) 
  {
         if (!databaseID)
          {
             throw new Error('Database id is required');
         }
         // Construct the nested data structure that the daemon expects
         const data = {
             databaseID: {}
         };
 
         // Send the request to API create a collection
         return this.request(`/databases`, 'POST', data);
  }

// deleteDatabase:	/databases/:databaseID   (DELETE) -> .emit("addCommand", "delete", data)
  /**
  * Deletes an existing database
  * 
  * @param {string} databaseID - The id of the database to be deleted
  * @returns {Promise<Object>} - The response from the server
  */
  
  async deleteDatabase(databaseID) 
  {
      if (!databaseID) 
      {
          throw new Error('Database id is required');
      }

      return this.request(`/databases/${databaseID}`, 'DELETE'); 
  }

// getDatabase:	/databases/:databaseID   (GET)    -> .emit("addCommand", "read", data)
  /**
   * Deletes an existing database
   * 
   * @param {string} databaseID - The id of the database to be returned
   * @returns {Promise<Object>} - The response from the server
   */

  async getDatabase(databaseID)
  {
      if (!databaseID) 
      {
          throw new Error('Database id is required');
      }

      // Construct the nested data structure that the daemon expects.
      const data = 
      {
          databaseID: {} 
      };
      return this.request(`/databases/${databaseID}`, 'GET', data); 
  }

  // getAllDatabase: /databases               (GET)    -> .emit("addCommand", "read", data)   
  /**
   * Returns all databases
   * 
   * @returns {Promise<Object>} - The response from the server
   */
  async getAllDatabase()
  {
      return this.request(`/databases`, 'GET'); 
  }

//updateDatabase: /databases/:databaseID   (PUT)    -> .emit("addCommand", "update", data)
  /**
   * Deletes an existing database
   * 
   * @param {string} databaseID - The id of the database to be returned
   * @param {json} updatedData - The updared data that replaces the current data?
   * @returns {Promise<Object>} - The response from the server
   */
  async updateDatabase(databaseID, updatedData) // not sure how update works
  {
      if (!databaseID) 
      {
          throw new Error('Database id is required');
      }
      if (!updatedData) 
      {
          throw new Error('updated database data is required');
      }

      return this.request(`/databases/${databaseID}`, 'PUT', updatedData); 
  }

//collection operations
// createCollection: /databases/:databaseID/collections     (POST)   -> .emit("addCommand", "create", data)
/**
* Creates a new collection in a database
* 
* @param {string} databaseID - The name of the database to create the collection in
* @param {string} collectionName - The name of the collection to create
* @returns {Promise<Object>} - The response from the server
*/
  async createCollection(databaseID, collectionName) 
  {
      if (!databaseID) 
          {
          throw new Error('Database name is required');
      }

      if (!collectionName) 
          {
          throw new Error('Collection name is required');
      }

      // Construct the nested data structure that the daemon expects
      const data = {
          databaseID: {
              collectionName: {}  
          }
      };

      // Send the request to API create a collection
      return this.request(`/databases/${databaseID}/collections`, 'POST', data);
  }
// dropCollection: /databases/:databaseID/collections/:name (DELETE) -> .emit("addCommand", "delete", data)
  /**
   * Deletes the selected collection
   * 
   * @param {string} databaseID - the name of the database the collection is being deleted from
   * @param {string} collectionName - the name of the collection being deleted
   * @returns {Promise<Object>} - the response from the server
   */
  async dropCollection(databaseID,collectionName)
  {
      if (!databaseID) 
      {
          throw new Error('Database id is required');
      }

      if (!collectionName) 
      {
          throw new Error('Collection name is required');
      }

      // Send the request to API create a collection
      return this.request(`/databases/${databaseID}/collections/${collectionName}`, 'DELETE');
  }
// getCollection:  /databases/:databaseID/collections/:name (GET)    -> .emit("addCommand", "read", data)
  /**
   * Retrieves the selected collection
   * 
   * @param {string} databaseID - the name of the database the collection is being retrieved from
   * @param {string} collectionName - the name of the collection being retrieved
   * @returns {Promise<Object>} - the response from the server
   */
  async getCollection(databaseID,collectionName)
  {
      if (!databaseID) 
      {
          throw new Error('Database id is required');
      }

      if (!collectionName) 
      {
          throw new Error('Collection name is required');
      }

      // Send the request to API create a collection
      return this.request(`/databases/${databaseID}/collections/${collectionName}`, 'GET');
  }
// getAllCollections: /databases/:databaseID/collections    (GET)    -> .emit("addCommand", "read", data)
  /**
   * Retrieves all the collections of a database
   * 
   * @param {string} databaseID - the name of the database the collections are being retrieved from
   * @returns {Promise<Object>} - the response from the server
   */
  async getAllCollections(databaseID)
  {
      if (!databaseID) 
      {
          throw new Error('Database id is required');
      }

      // Send the request to API create a collection
      return this.request(`/databases/${databaseID}/collections/`, 'GET');
  }
// UpdateCollection: /databases/:databaseID		 (PUT)    -> .emit("addCommand", "update",data)
  /**
  * updates the collection of a database
  * 
  * @param {string} databaseID - the name of the database containing the collection being updated
  * @returns {Promise<Object>} - the response from the server
  */
     async updateCollecion(databaseID,updatedCollection)
     {
         if (!databaseID) 
         {
             throw new Error('Database id is required');
         }
         if(!updatedCollection)
         {
              throw new Error('Updated collection data is required')
         }
         // Send the request to API create a collection
         return this.request(`/databases/${databaseID}/collections/`, 'PUT',updatedCollection);
     }
//document operations
//createDocument: /databases/:databaseID/collections/:collection/documents     (POST)    -> .emit("addCommand", "create", data)
  /**
   * Creates a new document in a collection in a database
   * 
   * @param {string} databaseID - The name of the database to create the document in
   * @param {string} collectionName - The name of the collection to create the document in
   * @param {string} documentName - The name of the document created
   * @returns {Promise<Object>} - The response from the server
   */
  async createDocument(databaseID, collectionName, documentName) 
  {
      if (!databaseID) 
          {
          throw new Error('Database id is required');
      }

      if (!collectionName) 
          {
          throw new Error('Collection name is required');
      }

      if(!documentName)
      {
          throw new Error('Document name is required');
      }

      // Construct the nested data structure that the daemon expects
      const data = {
          databaseID: {
              collectionName: 
              {
                  documentName: {}
              }  
          }
      };

      // Send the request to API create a collection
      return this.request(`/databases/${databaseID}/collections/${collectionName}/documents`, 'POST', data);
  }

//deleteDocument: /databases/:databaseID/collections/:collection/documents/:id (DELETE)  -> .emit("addCommand", "delete", data)
  /**
   * Deletes a document in a collection in a database
   * 
   * @param {string} databaseID - The name of the database containing the document
   * @param {string} collectionName - The name of the collection containing the document
   * @param {string} documentName - The name of the document deleted
   * @returns {Promise<Object>} - The response from the server
   */
  async deleteDocument(databaseID, collectionName, documentName) 
  {
      if (!databaseID) 
          {
          throw new Error('Database id is required');
      }

      if (!collectionName) 
          {
          throw new Error('Collection name is required');
      }

      if(!documentName)
      {
          throw new Error('Document name is required');
      }

      // Send the request to API create a collection
      return this.request(`/databases/${databaseID}/collections/${collectionName}/documents/${documentName}`, 'DELETE');
  }
//getDocument:    /databases/:databaseID/collections/:collection/documents/:id (GET)     -> .emit("addCommand", "read", data)
  /**
   * retrieves a document in a collection in a database
   * 
   * @param {string} databaseID - The name of the database containing the document
   * @param {string} collectionName - The name of the collection containing the document
   * @param {string} documentName - The name of the document retireved
   * @returns {Promise<Object>} - The response from the server
   */
  async getDocument(databaseID, collectionName, documentName) 
  {
      if (!databaseID) 
          {
          throw new Error('Database id is required');
      }

      if (!collectionName) 
          {
          throw new Error('Collection name is required');
      }

      if(!documentName)
      {
          throw new Error('Document name is required');
      }

      // Send the request to API create a collection
      return this.request(`/databases/${databaseID}/collections/${collectionName}/documents/${documentName}`, 'GET');
  }
//getAllDocuments:/databases/:databaseID/collections/:collection/documents     (GET)     -> .emit("addCommand", "read", data)
  /**
  * retrieves all documents in a collection in a database
  * 
  * @param {string} databaseID - The name of the database containing the documents
  * @param {string} collectionName - The name of the collection containing the documents
  * @returns {Promise<Object>} - The response from the server
  */
  async getAllDocuments(databaseID, collectionName) 
  {
      if (!databaseID) 
          {
          throw new Error('Database id is required');
      }
      if (!collectionName) 
          {
          throw new Error('Collection name is required');
      }

      // Send the request to API create a collection
      return this.request(`/databases/${databaseID}/collections/${collectionName}/documents`, 'GET');
  }
//updateDocument: /databases/:databaseID/collections/:collection/documents/:id (PUT)     -> .emit("addCommand", "update", data)
  /**
   * retrieves a document in a collection in a database
   * 
   * @param {string} databaseID - The name of the database containing the document
   * @param {string} collectionName - The name of the collection containing the document
   * @param {string} documentName - The name of the document retireved
   * @param {json} updatedDocument - The updated data recieved from user
   * @returns {Promise<Object>} - The response from the server
   */
  async updateDocument(databaseID, collectionName, documentName,updatedDocument) 
  {
      if (!databaseID) 
          {
          throw new Error('Database id is required');
      }

      if (!collectionName) 
          {
          throw new Error('Collection name is required');
      }

      if(!documentName)
      {
          throw new Error('Document name is required');
      }

      if(!updatedDocument)
      {
          throw new Error('Updated data is required');
      }
      // Send the request to API create a collection
      return this.request(`/databases/${databaseID}/collections/${collectionName}/documents/${documentName}`, 'PUT',updatedDocument);
  }
  
      
}

// Export the class so it can be used in other files
module.exports = APIWrapper;