import StorageEngine from '../StorageEngine.js';
// function testImport() {
//     readFile('./daemon/Tests/db.json', (err, data) => {
//         const text = data.toString('utf-8');

//         const lines = text.split(/\r?\n/);
//         lines.forEach((line) => {
//             let parsedData;
//             try {
//                 parsedData = JSON.parse(line);
//             } catch (error) {
//                 console.error('Error parsing JSON:', error);
//                 return;
//             }
//             let command = new createCommand(parsedData, socket);
//             command.execute();
//         });
//     });
// }

async function testExport() {
    let returnData = await StorageEngine.export();
    console.log(returnData);
}
testExport();