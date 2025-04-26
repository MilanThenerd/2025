import command from "./command.js";
import StorageEngine from "../StorageEngine.js";
import logHandler from "../logHandler.js";  
class createCommand extends command {
    /**
     * Calls the parent constructor
     */
    constructor(data, socket) {
        super(data, socket);
    }
    /**
     * Executes the command and<br> emits a response to the socket
     * @returns {Object} returns a response code and message
     */
    async execute() {
        let responseCode = 200;
        let databases = [];
        let countDb = 0;
        let countCollection = 0;
        let countDocument = 0;
        const dataKeys = Object.keys(this.data);

        if (dataKeys.length > 0) {
            databases = dataKeys.map((db) => {
                const nestedObj = this.data[db];
                const collections = Object.keys(nestedObj || {}).map((collection) => {
                    const docs = nestedObj[collection];
                    const documents = Object.entries(docs).map(([, value]) => value);
                    return { collection, documents };
                });
                return { db, collections };
            });
        }

        for (const { db, collections } of databases) {
            let res = await StorageEngine.create(db);
            if (res.created.length > 0) {
                countDb++;
            }
            for (const { collection, documents } of collections) {
                let res = await StorageEngine.create(db, collection);
                if (res.created.length > 0) {
                    countCollection++;
                }
                for (const doc of documents) {
                    let res = await StorageEngine.create(db, collection, doc);
                    if (res.created.length > 0) {
                        countDocument++;
                    }
                }
            }
        }


        let message = `Successfully created ${countDb} databases, ${countCollection} collections, and ${countDocument} documents`;
        let response = {
            response: responseCode,
            type: 'create',
            message: message,
        };

        this.socket?.emit("response", response);
        logHandler.addLog(this.data, "createCommand" , responseCode);

        return response;
    }
}
export default createCommand;
