import command from './command.js';
import StorageEngine from '../StorageEngine.js';

class listCommand extends command {
  /**
   * Calls the parent constructor
   */
  constructor(data, socket) {
    super(data, socket);
  }
  /**
   * Executes the command and<br> emits a response to the socket
   * @returns {Object} returns a response code, message and data for all retrieved records
   */

  async execute() {
    let responseCode = 200;
    let arr = {};
    if (this.data === '{}' || this.data === ' ') {
      this.data = '';
    }
    let db = '';
    let collection = '';

    const dataKeys = Object.keys(this.data);

    if (dataKeys.length > 0) {
      [db] = dataKeys;
      const nestedObj = this.data[db];

      if (nestedObj && typeof nestedObj === 'object') {
        const [firstCollection] = Object.keys(nestedObj);
        collection = firstCollection || '';
      }
    }

    arr = await StorageEngine.list(db, collection);
    
    let count = arr.length;
    let message = `Successfully listed ${count} records`;

    if (arr.length == 0) {
      responseCode = 404;
      message = 'No records found';
    }

    let response = {
      response: responseCode,
      type: 'list',
      message: message,
      data: arr,
    };

    this.socket?.emit('response', response);
    return response;
  }
}
export default listCommand;
