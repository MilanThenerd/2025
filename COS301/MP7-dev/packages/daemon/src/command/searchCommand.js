import command from './command.js';
import StorageEngine from '../StorageEngine.js';

const OPS = ["==", "!=", ">", "<", ">=", "<="];
class searchCommand extends command {
  /**
   * Calls the parent constructor
   */
  constructor(data, socket) {
    super(data, socket);
  }

  /**
   * Executes the command and<br> emits a response to the socket
   * @returns {Object} returns a response code, message, and the search results
   */
  async execute() {
    let responseCode = 200;
    let message = "";
    const results = [];
    let totalResults = 0;

    console.log(data);
    const topKeys = Object.keys(this.data.data);   // DB names or top‑level operator
    const pageNumber = this.data.pageNumber || 1;
    const limit = this.data.limit || 10;
    const databases = topKeys;                  // shorthand


    if (topKeys.length === 1 && OPS.slice(0, 2).includes(topKeys[0])) {
      const { dbs } = await searchCommand.processDbLevelEquality(this.data.data);
      dbs.forEach(db =>
        results.push({ database: db, collections: [], documents: [], count: 0 })
      );
      return this.emitDone(200, `Matched ${dbs.length} database(s).`, results,pageNumber,limit);
    }


    if (databases.length === 0) {
      responseCode = 400;
      message = "No database specified for search";
    } else {
      for (const dbName of databases) {
        const collObj = this.data.data[dbName] || {};
        const collKeys = Object.keys(collObj);

        if (collKeys.length === 0) {
          responseCode = 400;
          message = `No collection specified for database '${dbName}'`;
          continue;
        }

        for (const collKey of collKeys) {
          const query = collObj[collKey] || {};

          try {

            if (OPS.slice(0, 2).includes(collKey)) {
              const { collections } = await searchCommand.processCollectionLevelEquality(
                dbName, collKey, query
              );
              collections.forEach(c =>
                results.push({ database: dbName, collection: c, documents: [], count: 0 })
              );
              totalResults += collections.length;
              continue;                        // next collKey
            }

            let docs = [];

            // 1️⃣ comparison operators (>, <, ==, …)
            if (query.$field && Object.keys(query).some(k => OPS.includes(k))) {
              docs = await StorageEngine.getDocumentsByOperator(dbName, collKey, query);
            }
            // 2️⃣ regex payload ({ "^John":{}, "$field":"username" })
            else if (query.$field) {
              docs = await StorageEngine.searchByRegex(dbName, collKey, query);
            }
            // 3️⃣ literal / full‑collection search
            else {
              docs = await StorageEngine.search(dbName, collKey, query);
            }

            results.push({
              database: dbName,
              collection: collKey,
              documents: docs,
              count: docs.length
            });
            totalResults += docs.length;

          } catch (err) {
            console.error(`Search error for ${dbName}.${collKey}:`, err);
            results.push({ database: dbName, collection: collKey, error: err.message });
            if (responseCode === 200) { responseCode = 404; message = err.message; }
          }
        }
      }

      if (responseCode === 200) {
        message = `Search completed successfully. Found ${totalResults} matching document(s).`;
      }
    }


    return this.emitDone(responseCode, message, results, pageNumber, limit);
  }


  emitDone(code, message, results, pageNumber, limit) {
    let start = (pageNumber - 1) * limit;
    let end = start + limit;
    results = results.slice(start, end);
    const response = { response: code, message, type: "search", results };
    this.socket?.emit("response", response);
    return response;
  }


  /**
  * Processes the database-level equality/inequality search
  * @param {Object} data - The search data
  * @returns {Promise<Object>} - The databases that match the criteria
  */
  static async processDbLevelEquality(data) {
    const [op] = Object.keys(data);
    const value = data[op];
    const dbs = await StorageEngine.filterDatabasesByOp(op, value);
    return { dbs };
  }

  static async processCollectionLevelEquality(db, op, value) {
    const collections = await StorageEngine.filterCollectionsByOp(db, op, value);
    return { collections };
  }
}
export default searchCommand;
