class command {
    /**
    *Constructor for abstract class
    * @param {String | Json} data The query data
    * @param {Socket} socket Keeps track of which socket to emit the response to
    */
    constructor(data, socket) {
        this.data = data;
        this.timestamp = Date.now();
        this.socket = socket;
    }
    /**
    * Abstract method to be implemented by child classes
    */
    async execute() {
        throw new Error("Parent class method called");
    }
}
export default command;