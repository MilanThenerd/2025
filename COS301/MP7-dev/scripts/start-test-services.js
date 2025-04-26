/**
 * Start Test Services Script
 * Starts all required services for integration testing
 */
import { spawn } from 'child_process';
import fs from 'fs';
import path from 'path';

console.log('🚀 Starting services for integration tests...');

// Track the spawned processes
const processes = [];
const processLogDir = path.join(process.cwd(), 'coverage', 'test-logs');

// Create logs directory if it doesn't exist
if (!fs.existsSync(processLogDir)) {
  fs.mkdirSync(processLogDir, { recursive: true });
}

// Start the daemon service
function startDaemon() {
  console.log('Starting Daemon service...');
  const logFile = fs.createWriteStream(path.join(processLogDir, 'daemon.log'));
  
  const daemon = spawn('npm', ['run', 'start:daemon'], {
    stdio: ['ignore', 'pipe', 'pipe'],
    detached: true
  });
  
  daemon.stdout.pipe(logFile);
  daemon.stderr.pipe(logFile);
  
  processes.push({
    name: 'daemon',
    process: daemon
  });
  
  // Write PID file for cleanup
  fs.writeFileSync(path.join(processLogDir, 'daemon.pid'), daemon.pid.toString());
  
  console.log(`Daemon service started with PID: ${daemon.pid}`);
  
  // Give the daemon time to start
  return new Promise(resolve => setTimeout(resolve, 2000));
}

// Start the API service
function startAPI() {
  console.log('Starting API service...');
  const logFile = fs.createWriteStream(path.join(processLogDir, 'api.log'));
  
  const api = spawn('npm', ['run', 'start:api'], {
    stdio: ['ignore', 'pipe', 'pipe'],
    detached: true
  });
  
  api.stdout.pipe(logFile);
  api.stderr.pipe(logFile);
  
  processes.push({
    name: 'api',
    process: api
  });
  
  // Write PID file for cleanup
  fs.writeFileSync(path.join(processLogDir, 'api.pid'), api.pid.toString());
  
  console.log(`API service started with PID: ${api.pid}`);
  
  // Give the API time to start
  return new Promise(resolve => setTimeout(resolve, 2000));
}

// Start services in sequence
async function startServices() {
  try {
    await startDaemon();
    await startAPI();
    
    console.log('✅ All services started successfully');
    
    // Don't exit this process yet - we want to keep these services running
    // for the duration of the tests
  } catch (error) {
    console.error('❌ Failed to start services:', error);
    process.exit(1);
  }
}

startServices();

// Register cleanup on exit
process.on('exit', () => {
  console.log('Exiting start-test-services script');
});