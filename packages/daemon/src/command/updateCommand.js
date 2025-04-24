import command from './command.js';
import StorageEngine from '../StorageEngine.js';
import logHandler from "../logHandler.js";  

class updateCommand extends command {
    /**
     * Calls the parent constructor
     */
    constructor(data, socket) {
        super(data, socket);
    }

    /**
     * Executes the command and emits a response to the socket
     * @returns {Object} returns a response code and message containing the number of records updated
     */
    async execute() {
        let responseCode = 200;
        let countDb = 0;
        let countCollection = 0;
        let countDocument = 0;
        let message = "";
        const dataKeys = Object.keys(this.data);
        try {
            for (const key of dataKeys) {
                const newValue = this.data[key];

                if (key.includes('#')) {
                    const [oldName] = key.split('#');

                    console.log(`Updating database ${oldName} to ${newValue}`);
                    await StorageEngine.update(newValue, oldName, null, null);
                    countDb++;
                }
                else if (typeof newValue === 'object') {
                    for (const collectionKey in newValue) {
                        const collectionValue = newValue[collectionKey];

                        if (collectionKey.includes('#')) {
                            const [oldCollection] = collectionKey.split('#');

                            console.log(`Updating collection ${oldCollection} to ${collectionValue}`);
                            await StorageEngine.update(collectionValue, key, oldCollection, null);
                            countCollection++;
                        }
                        else if (typeof collectionValue === 'object') {
                            for (const docKey in collectionValue) {
                                const docValue = collectionValue[docKey];

                                if (docKey.includes('#')) {
                                    const [oldDoc] = docKey.split('#');

                                    console.log(`Updating document ${oldDoc} to ${docValue}`);
                                    await StorageEngine.update(docValue, key, collectionKey, oldDoc);
                                    countDocument++;
                                }
                            }
                        }
                    }
                }
            }
            message = `Successfully updated ${countDb + countCollection + countDocument} records`;
        }
        catch (e) {
            responseCode = 500;
            message = "Database/Collection/Document not found";
        }

        let response = {
            response: responseCode,
            type: 'update',
            message: message
        };

        this.socket?.emit('response', response);
        logHandler.addLog(this.data, "updateCommand" , responseCode);

        return response;
    }
}

export default updateCommand;
