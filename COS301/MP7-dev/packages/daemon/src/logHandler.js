import path from "path";
import fs from "fs";
class logHandler {
    static dir = path.join(process.cwd(), "src/log/db.log");
    static async addLog(data, type, responseCode) {
        let logData = logHandler.getContent(data, type, responseCode);
        try {
            await fs.promises.appendFile(logHandler.dir, logData + "\n");
            console.log(`Log updated successfully`);
        } catch (err) {
            console.error(`Error updating log: ${err}`);
        }

    }

    static getContent(data, type, responseCode) {
        let timestamp = new Date().getTime().toString(36);
        data = JSON.stringify(data);
        data = data.replace(/:/g, "");
        data = data.replace(/"/g, "");
        data = data.replace(/ /g, "");
        return "time:[" + timestamp + "]    action:[" + type + "]  responseCode:["+ responseCode +"]    data:[" + data + "]";
    }


}

export default logHandler;