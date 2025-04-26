import fs from "fs/promises";
import path from "path";
import { randomBytes } from 'crypto';

/*
An updated Storage Engine idea that enables file chunking, storage partioning and indexing of id's and potentially other fiels
*/
class StorageEngine {
    static dataDir = path.join(process.cwd(), "data");

    static async start() {
        try {
            await fs.mkdir(StorageEngine.dataDir, { recursive: true });
            console.log(`Data directory created at ${StorageEngine.dataDir}`);
            return true;
        } catch (err) {
            console.error("Failed to create data directory:", err);
            throw err;
        }
    }

    static async create(databaseName, collectionName, document) {
        let result = { status: "success", message: "", created: [], documentId: null};
        let dbPath = "";

        try {
            if (!databaseName || typeof databaseName !== "string") {
                throw new Error("No valid database name provided");
            }

            if (collectionName && typeof collectionName !== "string") {
                throw new Error("Collection name must be a string");
            }

            if (document && typeof document !== "object") {
                throw new Error("Document must be a valid object");
            }

            if (document && !collectionName) {
                throw new Error("Database and Document provided but no Collection");
            }

            dbPath = path.join(StorageEngine.dataDir, databaseName);

            let dbExists = false;
            try {
                await fs.access(dbPath);
                dbExists = true;
            } catch (err) {
                //Database will be created
            }

            if (!dbExists) {
                await fs.mkdir(dbPath, { recursive: true });
                result.message += `Database '${databaseName}' created. `;
                result.created.push("database");
            } else {
                result.message += `Using database '${databaseName}'. `;
            }

            if (!collectionName) {
                return result;
            }

            const collectionPath = path.join(dbPath, collectionName);
            const indexFilePath = path.join(collectionPath, 'collection_index.json');

            let colExists = false;
            try {
                await fs.access(collectionPath);
                colExists = true;
            } catch (err) {
                //Collection will be created
            }

            if (!colExists) {
                //Create Collection
                await fs.mkdir(collectionPath, { recursive: true });
                result.message += `Collection '${collectionName}' created. `;
                result.created.push("collection");

                const initialIndex = {
                    metadata: {
                        name: collectionName,
                        created: new Date().toISOString(),
                        documentCount: 0,
                        chunkCount: 0
                    },
                    // Maps document IDs to chunk files
                    documentMap: {},
                    // Tracks chunk files and their stats
                    chunks: []
                };

                await fs.writeFile(
                    indexFilePath,
                    JSON.stringify(initialIndex, null, 2),
                    'utf8'
                );
            } else {
                result.message += `Using collection '${collectionName}'. `;
            }

            if (!document) {
                return result;
            }

            //Create Document
            document._id = StorageEngine.generateId();
            result.documentId = document._id;

            const indexRaw = await fs.readFile(indexFilePath, 'utf8');
            const index = JSON.parse(indexRaw);

            let targetChunk;
            if (index.chunks.length === 0) {
                // Ensure chunkCount is initialized properly
                index.metadata.chunkCount = index.metadata.chunkCount || 0;
                
                // Create a new chunk file with a proper number
                const chunkNumber = index.metadata.chunkCount + 1;
                targetChunk = `chunk_${chunkNumber}.dat`;
                
                const newChunk = {
                    header: {
                        documentCount: 0,
                        totalBytes: 0,
                        createdAt: new Date().toISOString(),
                        lastUpdated: new Date().toISOString()
                    },
                    index: {},
                    data: Buffer.alloc(0) // Empty buffer initially
                };

                await StorageEngine.writeChunkFile(path.join(collectionPath, targetChunk), newChunk);

                index.chunks.push({
                    name: targetChunk,
                    documentCount: 0,
                    totalBytes: 0,
                    createdAt: new Date().toISOString()
                });

                index.metadata.chunkCount = chunkNumber;
            } else {
                const latestChunk = index.chunks[index.chunks.length-1];
                
                // Check if we need to read the actual chunk file to get an accurate count
                try {
                    const chunkPath = path.join(collectionPath, latestChunk.name);
                    const fileBuffer = await fs.readFile(chunkPath);
                    const fileHeaderSize = 256;
                    const fileHeaderJson = fileBuffer.subarray(0, fileHeaderSize).toString().trim();
                    const fileHeader = JSON.parse(fileHeaderJson.replace(/\0+$/, ''));
                    
                    const headerBuffer = fileBuffer.subarray(
                        fileHeaderSize, 
                        fileHeaderSize + fileHeader.headerLength
                    );
                    const header = JSON.parse(headerBuffer.toString());
                    
                    // Update documentCount in our latestChunk object to match what's in the file
                    latestChunk.documentCount = header.documentCount;
                    latestChunk.totalBytes = header.totalBytes;
                } catch (err) {
                    console.error(`Failed to read chunk stats: ${err.message}`);
                    // Keep using the count we have, which might not be accurate
                }

                if (StorageEngine.isChunkFull(latestChunk)) {
                    // Current chunk full, create a new one
                    // Ensure chunkCount is initialized properly
                    index.metadata.chunkCount = index.metadata.chunkCount || 0;
                    
                    // Create a new chunk file with a proper number
                    const chunkNumber = index.metadata.chunkCount + 1;
                    targetChunk = `chunk_${chunkNumber}.dat`;
                    
                    const newChunk = {
                        header: {
                            documentCount: 0,
                            totalBytes: 0,
                            createdAt: new Date().toISOString(),
                            lastUpdated: new Date().toISOString()
                        },
                        index: {},
                        data: Buffer.alloc(0)
                    };

                    await StorageEngine.writeChunkFile(path.join(collectionPath, targetChunk), newChunk);

                    index.chunks.push({
                        name: targetChunk,
                        documentCount: 0,
                        totalBytes: 0,
                        createdAt: new Date().toISOString()
                    });

                    index.metadata.chunkCount = chunkNumber;
                } else {
                    // Use the existing chunk
                    targetChunk = latestChunk.name;
                }
            }

            const documentPosition = await StorageEngine.addDocumentToChunk(
                path.join(collectionPath, targetChunk),
                document
            );

            index.documentMap[document._id] = {
                chunk: targetChunk,
                // You might store additional info here
            };

            index.metadata.documentCount++;

            // Update the index file
            const chunkIndex = index.chunks.findIndex(chunk => chunk.name === targetChunk);
            if (chunkIndex !== -1) {
                // Read the chunk file to get accurate stats
                const chunkPath = path.join(collectionPath, targetChunk);
                try {
                    const fileBuffer = await fs.readFile(chunkPath);
                    const fileHeaderSize = 256;
                    const fileHeaderJson = fileBuffer.subarray(0, fileHeaderSize).toString().trim();
                    const fileHeader = JSON.parse(fileHeaderJson.replace(/\0+$/, ''));
                    
                    const headerBuffer = fileBuffer.subarray(
                        fileHeaderSize, 
                        fileHeaderSize + fileHeader.headerLength
                    );
                    const header = JSON.parse(headerBuffer.toString());
                    
                    // Update the index with accurate counts
                    index.chunks[chunkIndex].documentCount = header.documentCount;
                    index.chunks[chunkIndex].totalBytes = header.totalBytes;
                } catch (err) {
                    console.error(`Failed to read chunk stats: ${err.message}`);
                    // Fallback to incrementing
                    index.chunks[chunkIndex].documentCount++;
                    index.chunks[chunkIndex].totalBytes += documentPosition.length;
                }
            }

            await fs.writeFile(
                indexFilePath,
                JSON.stringify(index, null, 2),
                'utf8'
            );
            result.message += `Document with ID '${document._id}' created.`;
            result.created.push("document");

            return result;

        } catch (error) {
            console.error("Creation failed:", error);
            throw error;
        }
    }

