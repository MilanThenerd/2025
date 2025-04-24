import { createServer } from 'http';
import { Server } from 'socket.io';
import createCommand from './command/createCommand.js';
import updateCommand from './command/updateCommand.js';
import deleteCommand from './command/deleteCommand.js';
import searchCommand from './command/searchCommand.js';
import listCommand from './command/listCommand.js';
import { Mutex } from 'async-mutex';
import StorageEngine from './StorageEngine.js';
import fs from 'fs';
import path from 'path';
import crypto from 'crypto';  

class mpdbdaemon {
    port;
    server;
    io;
    connections; // set of active socket connections

    constructor(port = 8008) {
        this.port = port;
        this.server = null;
        this.io = null;
        this.connections = new Set();

        //concurrency control
        this.userQueue = {};
        this.resourceLocks = {};
        this.userLocks = {};
    }

    async start() {
        this.server = createServer();
        this.io = new Server(this.server); // creating a new socket io server in the http server
        this.io.on('connection', (socket) => {
            this.connections.add(socket);
            console.log('Client connected');

            socket.on('addCommand', (type, data) => { //event listener for the add command , when it is received it will add the socket, command type and data to the queue
                console.log('Type: ', type);
                console.log('Data: ', JSON.stringify(data));
                this.addToQueue(socket, type, data);
            });

            socket.on("login", (data) => {
                const filePath = path.join(process.cwd(), 'users.cf');
                const fileContent = fs.readFileSync(filePath, 'utf-8');
                const lines = fileContent.split('\n');
                let username = data.username;

                //let email = data.email;
                const hashedPassword = crypto.createHash('sha256').update(data.password).digest('hex');
                for (let line of lines) {
                    let [timestamp, storedusername, , storedPassword] = line.split('|');
                    if ((storedusername === username) && storedPassword === hashedPassword) {
                        socket.userId = timestamp;
                        socket.username = storedusername;
                        socket.emit('response', { response: 200, message: 'Login successful' });
                        return;
                    }
                }
                socket.emit('response', { response: 500, message: 'Login failed' });
            });

            socket.on("register", (data) => {

                const filePath = path.join(process.cwd(), 'users.cf');
                const fileContent = fs.readFileSync(filePath, 'utf-8');
                let username = data.username;
                let email = data.email;
                const lines = fileContent.split('\n');
                for (let line of lines) {
                    if (line.includes(username) || line.includes(email)) {
                        socket.emit('response', { response: 500, message: 'User already exists' });
                        return;
                    }
                }
                const hashedPassword = crypto.createHash('sha256').update(data.password).digest('hex');
                let timestamp = new Date().getTime().toString(36);
                fs.appendFileSync(filePath, timestamp + "|" + data.username + "|" + data.email + "|" + hashedPassword + '\n', 'utf-8');
                
                const permissionsPath = path.join(process.cwd(), 'permissions.cf');
                let permissionsData = {};
                
                if (fs.existsSync(permissionsPath)) {
                    try {
                        const permissionsContent = fs.readFileSync(permissionsPath, 'utf-8');
                        permissionsData = JSON.parse(permissionsContent);
                    } catch (error) {
                        console.error('Error reading permissions file:', error);
                        permissionsData = {};
                    }
                }
                
                permissionsData[timestamp] = {
                    role: "user",
                    databases: {}
                };
                
                fs.writeFileSync(permissionsPath, JSON.stringify(permissionsData, null, 2));
                
                socket.emit('response', { response: 200, message: 'User registered successfully' });
            });

            socket.on('setPermission', (data) =>{
                const userId = socket.userId;
                if (!userId || !this.isAdmin(userId)) {
                    socket.emit('response', { 
                        response: 403, 
                        message: 'Only administrators can modify permissions' 
                    });
                    return;
                }
                
                const { targetUser, database, permissions } = data;

                if (!targetUser || !database || !permissions) {
                    socket.emit('response', {
                        response: 400,
                        message: 'Missing required fields: targetUser, database, permissions'
                    });
                    return;
                }

                if (Array.isArray(permissions)) {
                    const validPermissions = ['read', 'write', 'delete'];
                    const invalidPermissions = permissions.filter(p => !validPermissions.includes(p));
                    
                    if (invalidPermissions.length > 0) {
                        socket.emit('response', {
                            response: 400,
                            message: `Invalid permissions: ${invalidPermissions.join(', ')}`
                        });
                        return;
                    }
                } else if (permissions !== '*') {
                    socket.emit('response', {
                        response: 400,
                        message: 'Permissions must be an array or "*"'
                    });
                    return;
                }
                
                const result = this.updateUserPermissions(targetUser, database, permissions);
                socket.emit('response', result);
            });

            socket.on('export', (data) => {
                let returnData = StorageEngine.export(data);
                let buffer = Buffer.from(JSON.stringify(returnData), 'utf-8');
                socket.emit('export', buffer);
            });

            socket.on('import', (data) => {
                const buffer = data;
                const text = buffer.toString('utf-8');

                const lines = text.split(/\r?\n/);
                lines.forEach((line) => {
                    let parsedData;
                    try {
                        parsedData = JSON.parse(line);
                    } catch (error) {
                        console.error('Error parsing JSON:', error);
                        return;
                    }
                    let command = new createCommand(parsedData, socket);
                    command.execute();
                });
            });

            socket.on('disconnect', () => {
                this.connections.delete(socket); //removing the socket from the set of active connections
                console.log('Client disconnected: ${socket.id}');
                //cleaning the queue and locks for the socket
                delete this.userQueue[socket.id];
                delete this.userLocks[socket.id];
            });

            socket.on('error', (error) => {
                console.log('A socket error occurred: ', error);//
                this.connections.delete(socket);
            });
        });

        this.server.listen(this.port, () => {
            console.log('MPDB daemon listening on port ', this.port);
        });

        this.server.on('error', (err) => {
            console.error('Server error:', err);
        });
    }

