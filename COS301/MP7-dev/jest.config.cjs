// jest.config.cjs
const fs = require('fs');
const path = require('path');

// Helper to read .jest-ignore file if it exists
function getIgnorePatterns() {
    try {
        return fs
            .readFileSync('.jest-ignore', 'utf8')
            .split('\n')
            .filter(line => line && !line.startsWith('#'))
            .map(line => line.trim());
    } catch (e) {
        // If file doesn't exist, return empty array
        return [];
    }
}

const ignorePatterns = getIgnorePatterns();

// Get all package directories for package-specific projects
function getPackageDirs() {
    const packagesDir = path.join(__dirname, 'packages');
    return fs.readdirSync(packagesDir)
        .filter(dir => fs.statSync(path.join(packagesDir, dir)).isDirectory())
        .map(dir => ({
            name: dir.toLowerCase().replace(/_/g, '-'), // Normalize names
            path: dir
        }));
}

const packages = getPackageDirs();

module.exports = {
    rootDir: '.',
    // Common settings for all tests
    testEnvironment: 'node',
    verbose: true,
    clearMocks: true,
    
    // Use projects to separate different types of tests
    projects: [
        // Unit tests configuration
        {
            displayName: 'unit',
            testMatch: [
                '**/__tests__/**/*.js',
                '**/*.test.js',
                '**/*.tests.js'
            ],
            testPathIgnorePatterns: [
                '/node_modules/',
                '.*\\.integration\\.test\\.js$',
                '.*\\.integration\\.tests\\.js$',
                '/tests/integration/'
            ],
            coverage: true,
            coverageDirectory: './coverage/unit'
        },
        
        // Integration tests configuration
        {
            displayName: 'integration',
            testMatch: [
                '**/tests/integration/**/*.js',
                '**/*.integration.test.js',
                '**/*.integration.tests.js'
            ],
            testPathIgnorePatterns: [
                '/node_modules/',
                '/.test-pids/'
            ],
            // Longer timeout for integration tests
            testTimeout: 30000,
            coverage: true,
            coverageDirectory: './coverage/integration'
        },
        
        // Add projects for each package (helps with running tests for specific packages)
        ...packages.map(pkg => ({
            displayName: pkg.name,
            rootDir: '.',
            testMatch: [
                `<rootDir>/packages/${pkg.path}/**/*.test.js`,
                `<rootDir>/packages/${pkg.path}/**/*.tests.js`
            ],
            testPathIgnorePatterns: [
                '/node_modules/'
            ]
        }))
    ],
    
    // Coverage configuration for when running all tests
    collectCoverageFrom: [
        'packages/*/src/**/*.js',
        '!**/node_modules/**',
    ],
    coverageDirectory: './coverage',
    coveragePathIgnorePatterns: [
        '/node_modules/'
    ],
    
    // For ES modules support when needed
    transform: {},
    
    // For monorepo package resolution
    moduleNameMapper: {
        '^@mp7/(.*)$': '<rootDir>/packages/$1/src'
    }
};