    static isChunkFull(chunk) {
        // Ensure we're looking at numbers
        const docCount = parseInt(chunk.documentCount, 10) || 0;
        const bytes = parseInt(chunk.totalBytes, 10) || 0;
        
        if (docCount < 100 && bytes < 1048576) {
            return false;
        } else {
            return true;
        }
    }

    static async writeChunkFile(filePath, chunkData) {
        const headerJson = JSON.stringify(chunkData.header);
        const headerBuffer = Buffer.from(headerJson);

        const indexJson = JSON.stringify(chunkData.index);
        const indexBuffer = Buffer.from(indexJson);

        const fileHeader = {
            headerLength: headerBuffer.length,
            indexLength: indexBuffer.length
        }

        const fileHeaderJson = JSON.stringify(fileHeader);
        const fileHeaderBuffer = Buffer.from(fileHeaderJson);

        const fixedFileHeaderBuffer = Buffer.alloc(256);
        fileHeaderBuffer.copy(fixedFileHeaderBuffer);

        const completeBuffer = Buffer.concat([
            fixedFileHeaderBuffer,
            headerBuffer,
            indexBuffer,
            chunkData.data || Buffer.alloc(0)
        ]);

        await fs.writeFile(filePath, completeBuffer);

        return true;
    }

    static async addDocumentToChunk(filePath, document) {
        let fileBuffer;
        try {
            fileBuffer = await fs.readFile(filePath);
        } catch (err) {
            // File doesn't exist yet, create an empty buffer
            const initialHeader = {
                documentCount: 0,
                totalBytes: 0,
                createdAt: new Date().toISOString(),
                lastUpdated: new Date().toISOString()
            };
            const initialIndex = {};
            
            const headerBuffer = Buffer.from(JSON.stringify(initialHeader));
            const indexBuffer = Buffer.from(JSON.stringify(initialIndex));
            
            const fileHeader = {
                headerLength: headerBuffer.length,
                indexLength: indexBuffer.length
            };
            
            const fileHeaderBuffer = Buffer.from(JSON.stringify(fileHeader));
            const fixedHeaderBuffer = Buffer.alloc(256);
            fileHeaderBuffer.copy(fixedHeaderBuffer);
            
            fileBuffer = Buffer.concat([
                fixedHeaderBuffer,
                headerBuffer,
                indexBuffer
            ]);
            
            await fs.writeFile(filePath, fileBuffer);
        }

        const fileHeaderSize = 256;
        
        const fileHeaderStr = fileBuffer.subarray(0, fileHeaderSize).toString().trim();
        const fileHeader = JSON.parse(fileHeaderStr.replace(/\0+$/, '')); // Remove null bytes at the end
        
        // Read the header section
        const headerStartPos = fileHeaderSize;
        const headerEndPos = headerStartPos + fileHeader.headerLength;
        const headerBuffer = fileBuffer.subarray(headerStartPos, headerEndPos);
        const header = JSON.parse(headerBuffer.toString());
        
        // Read the index section
        const indexStartPos = headerEndPos;
        const indexEndPos = indexStartPos + fileHeader.indexLength;
        const indexBuffer = fileBuffer.subarray(indexStartPos, indexEndPos);
        const index = JSON.parse(indexBuffer.toString());
        
        // Read the data section
        const dataStartPos = indexEndPos;
        const dataBuffer = fileBuffer.subarray(dataStartPos);
        
        // Prepare the document
        const documentJson = JSON.stringify(document);
        const documentBuffer = Buffer.from(documentJson);
        
        // Calculate where to place the document in the data section
        const documentOffset = dataBuffer.length;
        const documentLength = documentBuffer.length;

        index[document._id] = {
            offset: documentOffset,
            length: documentLength
        };

        header.documentCount += 1;
        header.totalBytes = (header.totalBytes || 0) + documentLength;
        header.lastUpdated = new Date().toISOString();

        const newHeaderBuffer = Buffer.from(JSON.stringify(header));
        const newIndexBuffer = Buffer.from(JSON.stringify(index));
        
        // Create a new file header with updated lengths
        const newFileHeader = {
            headerLength: newHeaderBuffer.length,
            indexLength: newIndexBuffer.length
        };

        const newFileHeaderJson = JSON.stringify(newFileHeader);
        const newFileHeaderBuffer = Buffer.alloc(256);
        Buffer.from(newFileHeaderJson).copy(newFileHeaderBuffer);
        
        // Combine all parts: file header + header + index + existing data + new document
        const newBuffer = Buffer.concat([
            newFileHeaderBuffer,
            newHeaderBuffer,
            newIndexBuffer,
            dataBuffer,
            documentBuffer
        ]);
        
        // Write the updated file
        await fs.writeFile(filePath, newBuffer);
        
        // Return information about where the document was stored
        return {
            offset: documentOffset,
            length: documentLength
        };
    }

