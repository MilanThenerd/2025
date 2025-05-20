# FTP File Monitoring and Auto-Upload System

This program automatically monitors HTML files for changes and uploads them to a remote server via FTP whenever they're modified.

## Features

- Monitors individual files or entire directories for changes using file system polling
- Implements FTP protocol from scratch without using external libraries
- Supports both passive mode (default) and active mode FTP transfers
- Provides automatic retry mechanism for failed uploads
- Basic support for TLS/SSL (FTPS) control connection
- Detailed error handling and logging
- Advanced FTP operations:
  - Directory creation and manipulation
  - Preserving directory structures when uploading
  - Batch file uploads in a single FTP session
  - Directory listing capabilities
  - File filtering by extension

## Prerequisites

To run this program, you'll need:

1. Java Development Kit (JDK) 8 or later
2. Apache HTTP Server installed on your server
3. FTP server (like vsftpd) installed on your server
4. Proper permissions for the FTP user to write to the Apache web directory

## Server Setup

### 1. Install Apache and FTP Server

On your Ubuntu/Debian server:

```bash
# Install Apache
sudo apt-get update
sudo apt-get install apache2

# Install vsftpd (FTP server)
sudo apt-get install vsftpd
```

### 2. Configure vsftpd

Edit the vsftpd configuration file:

```bash
sudo nano /etc/vsftpd.conf
```

Ensure the following settings are enabled:

```
anonymous_enable=NO
local_enable=YES
write_enable=YES
local_umask=022
chroot_local_user=YES
```

### 3. Create FTP User with Access to Web Directory

```bash
# Create a user
sudo adduser ftpuser

# Add the user to the www-data group (Apache group)
sudo usermod -a -G www-data ftpuser

# Set proper permissions on the web directory
sudo chmod 775 /var/www/html
sudo chown -R www-data:www-data /var/www/html
```

### 4. Restart Services

```bash
sudo systemctl restart vsftpd
sudo systemctl restart apache2
```

## Client Program Configuration

Before running the program, modify the following settings in `main.java`:

### Basic Configuration

```java
// FTP server configuration
private static final String FTP_HOST = "your.server.ip.address"; // Set to your server IP/hostname
private static final String FTP_USERNAME = "ftpuser"; // Set to your FTP username
private static final String FTP_PASSWORD = "yourpassword"; // Set to your FTP password
private static final String REMOTE_DIRECTORY = "/var/www/html/"; // Apache served directory
```

### Single File Mode

For monitoring a single file (default mode):

```java
private static final String MONITORED_FILE_PATH = "/path/to/your/index.html"; // Local HTML file to monitor
private static final boolean MONITOR_DIRECTORY = false; // Keep as false
```

### Directory Monitoring Mode

For monitoring an entire directory:

```java
private static final String MONITORED_DIRECTORY = "/path/to/your/webfiles"; // Directory containing files to monitor
private static final boolean MONITOR_DIRECTORY = true; // Set to true for directory monitoring
private static final String[] FILE_EXTENSIONS = {".html", ".css", ".js"}; // Extensions to monitor
```

### FTP Connection Settings

```java
// Change to false to use active mode instead of passive mode
private static final boolean USE_PASSIVE_MODE = true;

// Port to use for active mode data connection (if needed)
private static final int ACTIVE_MODE_PORT = 50000;
```

## Compile and Run

Compile the program:

```bash
javac main.java
```

Run the program:

```bash
java main
```

## Testing the Setup

### Testing Single File Mode

1. Make sure the program is running and monitoring your HTML file
2. Open a web browser and navigate to `http://your.server.ip.address/index.html`
3. Edit and save the HTML file on your local computer
4. The program should detect the change and upload the file automatically
5. Refresh your browser to see the updated content

### Testing Directory Mode

1. Enable directory mode in the configuration
2. Start the program, which will begin monitoring your specified directory
3. Create or modify files with the specified extensions (.html, .css, .js by default)
4. The program will detect changes and upload the modified files automatically
5. If you modify multiple files, they will be uploaded in a single FTP session

## Notes on FTP Protocol Implementation

This program demonstrates advanced FTP concepts:

- **Control Connection**: Used for sending commands and receiving responses
- **Data Connection**: Established in either passive or active mode for transferring file data
- **Passive Mode**: Client initiates both connections (control and data)
- **Active Mode**: Client initiates control connection, server initiates data connection
- **Directory Operations**: Creating and navigating remote directories (MKD, CWD)
- **Directory Listing**: Retrieving directory contents (LIST)
- **Binary Mode**: Transferring files without text transformations (TYPE I)
- **Batch Operations**: Optimizing by maintaining a single control connection for multiple transfers

## Troubleshooting

- Ensure your firewall allows FTP connections (ports 21, 20, and passive mode ports)
- Check server logs: `/var/log/vsftpd.log`
- Verify FTP user has write permissions to the web directory
- For active mode, ensure the client is accessible from the server (no NAT/firewall blocking)
- Check the console output for detailed error messages and FTP protocol communication 