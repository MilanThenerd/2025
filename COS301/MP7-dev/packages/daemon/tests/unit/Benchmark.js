/**
 * Benchmarks read performance between the original file-per-document storage
 * engine (V1) and the new chunked storage engine (V2)
 */
import StorageEngineV1 from './StorageEngineOld.js';
import StorageEngine from '../../src/StorageEngine.js';


async function benchmarkStorageEngines() {
    // Test configuration
    const DATABASE_NAME = 'benchmark_db';
    const COLLECTION_NAME = 'test_collection';
    const DOCUMENT_COUNT = 1000;
    const SAMPLE_SIZE = 100; // Number of random reads to perform
    const ITERATIONS = 5; // Number of times to run each test for averaging

    console.log('=== STORAGE ENGINE BENCHMARK ===');
    console.log(`Creating test data: ${DOCUMENT_COUNT} documents`);
    
    // Class references to both storage engines
    const V1Engine = StorageEngineV1; // Your original storage engine
    const V2Engine = StorageEngine;    // Your new chunked storage engine
    
    // Initialize both engines
    await V1Engine.start();
    await V2Engine.start();
    
    // Clean up any existing test data
    try {
        await V1Engine.delete(DATABASE_NAME);
    } catch (err) {
        // Ignore - database might not exist
    }
    
    try {
        await V2Engine.delete(DATABASE_NAME);
    } catch (err) {
        // Ignore - database might not exist
    }
    
    // Create test collections
    await V1Engine.create(DATABASE_NAME, COLLECTION_NAME);
    await V2Engine.create(DATABASE_NAME, COLLECTION_NAME);
    
    // Generate and insert test documents
    console.log('Inserting test documents...');
    const documentIds = [];
    
    for (let i = 0; i < DOCUMENT_COUNT; i++) {
        const testDoc = {
            title: `Test Document ${i}`,
            content: `This is test document ${i} with some content to make it more realistic. ${generateRandomText(100)}`,
            tags: ['test', 'benchmark', `tag${i % 10}`],
            metadata: {
                created: new Date().toISOString(),
                priority: Math.floor(Math.random() * 5),
                views: Math.floor(Math.random() * 1000),
                nested: {
                    field1: `value ${i}`,
                    field2: Math.random() * 100
                }
            }
        };
        
        // Insert into V1 engine
        const v1Doc = { ...testDoc };
        v1Doc._id = V1Engine.generateId();
        await V1Engine.create(DATABASE_NAME, COLLECTION_NAME, v1Doc);
        
        // Insert into V2 engine
        const v2Doc = { ...testDoc };
        const result = await V2Engine.create(DATABASE_NAME, COLLECTION_NAME, v2Doc);
        
        // Store the ID for later retrieval
        documentIds.push({
            v1Id: v1Doc._id, 
            v2Id: result.documentId
        });
    }
    
    console.log(`${DOCUMENT_COUNT} documents inserted into both storage engines`);
    console.log('\nBenchmarking read operations:');
    
    // --- TEST 1: Single document read by ID ---
    const singleReadResults = {
        v1: [],
        v2: []
    };
    
    console.log('\n1. Single document read performance:');
    for (let iteration = 0; iteration < ITERATIONS; iteration++) {
        // Select a random document
        const randomIndex = Math.floor(Math.random() * documentIds.length);
        const { v1Id, v2Id } = documentIds[randomIndex];
        
        // Benchmark V1 read
        const v1Start = performance.now();
        await V1Engine.read(DATABASE_NAME, COLLECTION_NAME, v1Id);
        const v1End = performance.now();
        singleReadResults.v1.push(v1End - v1Start);
        
        // Benchmark V2 read
        const v2Start = performance.now();
        await V2Engine.get(DATABASE_NAME, COLLECTION_NAME, v2Id);
        const v2End = performance.now();
        singleReadResults.v2.push(v2End - v2Start);
    }
    
    // Calculate average times
    const v1SingleAvg = singleReadResults.v1.reduce((a, b) => a + b, 0) / ITERATIONS;
    const v2SingleAvg = singleReadResults.v2.reduce((a, b) => a + b, 0) / ITERATIONS;
    
    console.log(`V1 Engine: ${v1SingleAvg.toFixed(2)}ms average per document`);
    console.log(`V2 Engine: ${v2SingleAvg.toFixed(2)}ms average per document`);
    console.log(`Difference: ${(v1SingleAvg / v2SingleAvg).toFixed(2)}x ${v1SingleAvg > v2SingleAvg ? 'faster with V2' : 'faster with V1'}`);
    
    // --- TEST 2: Multiple random document reads ---
    const randomReadResults = {
        v1: [],
        v2: []
    };
    
    console.log('\n2. Random document access performance:');
    for (let iteration = 0; iteration < ITERATIONS; iteration++) {
        // Select random sample of documents
        const randomIndices = [];
        while (randomIndices.length < SAMPLE_SIZE) {
            const idx = Math.floor(Math.random() * documentIds.length);
            if (!randomIndices.includes(idx)) {
                randomIndices.push(idx);
            }
        }
        
        // Benchmark V1 reads
        const v1Start = performance.now();
        for (const idx of randomIndices) {
            await V1Engine.read(DATABASE_NAME, COLLECTION_NAME, documentIds[idx].v1Id);
        }
        const v1End = performance.now();
        randomReadResults.v1.push(v1End - v1Start);
        
        // Benchmark V2 reads
        const v2Start = performance.now();
        for (const idx of randomIndices) {
            await V2Engine.get(DATABASE_NAME, COLLECTION_NAME, documentIds[idx].v2Id);
        }
        const v2End = performance.now();
        randomReadResults.v2.push(v2End - v2Start);
    }
    
    // Calculate average times
    const v1RandomAvg = randomReadResults.v1.reduce((a, b) => a + b, 0) / ITERATIONS;
    const v2RandomAvg = randomReadResults.v2.reduce((a, b) => a + b, 0) / ITERATIONS;
    
    console.log(`V1 Engine: ${v1RandomAvg.toFixed(2)}ms for ${SAMPLE_SIZE} random reads (${(v1RandomAvg/SAMPLE_SIZE).toFixed(2)}ms per read)`);
    console.log(`V2 Engine: ${v2RandomAvg.toFixed(2)}ms for ${SAMPLE_SIZE} random reads (${(v2RandomAvg/SAMPLE_SIZE).toFixed(2)}ms per read)`);
    console.log(`Difference: ${(v1RandomAvg / v2RandomAvg).toFixed(2)}x ${v1RandomAvg > v2RandomAvg ? 'faster with V2' : 'faster with V1'}`);
    
    // --- TEST 3: Sequential document reads ---
    const sequentialReadResults = {
        v1: [],
        v2: []
    };
    
    console.log('\n3. Sequential document access performance:');
    for (let iteration = 0; iteration < ITERATIONS; iteration++) {
        // Take a sequential sample from the middle
        const startIdx = Math.floor(documentIds.length / 2) - Math.floor(SAMPLE_SIZE / 2);
        const sequentialSample = documentIds.slice(startIdx, startIdx + SAMPLE_SIZE);
        
        // Benchmark V1 reads
        const v1Start = performance.now();
        for (const doc of sequentialSample) {
            await V1Engine.read(DATABASE_NAME, COLLECTION_NAME, doc.v1Id);
        }
        const v1End = performance.now();
        sequentialReadResults.v1.push(v1End - v1Start);
        
        // Benchmark V2 reads
        const v2Start = performance.now();
        for (const doc of sequentialSample) {
            await V2Engine.get(DATABASE_NAME, COLLECTION_NAME, doc.v2Id);
        }
        const v2End = performance.now();
        sequentialReadResults.v2.push(v2End - v2Start);
    }
    
    // Calculate average times
    const v1SeqAvg = sequentialReadResults.v1.reduce((a, b) => a + b, 0) / ITERATIONS;
    const v2SeqAvg = sequentialReadResults.v2.reduce((a, b) => a + b, 0) / ITERATIONS;
    
    console.log(`V1 Engine: ${v1SeqAvg.toFixed(2)}ms for ${SAMPLE_SIZE} sequential reads (${(v1SeqAvg/SAMPLE_SIZE).toFixed(2)}ms per read)`);
    console.log(`V2 Engine: ${v2SeqAvg.toFixed(2)}ms for ${SAMPLE_SIZE} sequential reads (${(v2SeqAvg/SAMPLE_SIZE).toFixed(2)}ms per read)`);
    console.log(`Difference: ${(v1SeqAvg / v2SeqAvg).toFixed(2)}x ${v1SeqAvg > v2SeqAvg ? 'faster with V2' : 'faster with V1'}`);
    
    // --- TEST 4: List all documents ---
    const listAllResults = {
        v1: [],
        v2: []
    };
    
    console.log('\n4. Listing all documents performance:');
    for (let iteration = 0; iteration < ITERATIONS; iteration++) {
        // Benchmark V1 list
        const v1Start = performance.now();
        await V1Engine.list(DATABASE_NAME, COLLECTION_NAME);
        const v1End = performance.now();
        listAllResults.v1.push(v1End - v1Start);
        
        // Benchmark V2 list (assuming we've implemented the list method)
        const v2Start = performance.now();
        await V2Engine.list(DATABASE_NAME, COLLECTION_NAME);
        const v2End = performance.now();
        listAllResults.v2.push(v2End - v2Start);
    }
    
    // Calculate average times
    const v1ListAvg = listAllResults.v1.reduce((a, b) => a + b, 0) / ITERATIONS;
    const v2ListAvg = listAllResults.v2.reduce((a, b) => a + b, 0) / ITERATIONS;
    
    console.log(`V1 Engine: ${v1ListAvg.toFixed(2)}ms to list all documents`);
    console.log(`V2 Engine: ${v2ListAvg.toFixed(2)}ms to list all documents`);
    console.log(`Difference: ${(v1ListAvg / v2ListAvg).toFixed(2)}x ${v1ListAvg > v2ListAvg ? 'faster with V2' : 'faster with V1'}`);
    
    // --- TEST 5: Filter/search documents ---
    const searchResults = {
        v1: [],
        v2: []
    };
    
    console.log('\n5. Search/filter documents performance:');
    for (let iteration = 0; iteration < ITERATIONS; iteration++) {
        // Create a simple search query
        const searchQuery = { 
            "metadata.priority": 3 
        };
        
        // Benchmark V1 search
        const v1Start = performance.now();
        await V1Engine.search(DATABASE_NAME, COLLECTION_NAME, searchQuery);
        const v1End = performance.now();
        searchResults.v1.push(v1End - v1Start);
        
        // Benchmark V2 search
        const v2Start = performance.now();
        await V2Engine.search(DATABASE_NAME, COLLECTION_NAME, searchQuery);
        const v2End = performance.now();
        searchResults.v2.push(v2End - v2Start);
    }
    
    // Calculate average times
    const v1SearchAvg = searchResults.v1.reduce((a, b) => a + b, 0) / ITERATIONS;
    const v2SearchAvg = searchResults.v2.reduce((a, b) => a + b, 0) / ITERATIONS;
    
    console.log(`V1 Engine: ${v1SearchAvg.toFixed(2)}ms to search/filter documents`);
    console.log(`V2 Engine: ${v2SearchAvg.toFixed(2)}ms to search/filter documents`);
    console.log(`Difference: ${(v1SearchAvg / v2SearchAvg).toFixed(2)}x ${v1SearchAvg > v2SearchAvg ? 'faster with V2' : 'faster with V1'}`);
    
    // Summary
    console.log('\n=== BENCHMARK SUMMARY ===');
    console.log(`Single reads: ${(v1SingleAvg / v2SingleAvg).toFixed(2)}x ${v1SingleAvg > v2SingleAvg ? 'faster with V2' : 'faster with V1'}`);
    console.log(`Random reads: ${(v1RandomAvg / v2RandomAvg).toFixed(2)}x ${v1RandomAvg > v2RandomAvg ? 'faster with V2' : 'faster with V1'}`);
    console.log(`Sequential reads: ${(v1SeqAvg / v2SeqAvg).toFixed(2)}x ${v1SeqAvg > v2SeqAvg ? 'faster with V2' : 'faster with V1'}`);
    console.log(`Listing all: ${(v1ListAvg / v2ListAvg).toFixed(2)}x ${v1ListAvg > v2ListAvg ? 'faster with V2' : 'faster with V1'}`);
    console.log(`Search/filter: ${(v1SearchAvg / v2SearchAvg).toFixed(2)}x ${v1SearchAvg > v2SearchAvg ? 'faster with V2' : 'faster with V1'}`);
    
    // Cleanup
    try {
        await V1Engine.delete(DATABASE_NAME);
        await V2Engine.delete(DATABASE_NAME);
        console.log('\nTest data cleaned up.');
    } catch (err) {
        console.error('Error cleaning up test data:', err);
    }
}

/**
 * Helper function to generate random text for documents
 */
function generateRandomText(length) {
    const words = [
        'lorem', 'ipsum', 'dolor', 'sit', 'amet', 'consectetur', 
        'adipiscing', 'elit', 'morbi', 'non', 'consequat', 'massa', 
        'duis', 'aute', 'irure', 'reprehenderit', 'voluptate', 'velit', 
        'esse', 'cillum', 'dolore', 'fugiat', 'nulla', 'pariatur', 
        'excepteur', 'sint', 'occaecat', 'cupidatat', 'proident', 'sunt'
    ];
    
    let result = '';
    
    for (let i = 0; i < length; i++) {
        const randomWord = words[Math.floor(Math.random() * words.length)];
        result += randomWord + ' ';
        
        // Add some punctuation occasionally
        if (i % 10 === 9) {
            result += '. ';
        } else if (i % 5 === 4) {
            result += ', ';
        }
    }
    
    return result.trim();
}

benchmarkStorageEngines();