    static async getCollectionIndex(databaseName, collectionName) {
        const collectionPath = path.join(StorageEngine.dataDir, databaseName, collectionName);
        const indexFilePath = path.join(collectionPath, 'collection_index.json');
        
        try {
            const indexRaw = await fs.readFile(indexFilePath, 'utf8');
            return JSON.parse(indexRaw);
        } catch (err) {
            throw new Error(`Failed to read collection index: ${err.message}`);
        }
    }

    static async get(databaseName, collectionName, documentId) {
        const collectionPath = path.join(StorageEngine.dataDir, databaseName, collectionName);
        const indexFilePath = path.join(collectionPath, 'collection_index.json');
        
        try {
            // Read the collection index
            const indexRaw = await fs.readFile(indexFilePath, 'utf8');
            const index = JSON.parse(indexRaw);
            
            // Check if document exists
            if (!index.documentMap[documentId]) {
                throw new Error(`Document '${documentId}' not found`);
            }
            
            // Get the chunk file that contains the document
            const chunkName = index.documentMap[documentId].chunk;
            const chunkPath = path.join(collectionPath, chunkName);
            
            // Read the document from the chunk
            return await StorageEngine.getDocumentFromChunk(chunkPath, documentId);
        } catch (err) {
            throw new Error(`Failed to retrieve document: ${err.message}`);
        }
    }