    stop() {
        return new Promise((resolve) => {
            // Close all socket connections
            if (this.connections && this.connections.length) {
                console.log(`Closing ${this.connections.length} socket connections...`);
                for (const socket of this.connections) {
                    socket.emit('shutDown');
                    socket.disconnect(true);
                }
                this.connections = [];
            }

            // Cancel Timers
            if (this.queueTimer) {
                clearInterval(this.queueTimer);
                this.queueTimer = null;
            }

            // Close the io server
            if (this.io) {
                console.log('Closing Socket.IO server...');
                this.io.close();
                this.io = null;
            }

            // Close HTTP server
            if (this.server) {
                console.log('Closing HTTP server...');

                // Clear connection checking interval that's keeping the process alive
                const checkConnectionsInterval = this.server[Symbol('http.server.connectionsCheckingInterval')];
                if (checkConnectionsInterval) {
                    clearInterval(checkConnectionsInterval);
                }

                // Close the server and resolve once completed
                this.server.close(() => {
                    console.log('HTTP server closed');
                    this.server = null;
                    resolve();
                });
            } else {
                resolve();
            }
        });
    }

    async addToQueue(socket, type, data) {
        const userId = socket.id;

        let newCommand;
        switch (type) {
            case 'create':
                newCommand = new createCommand(data, socket);
                break;
            case 'update':
                newCommand = new updateCommand(data, socket);
                break;
            case 'delete':
                newCommand = new deleteCommand(data, socket);
                break;
            case 'search':
                newCommand = new searchCommand(data, socket);
                break;
            case 'list':
                newCommand = new listCommand(data, socket);
                break;
            default:
                console.log(`Invalid command type: ${type}`);
                return;
        }

        if (!this.userQueue[userId]) {
            this.userQueue[userId] = [];
        }
        this.userQueue[userId].push(newCommand);

        //process the user queue using the socket lock
        this.handle(socket);
    }

    async handle(socket) {
        const userId = socket.id;

        // Create a per-user lock if one does not exist
        if (!this.userLocks[userId]) {
            this.userLocks[userId] = new Mutex();
        }
        const userLock = this.userLocks[userId];

        await userLock.runExclusive(async () => {
            while (this.userQueue[userId] && this.userQueue[userId].length > 0) {
                const command = this.userQueue[userId].shift();
                await this.processCommand(command);
            }
        });
    }

    // multiple user concurrency control
    async processCommand(command) {
        console.log(`Processing command: ${command.constructor.name}`);
        console.log(`Socket has userId: ${command.socket.userId ? 'Yes' : 'No'}`);
        const resourceIds = this.getResourceIdsFromData(command.data);

        //Check user has been authenticated
        const userId = command.socket.userId;

        // No need for auth for login and register
        const commandType = command.constructor.name.replace('Command', '').toLowerCase();
        const authExemptCommands = ['login', 'register'];

        if (!userId && !authExemptCommands.includes(commandType)) {
            command?.socket?.emit('response', {
                response: 401,
                message: 'Authentication required',
            });
            return;
        }

        if (userId && !authExemptCommands.includes(commandType)) {
            const hasPermission = await this.checkPermission(userId, commandType, resourceIds);
            if (!hasPermission) {
                command?.socket?.emit('response', {
                    response: 403, 
                    message: 'Access denied: Insufficient permissions',
                });
                return;
            }
        }
        
        // Sort them to avoid deadlocks
        resourceIds.sort();

        const acquiredLocks = [];
        try {
            // Acquire each resource lock in sorted order
            for (const rId of resourceIds) {
                if (!this.resourceLocks[rId]) {
                    this.resourceLocks[rId] = new Mutex();
                }
                const lock = this.resourceLocks[rId];

                await lock.acquire();
                acquiredLocks.push(lock);
            }

            // Execute the command (which  calls StorageEngine)
            const response = await command.execute();
            return response;
        } catch (error) {
            console.error('Error processing command:', error);
            command?.socket?.emit('response', {
                response: 500,
                message: 'Command execution failed',
            });
        } finally {
            // Release locks
            for (let i = acquiredLocks.length - 1; i >= 0; i--) {
                acquiredLocks[i].release();
            }
        }
    }

