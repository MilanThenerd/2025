// scripts/run-manual-tests.js
import { spawn } from 'child_process';
import path from 'path';
import { fileURLToPath } from 'url';

// Get current script directory
const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);
const rootDir = path.join(__dirname, '..');

const manualTests = [
  {
    name: 'CLI Tests',
    package: 'Cli',
    script: './tests/cli_tests.js',
    requiresDaemon: true
  },
  {
    name: 'Daemon Command Tests',
    package: 'daemon',
    script: './tests/unit/commandTest.js',
    requiresDaemon: false
  },
  {
    name: 'Storage Engine Tests',
    package: 'daemon',
    script: './tests/unit/storageEngineTest.js',
    requiresDaemon: false
  },
//   {
//     name: 'Concurrency Tests',
//     package: 'daemon',
//     script: './tests/unit/testConcurrency.js',
//     requiresDaemon: false
//   },
  {
    name: 'Command Integration Tests',
    package: 'daemon',
    script: './tests/integration/commandIntergrationTest.js',
    requiresDaemon: true
  }
];

async function runTest(test) {
  return new Promise((resolve, ) => {
    console.log(`\n🧪 Running ${test.name}...`);
    
    const testProc = spawn('node', [test.script], {
      cwd: path.join(rootDir, 'packages', test.package),
      stdio: 'inherit'
    });
    
    testProc.on('close', (code) => {
      if (code === 0) {
        console.log(`✅ ${test.name} completed successfully`);
        resolve(true);
      } else {
        console.error(`❌ ${test.name} failed with exit code ${code}`);
        resolve(false); // Resolve with false to allow other tests to run
      }
    });
    
    testProc.on('error', (err) => {
      console.error(`Failed to run ${test.name}:`, err);
      resolve(false);
    });
  });
}

// Function to run a daemon instance
async function startDaemon() {
  return new Promise((resolve, reject) => {
    console.log('🚀 Starting test daemon...');
    
    const daemon = spawn('node', ['./src/daemon.js'], {
      cwd: path.join(rootDir, 'packages/daemon'),
      stdio: ['ignore', 'pipe', 'pipe']
    });
    
    let isReady = false;
    
    daemon.stdout.on('data', (data) => {
      const output = data.toString();
      console.log(`[daemon] ${output.trim()}`);
      
      if (!isReady && (output.includes('Daemon started') || output.includes('listening'))) {
        console.log('✅ Daemon is ready');
        isReady = true;
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
    
    setTimeout(() => {
      if (!isReady) {
        console.log('✅ Assuming daemon is ready (timeout)');
        resolve(daemon);
      }
    }, 5000);
  });
}


async function runAllManualTests() {
  const standaloneTasks = manualTests.filter(test => !test.requiresDaemon);
  const daemonDependentTasks = manualTests.filter(test => test.requiresDaemon);
  
  let daemon = null;
  let hasErrors = false;
  
  try {
    console.log('📋 Running standalone tests first...');
    
    for (const test of standaloneTasks) {
      const success = await runTest(test);
      if (!success) hasErrors = true;
    }
    

    if (daemonDependentTasks.length > 0) {
      console.log('\n📋 Running tests that require the daemon...');
      daemon = await startDaemon();
      
      for (const test of daemonDependentTasks) {
        const success = await runTest(test);
        if (!success) hasErrors = true;
      }
    }
    
    if (hasErrors) {
      console.log('\n⚠️ Some tests failed. Check the output above for details.');
      process.exit(1);
    } else {
      console.log('\n🎉 All manual tests passed!');
      process.exit(0);
    }
  } catch (err) {
    console.error('\n❌ Error running tests:', err);
    process.exit(1);
  } finally {
    if (daemon) {
      console.log('\n🧹 Stopping daemon...');
      daemon.kill();
    }
  }
}

const args = process.argv.slice(2);
if (args.length > 0) {
  const testName = args[0];
  const test = manualTests.find(t => t.name.toLowerCase().includes(testName.toLowerCase()));
  
  if (test) {
    console.log(`Running specific test: ${test.name}`);
    
    (async () => {
      let daemon = null;
      try {
        if (test.requiresDaemon) {
          daemon = await startDaemon();
        }
        
        const success = await runTest(test);
        process.exit(success ? 0 : 1);
      } catch (err) {
        console.error('Error:', err);
        process.exit(1);
      } finally {
        if (daemon) daemon.kill();
      }
    })();
  } else {
    console.error(`Test not found: ${testName}`);
    console.log('Available tests:');
    manualTests.forEach(t => console.log(`- ${t.name}`));
    process.exit(1);
  }
} else {
  // Run all tests
  runAllManualTests();
}