/**
 * Stop Test Services Script
 * Stops all services that were started for integration testing
 */
import fs from 'fs';
import path from 'path';

console.log('🛑 Stopping services from integration tests...');

const processLogDir = path.join(process.cwd(), 'coverage', 'test-logs');

// Check if the log directory exists
if (!fs.existsSync(processLogDir)) {
  console.log('No services to stop - log directory not found');
  process.exit(0);
}

// Function to kill a process by PID
function killProcess(name) {
  const pidFile = path.join(processLogDir, `${name}.pid`);
  
  if (!fs.existsSync(pidFile)) {
    console.log(`No PID file found for ${name}`);
    return;
  }
  
  try {
    const pid = parseInt(fs.readFileSync(pidFile, 'utf8').trim());
    
    if (isNaN(pid)) {
      console.log(`Invalid PID for ${name}`);
      return;
    }
    
    console.log(`Stopping ${name} (PID: ${pid})...`);
    
    // Different process killing approach based on platform
    if (process.platform === 'win32') {
      try {
        process.kill(pid);
      } catch (e) {
        // On Windows, trying to kill may fail more often
        console.log(`Could not kill ${name} process directly, trying taskkill...`);
        require('child_process').execSync(`taskkill /PID ${pid} /F /T`);
      }
    } else {
      try {
        process.kill(pid, 'SIGTERM');
        // Give it a moment to clean up
        setTimeout(() => {
          try {
            // Check if it's still running
            process.kill(pid, 0);
            // If we get here, it's still running, force kill
            console.log(`${name} still running, force killing...`);
            process.kill(pid, 'SIGKILL');
          } catch (e) {
            // Process is already gone, which is good
          }
        }, 500);
      } catch (e) {
        console.log(`Could not kill ${name} process: ${e.message}`);
      }
    }
    
    // Remove the PID file
    fs.unlinkSync(pidFile);
    console.log(`✅ ${name} stopped`);
  } catch (error) {
    console.error(`❌ Error stopping ${name}:`, error.message);
  }
}

// Stop all known services
killProcess('daemon');
killProcess('api');

console.log('All services stopped');