// scripts/run-integration-tests.js
import { spawn } from 'child_process';
import { setTimeout } from 'timers/promises';
import path from 'path';
import { fileURLToPath } from 'url';

// Get current script directory
const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);
const rootDir = path.join(__dirname, '..');

// Start required services for testing
async function startDaemon() {
  console.log('🚀 Starting daemon for integration tests...');
  
  return new Promise((resolve, reject) => {
    const daemon = spawn('node', ['./packages/daemon/src/daemon.js'], {
      cwd: rootDir,
      stdio: ['ignore', 'pipe', 'pipe']
    });
    
    // Wait for daemon to be ready
    daemon.stdout.on('data', (data) => {
      const output = data.toString();
      console.log(`[daemon] ${output.trim()}`);
      
      // Replace this with your actual daemon ready message
      if (output.includes('Daemon started') || output.includes('listening')) {
        console.log('✅ Daemon is ready');
        resolve(daemon);
      }
    });
    
    daemon.stderr.on('data', (data) => {
      console.error(`[daemon error] ${data.toString().trim()}`);
    });
    
    daemon.on('error', (err) => {
      console.error('Failed to start daemon:', err);
      reject(err);
    });
    
    // Set a timeout in case the ready message is missed
    setTimeout(10000).then(() => {
      console.log('✅ Assuming daemon is ready (timeout)');
      resolve(daemon);
    });
  });
}

// Run the CLI tests
async function runCliTests() {
  console.log('🧪 Running CLI integration tests...');
  
  return new Promise((resolve, reject) => {
    const testProc = spawn('node', ['./packageCli/tests/cli_tests.js'], {
      cwd: path.join(rootDir, 'packages/Cli'),
      stdio: 'inherit'
    });
    
    testProc.on('close', (code) => {
      if (code === 0) {
        console.log('✅ CLI tests completed successfully');
        resolve();
      } else {
        console.error(`❌ CLI tests failed with exit code ${code}`);
        reject(new Error(`CLI tests exited with code ${code}`));
      }
    });
    
    testProc.on('error', (err) => {
      console.error('Failed to run CLI tests:', err);
      reject(err);
    });
  });
}

// Run daemon integration tests
async function runDaemonTests() {
  console.log('🧪 Running daemon integration tests...');
  
  return new Promise((resolve, reject) => {
    const testProc = spawn('node', ['./daemon/Tests/commandIntergrationTest.js'], {
      cwd: path.join(rootDir, 'packages/daemon'),
      stdio: 'inherit'
    });
    
    testProc.on('close', (code) => {
      if (code === 0) {
        console.log('✅ Daemon integration tests completed successfully');
        resolve();
      } else {
        console.error(`❌ Daemon integration tests failed with exit code ${code}`);
        reject(new Error(`Daemon tests exited with code ${code}`));
      }
    });
    
    testProc.on('error', (err) => {
      console.error('Failed to run daemon tests:', err);
      reject(err);
    });
  });
}

// Main test runner
async function runAllIntegrationTests() {
  let daemon = null;
  
  try {
    // Start the daemon
    daemon = await startDaemon();
    
    // Allow daemon to fully initialize
    await setTimeout(2000);
    
    // Run CLI tests
    await runCliTests(daemon);
    
    // Run daemon integration tests
    await runDaemonTests(daemon);
    
    console.log('🎉 All integration tests completed successfully');
    process.exit(0);
  } catch (err) {
    console.error('❌ Integration tests failed:', err);
    process.exit(1);
  } finally {
    // Clean up - kill the daemon
    if (daemon) {
      console.log('🧹 Cleaning up - stopping daemon...');
      daemon.kill();
    }
  }
}

// Run all tests
runAllIntegrationTests();