    async checkPermission(userId, commandType, resourceIds) {
        const permissionsPath = path.join(process.cwd(), 'permissions.cf');
        
        // Create if it doesn't exist
        if (!fs.existsSync(permissionsPath)) {
            const defaultPermissions = {
                "m9t1v9fc": {
                    role: "admin",
                    databases: "*"
                }
            };
            fs.writeFileSync(permissionsPath, JSON.stringify(defaultPermissions, null, 2));
        }
        
        const permissionsContent = fs.readFileSync(permissionsPath, 'utf-8');
        const permissions = JSON.parse(permissionsContent);
        
        const userPermissions = permissions[userId];
        if (!userPermissions) {
            return false; // No permissions defined
        }

        // Role types
        if (userPermissions.role === "admin") {
            return true;
        }
        if (userPermissions.databases === "*") {
            return true;
        }

        if (commandType === "create" && this.isCreatingNewDatabase(resourceIds)) {
            return true;
        }
        
        // For each DB check permission
        for (const resourceId of resourceIds) {
            const dbName = resourceId.split('/')[0];
            
            const dbPermissions = userPermissions.databases[dbName];
            if (!dbPermissions) {
                return false;
            }
            
            // Check command type against allowed operations
            if (commandType === "search" || commandType === "list") {
                if (!dbPermissions.includes("read")) {
                    return false;
                }
            } else if (commandType === "create" || commandType === "update" || commandType === "delete") {
                if (!dbPermissions.includes("write")) {
                    return false;
                }
            }
        }
        
        return true; // User has all required permissions
    }

    // Helper method to check if user is admin
    isAdmin(userId) {
        const permissionsPath = path.join(process.cwd(), 'permissions.cf');
        if (!fs.existsSync(permissionsPath)) {
            return false;
        }
        
        const permissionsContent = fs.readFileSync(permissionsPath, 'utf-8');
        const permissions = JSON.parse(permissionsContent);
        
        return permissions[userId]?.role === "admin";
    }

    isCreatingNewDatabase(resourceIds){  //TODO
        return resourceIds.some(id => !id.includes('/'));
    }

    // Helper method to update permissions
    updateUserPermissions(user, database, permissions) {
        const permissionsPath = path.join(process.cwd(), 'permissions.cf');
        if (!fs.existsSync(permissionsPath)) {
            return { 
                response: 500, 
                message: 'Permissions file not found' 
            };
        }
        
        const permissionsContent = fs.readFileSync(permissionsPath, 'utf-8');
        const permissionsData = JSON.parse(permissionsContent);
        
        if (!permissionsData[user]) {
            // Initialize user
            permissionsData[user] = {
                role: "user",
                databases: {}
            };
        }
        
        // Update database permissions
        if (database === "*") {
            permissionsData[user].databases = "*";
        } else {
            if (permissionsData[user].databases === "*") {
                permissionsData[user].databases = {};
            }
            
            permissionsData[user].databases[database] = permissions;
        }
        fs.writeFileSync(permissionsPath, JSON.stringify(permissionsData, null, 2));
        
        return { 
            response: 200, 
            message: 'Permissions updated successfully' 
        };
    }

    async handleRequests(socket, command) {
        console.log('Handling requests');
        socket.emit('response', 'Received request');
        let test = new createCommand('Data');
        console.log(test.execute());
        while (command) {
            let response = command.execute();
            console.log('Sending response: ', response);
            socket.emit('response', response);
            command = command.next;
        }
    }

    getResourceIdsFromData(commandData = {}) {
        let resourceIds = [];

        for (const dbNameRef of Object.keys(commandData)) {
            // The top-level key might be myDB or myDB#reference
            const dbName = dbNameRef.split('#')[0];
            const dbValue = commandData[dbNameRef];

            // If dbValue is empty == operating on the DB
            if (
                typeof dbValue === 'object' &&
                dbValue !== null &&
                Object.keys(dbValue).length === 0
            ) {
                resourceIds.push(dbName);
                continue;
            }

            // If dbValue is a string == likely renaming DB
            if (typeof dbValue === 'string') {
                resourceIds.push(dbName);
                continue;
            }

            // Otherwise== parse collections
            for (const collNameRef of Object.keys(dbValue)) {
                const collName = collNameRef.split('#')[0];
                const collValue = dbValue[collNameRef];

                // If collValue is empty ===operating on the collection
                if (
                    typeof collValue === 'object' &&
                    collValue !== null &&
                    Object.keys(collValue).length === 0
                ) {
                    resourceIds.push(`${dbName}/${collName}`);
                    continue;
                }

                // If its a string == likely renaming a collection
                if (typeof collValue === 'string') {
                    resourceIds.push(`${dbName}/${collName}`);
                    continue;
                }

                // Otherwise parse doc references
                for (const docRef of Object.keys(collValue)) {
                    const docId = docRef.split('#')[0];
                    resourceIds.push(`${dbName}/${collName}/${docId}`);
                }
            }
        }

        return resourceIds;
    }
}

const daemon = new mpdbdaemon();
daemon.start();
StorageEngine.start();
export default mpdbdaemon;
