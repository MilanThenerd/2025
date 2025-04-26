import command from './command.js';
import StorageEngine from '../StorageEngine.js';
import logHandler from '../logHandler.js';

const OPS = ["==", "!=", ">", "<", ">=", "<="];
class deleteCommand extends command {
    /**
     * Calls the parent constructor
     */
    constructor(data, socket) {
        super(data, socket);
    }

    /**
     * Executes the command and<br> emits a response to the socket
     * @returns {Object} returns a response code and message containing the number of records deleted
     */
    async execute() {
        let responseCode = 200;
        let databases = [];
        let countDb = 0;
        let countCollection = 0;
        let countDocument = 0;
        const dataKeys = Object.keys(this.data);
        
          // 🟡 Handle DB-level equality (==, !=)
        if (dataKeys.length === 1 && ["==", "!="].includes(dataKeys[0])) {
             const { dbCount } = await deleteCommand.processDbLevelEquality(this.data);
              countDb += dbCount;
            return this.emitDone(countDb, countCollection, countDocument);
        }
        
        if (dataKeys.length > 0) {
            databases = dataKeys.map((db) => {
                const nestedObj = this.data[db];
                const collections = Object.keys(nestedObj || {}).map((collectionKey) => {
                    const docs = nestedObj[collectionKey];

                    // 🟡 Collection-level equality
                    if (["==", "!="].includes(collectionKey)) {
                        return { db, op: collectionKey, collName: docs };
                    }
                     // 🟡 Regex deletion pattern
                     if (
                        docs &&
                        typeof docs === "object" &&
                        "collectionKey" in docs &&
                        "docsObj" in docs
                    ) {
                        return docs;
                    }
                    // 🟡 Document field operator (e.g. >, <, ==, etc.)
                      if (
                        docs &&
                        typeof docs === "object" &&
                        docs.$field &&
                        Object.keys(docs).some((k) => OPS.includes(k))
                    ) {
                        return { collection: collectionKey, operatorSpec: docs };
                    }
                    //check if collectionKey and docsObj 
                    if (
                      docs &&
                      typeof docs === "object" &&
                      "collectionKey" in docs &&
                      "docsObj" in docs
                    ) {
                      return docs;
                    }
                    // Regular deletion branch 
                    const documents = Object.entries(docs).map(([key]) => key);
                    return { collection: collectionKey, documents };
                });
                return { db, collections };
            });
        }
        
        for (const { db, collections } of databases) {
            if (collections.length === 0) {
                console.log(`Deleting database ${db}`);
                try {
                    await StorageEngine.delete(db);
                    countDb++;
                } catch (error) {
                  console.error('Error deleting database:', error);
                }
                continue;
            }
            for (const spec of collections) {

                /* ────────── collection‑level == / != ────────── */
                if ("op" in spec && "collName" in spec) {
                  const { count } = await deleteCommand.processCollectionLevelEquality(
                                      spec.db, spec.op, spec.collName);
                  countCollection += count;
                  continue;
                }
              
                /* ────────── regex‑based deletions (collections OR docs) ────────── */
                if (spec && typeof spec === "object" &&
                    "collectionKey" in spec && "docsObj" in spec) {
              
                  const { countCollection: cColl,
                          countDocument  : cDoc } = await deleteCommand.processRegexDeletion(db, spec);
              
                  countCollection += cColl;
                  countDocument   += cDoc;
                  continue;                           
                }
              
                /* ────────── field‑operator (>, <, ==, …) deletions ────────── */
                if ("collection" in spec && "operatorSpec" in spec) {
                  const { collection, operatorSpec } = spec;
                  const matchedDocs = await StorageEngine.getDocumentsByOperator(
                                        db, collection, operatorSpec);
              
                  for (const doc of matchedDocs) {
                    await StorageEngine.delete(db, collection, doc._id);
                    countDocument++;
                  }
                  continue;
                }
              
                /* ────────── regular deletions ────────── */
                if ("collection" in spec && "documents" in spec) {
                  const { collection, documents } = spec;
              
                  if (documents.length === 0) {
                    await StorageEngine.delete(db, collection);
                    countCollection++;
                    continue;
                  }
              
                  for (const doc of documents) {
                    await StorageEngine.delete(db, collection, doc);
                    countDocument++;
                  }
                  continue;
                }
              
              
                console.warn("Unrecognized collection specification, skipping:", spec);
              }
              
          
  
        }

        logHandler.addLog(this.data, "deleteCommand" , responseCode);
        return this.emitDone(countDb, countCollection, countDocument);
    }
   
    emitDone(dbCnt, collCnt, docCnt) {
        let msg = `Successfully deleted ${dbCnt} databases, ${collCnt} collections, and ${docCnt} documents`;
        let responseCode = 200;
        if (dbCnt === 0 && collCnt === 0 && docCnt === 0) {
            responseCode = 404;
            msg = "No records found to delete";
        }
        let resp = {
            response: responseCode,
            type: 'delete',
            message: msg
        };        
        this.socket?.emit("response", resp);
        return resp;
    }
    
    static async processDbLevelEquality(data) {
        let dbCount = 0;
        const [op] = Object.keys(data);
        const value = data[op];
        const dbs = await StorageEngine.filterDatabasesByOp(op, value);
        for (const db of dbs) {
            await StorageEngine.delete(db);
            dbCount++;
        }
        return { dbCount };
    }

    static async processCollectionLevelEquality(db, op, collName) {
        let count = 0;
        const collections = await StorageEngine.filterCollectionsByOp(db, op, collName);
        for (const col of collections) {
            await StorageEngine.delete(db, col);
            count++;
        }
        return { count };
    }

    static async processRegexDeletion(db, spec) {
        let countCollection = 0;
        let countDocument = 0;

        const { collectionKey, docsObj } = spec;
        const colls = await StorageEngine.getMatchingCollections(db, collectionKey);

        for (const c of colls) {
            if (Object.keys(docsObj).length === 0) {
                await StorageEngine.delete(db, c);
                countCollection++;
                continue;
            }

            const field = docsObj.$field || null;
            if (!field) continue;

            for (const pat of Object.keys(docsObj)) {
                if (pat === "$field") continue;
                const docs = await StorageEngine.getMatchingDocuments(db, c, pat, field);
                for (const d of docs) {
                    await StorageEngine.delete(db, c, d._id);
                    countDocument++;
                }
            }
        }

        return { countCollection, countDocument };
    }
}
export default deleteCommand;