    static async getDocumentFromChunk(filePath, documentId) {
        try {
            // Read the file
            const fileBuffer = await fs.readFile(filePath);
            
            // Parse the file header
            const fileHeaderSize = 256;
            const fileHeaderJson = fileBuffer.subarray(0, fileHeaderSize).toString().trim();
            const fileHeader = JSON.parse(fileHeaderJson.replace(/\0+$/, '')); // Remove null bytes
            
            // Get the header section position
            const headerStartPos = fileHeaderSize;
            const headerEndPos = headerStartPos + fileHeader.headerLength;
            
            // Get the index section position
            const indexStartPos = headerEndPos;
            const indexEndPos = indexStartPos + fileHeader.indexLength;
            
            // Extract and parse the index section
            const indexBuffer = fileBuffer.subarray(indexStartPos, indexEndPos);
            const index = JSON.parse(indexBuffer.toString());
            
            // Check if the document exists in this chunk
            if (!index[documentId]) {
                return null;
            }
            
            // Get document position from the index
            const { offset, length } = index[documentId];
            
            // Calculate the absolute position in the file
            const dataStartPos = indexEndPos;
            const documentStartPos = dataStartPos + offset;
            const documentEndPos = documentStartPos + length;
            
            // Extract the document data
            const documentBuffer = fileBuffer.subarray(documentStartPos, documentEndPos);
            
            // Parse and return the document
            return JSON.parse(documentBuffer.toString());
        } catch (err) {
            console.error(`Error reading document from chunk: ${err.message}`);
            throw err;
        }
    }

    static generateId() {
        const timestamp = Math.floor(Date.now() / 1000).toString(16).padStart(8, '0');
        const machineId = randomBytes(3).toString('hex');
        const processId = Math.floor(Math.random() * 65535).toString(16).padStart(4, '0');
        const counter = Math.floor(Math.random() * 16777215).toString(16).padStart(6, '0');

        return timestamp + machineId + processId + counter;
    }
}

async function runStorageTest() {
    // Create the database and collection if they don't exist
    await StorageEngine.start();
    await StorageEngine.create('foo', 'bar');

    console.log('Starting test: Inserting 20 documents...');

    // Insert 20 documents and measure time
    const startTime = Date.now();
    const docIds = [];

    for (let i = 0; i < 2000; i++) {
        const doc = {
            value: i,
            timestamp: new Date().toISOString()
        };

        try {
            const result = await StorageEngine.create('foo', 'bar', doc);
            docIds.push(result.documentId);
            console.log(`Inserted document ${i}: ${result.documentId}`);
        } catch (err) {
            console.error(`Error inserting document ${i}:`, err);
        }
    }

    const endTime = Date.now();
    console.log(`Test completed in ${endTime - startTime}ms`);

    // Verify the documents were stored correctly
    console.log('\nVerifying document storage...');

    // Check the collection index
    const index = await StorageEngine.getCollectionIndex('foo', 'bar');
    console.log(`Total documents in index: ${index.metadata.documentCount}`);
    console.log(`Total chunks: ${index.chunks.length}`);
    console.log('Chunk details:');
    index.chunks.forEach(chunk => {
        console.log(`  ${chunk.name}: ${chunk.documentCount} documents, ${Math.round(chunk.totalBytes/1024)} KB`);
    });

    // Try to retrieve a few documents
    console.log('\nRetrieving sample documents:');

    // Get a few document IDs that we saved during insertion
    const sampleDocIds = [docIds[0], docIds[5], docIds[10], docIds[19]];
    
    for (const docId of sampleDocIds) {
        try {
            const doc = await StorageEngine.get('foo', 'bar', docId);
            console.log(`Retrieved ${docId}: ${JSON.stringify(doc)}`);
        } catch (err) {
            console.error(`Error retrieving ${docId}:`, err);
        }
    }
}

// Run the test
runStorageTest()
    .then(() => console.log('Test completed successfully!'))
    .catch(err => console.error('Test failed:', err));