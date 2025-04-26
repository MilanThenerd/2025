import updateCommand from "../../src/command/updateCommand.js";
import createCommand from "../../src/command/createCommand.js";
import deleteCommand from "../../src/command/deleteCommand.js";
import command from "../../src/command/command.js";

let passed = 0;
let failed = 0;

async function runTest(commandClass, expectedMessage, shouldThrow = false) {
    try {
        let cmd = new commandClass("");
        let result = await cmd.execute();

        if (shouldThrow) {
            console.log("\x1b[31mTest failed", "\x1b[0m");
            failed++;
            return;
        }

        if (result.message.includes(expectedMessage)) {
            console.log("\x1b[32mTest passed\x1b[0m");
            passed++;
        } else {
            console.log("\x1b[31mTest failed", "\x1b[0m");
            failed++;
        }

    } catch (e) {
        if (shouldThrow) {
            passed++;
        } else {
            console.log("\x1b[31mTest failed", "\x1b[0m");
            failed++;
        }
    }
}

async function commandTesting() {
    await runTest(createCommand, "create");
    await runTest(updateCommand, "updated");
    await runTest(deleteCommand, "deleted");
    await runTest(command, "", true);
}

(async () => {
    await commandTesting();
    console.log(`\x1b[32mPassed: ${passed}, Failed: ${failed}\x1b[0m`);
})();