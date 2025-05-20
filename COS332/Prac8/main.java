import java.io.*;
import java.net.*;
import java.nio.file.*;
import java.nio.file.attribute.FileTime;
import java.util.*;
import java.util.concurrent.TimeUnit;
import java.util.stream.Collectors;

public class main {
    private static final String FTP_HOST = "localhost";
    private static final int FTP_PORT = 21;
    private static final String FTP_USERNAME = "ftpuser";
    private static final String FTP_PASSWORD = "3918";
    private static final String REMOTE_DIRECTORY = "";
    
    private static final long POLLING_INTERVAL = 5000;
    private static final String MONITORED_DIRECTORY = ".";
    private static final boolean MONITOR_DIRECTORY = true;
    private static final String[] FILE_EXTENSIONS = {".html", ".css", ".js"};
    
    private static final boolean USE_PASSIVE_MODE = true;
    private static final int ACTIVE_MODE_PORT = 50000;
    
    private static final boolean USE_SECURE_CONTROL = false;
    private static final int MAX_RETRIES = 3;
    
    private static Map<Path, FileTime> fileModificationTimes = new HashMap<>();
    
    private static Socket controlSocket = null;
    private static BufferedReader controlReader = null;
    private static BufferedWriter controlWriter = null;
    
    private static boolean connectFTP() {
        try {
            if (controlSocket != null && controlSocket.isConnected()) {
                String response = sendCommand(controlWriter, controlReader, "NOOP");
                if (response.startsWith("200")) {
                    return true;
                }
            }
            
            if (controlSocket != null) {
                try {
                    controlSocket.close();
                } catch (IOException e) {
                }
            }
            
            System.out.println("Connecting to FTP server: " + FTP_HOST);
            controlSocket = new Socket(FTP_HOST, FTP_PORT);
            controlReader = new BufferedReader(new InputStreamReader(controlSocket.getInputStream()));
            controlWriter = new BufferedWriter(new OutputStreamWriter(controlSocket.getOutputStream()));
            
            String response = readResponse(controlReader);
            if (!response.startsWith("220")) {
                System.err.println("Unexpected server welcome: " + response);
                return false;
            }
            
            response = sendCommand(controlWriter, controlReader, "USER " + FTP_USERNAME);
            if (!response.startsWith("331") && !response.startsWith("230")) {
                System.err.println("Username not accepted: " + response);
                return false;
            }
            
            if (response.startsWith("331")) {
                response = sendCommand(controlWriter, controlReader, "PASS " + FTP_PASSWORD);
                if (!response.startsWith("230") && !response.startsWith("202")) {
                    System.err.println("Password not accepted: " + response);
                    return false;
                }
            }
            
            response = sendCommand(controlWriter, controlReader, "TYPE I");
            if (!response.startsWith("200")) {
                System.err.println("Failed to set binary mode: " + response);
                return false;
            }
            
            System.out.println("FTP connection established and ready");
            return true;
            
        } catch (IOException e) {
            System.err.println("FTP connection error: " + e.getMessage());
            return false;
        }
    }
    
    public static void main(String[] args) {
        System.out.println("FTP File Monitor starting...");
        System.out.println("Monitoring directory: " + MONITORED_DIRECTORY);
        System.out.println("File extensions: " + String.join(", ", FILE_EXTENSIONS));
        
        if (!connectFTP()) {
            System.err.println("Failed to establish initial FTP connection. Exiting.");
            return;
        }
        
        try {
            Path dirPath = Paths.get(MONITORED_DIRECTORY);
            initializeFileMap(dirPath);
            
            while (true) {
                try {
                    if (!connectFTP()) {
                        System.err.println("Lost FTP connection. Retrying...");
                        Thread.sleep(5000);
                        continue;
                    }
                    
                    List<Path> modifiedFiles = checkModifiedFiles(dirPath);
                    
                    if (!modifiedFiles.isEmpty()) {
                        System.out.println("Found " + modifiedFiles.size() + " modified file(s)");
                        
                        for (Path file : modifiedFiles) {
                            System.out.println("Uploading: " + file);
                            if (transferSingleFile(file.toFile(), REMOTE_DIRECTORY, controlWriter, controlReader)) {
                                System.out.println("Successfully uploaded: " + file);
                            } else {
                                System.err.println("Failed to upload: " + file);
                            }
                        }
                    }
                    
                    Thread.sleep(POLLING_INTERVAL);
                    
                } catch (IOException e) {
                    System.err.println("Error during monitoring: " + e.getMessage());
                } catch (InterruptedException e) {
                    System.err.println("Monitoring interrupted: " + e.getMessage());
                    break;
                }
            }
        } catch (IOException e) {
            System.err.println("Error initializing file monitoring: " + e.getMessage());
            e.printStackTrace();
        } finally {
            try {
                if (controlWriter != null) controlWriter.close();
                if (controlReader != null) controlReader.close();
                if (controlSocket != null) controlSocket.close();
            } catch (IOException e) {
                System.err.println("Error closing FTP connection: " + e.getMessage());
            }
        }
    }
    
    private static void initializeFileMap(Path dirPath) throws IOException {
        Files.walk(dirPath)
            .filter(Files::isRegularFile)
            .filter(main::hasWatchedExtension)
            .forEach(file -> {
                try {
                    fileModificationTimes.put(file, Files.getLastModifiedTime(file));
                    System.out.println("Monitoring: " + file);
                } catch (IOException e) {
                    System.err.println("Error reading file: " + file);
                }
            });
        
        System.out.println("Monitoring " + fileModificationTimes.size() + " files");
    }
    
    private static boolean hasWatchedExtension(Path file) {
        String fileName = file.toString().toLowerCase();
        for (String ext : FILE_EXTENSIONS) {
            if (fileName.endsWith(ext.toLowerCase())) {
                return true;
            }
        }
        return false;
    }
    
    private static List<Path> checkModifiedFiles(Path dirPath) throws IOException {
        List<Path> modifiedFiles = new ArrayList<>();
        
        for (Map.Entry<Path, FileTime> entry : fileModificationTimes.entrySet()) {
            Path file = entry.getKey();
            FileTime lastModifiedTime = entry.getValue();
            
            if (Files.exists(file)) {
                FileTime currentModTime = Files.getLastModifiedTime(file);
                if (lastModifiedTime.compareTo(currentModTime) < 0) {
                    System.out.println("File changed: " + file);
                    modifiedFiles.add(file);
                    fileModificationTimes.put(file, currentModTime);
                }
            }
        }
        
        Files.walk(dirPath)
            .filter(Files::isRegularFile)
            .filter(main::hasWatchedExtension)
            .forEach(file -> {
                if (!fileModificationTimes.containsKey(file)) {
                    try {
                        System.out.println("New file detected: " + file);
                        FileTime currentModTime = Files.getLastModifiedTime(file);
                        fileModificationTimes.put(file, currentModTime);
                        modifiedFiles.add(file);
                    } catch (IOException e) {
                        System.err.println("Error reading new file: " + file);
                    }
                }
            });
        
        return modifiedFiles;
    }
    
    private static String createRemotePathForFile(Path filePath) {
        Path basePath = Paths.get(MONITORED_DIRECTORY).toAbsolutePath();
        Path relativePath = basePath.relativize(filePath.toAbsolutePath());
        
        String remoteFilePath = REMOTE_DIRECTORY;
        if (!remoteFilePath.endsWith("/")) {
            remoteFilePath += "/";
        }
        
        if (relativePath.getParent() != null) {
            remoteFilePath += relativePath.getParent().toString().replace("\\", "/");
            if (!remoteFilePath.endsWith("/")) {
                remoteFilePath += "/";
            }
        }
        
        return remoteFilePath;
    }
    
    private static Socket setupDataConnection(BufferedWriter writer, BufferedReader reader) throws IOException {
        if (USE_PASSIVE_MODE) {
            return setupPassiveMode(writer, reader);
        } else {
            return setupActiveMode(writer, reader);
        }
    }
    
    private static Socket setupPassiveMode(BufferedWriter writer, BufferedReader reader) throws IOException {
        String pasvResponse = sendCommand(writer, reader, "PASV");
        if (!pasvResponse.startsWith("227")) {
            System.err.println("Passive mode failed: " + pasvResponse);
            return null;
        }
        
        int startIndex = pasvResponse.indexOf('(');
        int endIndex = pasvResponse.indexOf(')', startIndex);
        String[] parts = pasvResponse.substring(startIndex + 1, endIndex).split(",");
        
        int dataPort = (Integer.parseInt(parts[4]) * 256) + Integer.parseInt(parts[5]);
        String dataHost = parts[0] + "." + parts[1] + "." + parts[2] + "." + parts[3];
        
        return new Socket(dataHost, dataPort);
    }
    
    private static Socket setupActiveMode(BufferedWriter writer, BufferedReader reader) throws IOException {
        InetAddress localAddress = InetAddress.getLocalHost();
        byte[] addressBytes = localAddress.getAddress();
        
        ServerSocket serverSocket = new ServerSocket(ACTIVE_MODE_PORT);
        
        int p1 = ACTIVE_MODE_PORT / 256;
        int p2 = ACTIVE_MODE_PORT % 256;
        
        String portCommand = String.format("PORT %d,%d,%d,%d,%d,%d", 
            addressBytes[0] & 0xFF, addressBytes[1] & 0xFF, 
            addressBytes[2] & 0xFF, addressBytes[3] & 0xFF, p1, p2);
        
        String response = sendCommand(writer, reader, portCommand);
        if (!response.startsWith("200")) {
            System.err.println("Active mode failed: " + response);
            serverSocket.close();
            return null;
        }
        
        serverSocket.setSoTimeout(30000);
        Socket dataSocket = serverSocket.accept();
        serverSocket.close();
        
        return dataSocket;
    }
    
    private static boolean transferSingleFile(File file, String remoteDirectory, 
                                             BufferedWriter writer, BufferedReader reader) throws IOException {
        try {
            System.out.println("Uploading file: " + file.getName() + " to " + remoteDirectory);
            
            Socket dataSocket = setupDataConnection(writer, reader);
            if (dataSocket == null) {
                return false;
            }
            
            String remoteFilePath = remoteDirectory + file.getName();
            String response = sendCommand(writer, reader, "STOR " + remoteFilePath);
            
            if (!response.startsWith("150") && !response.startsWith("125")) {
                System.err.println("Failed to initiate file transfer: " + response);
                dataSocket.close();
                return false;
            }
            
            BufferedInputStream fileInput = new BufferedInputStream(new FileInputStream(file));
            BufferedOutputStream dataOutput = new BufferedOutputStream(dataSocket.getOutputStream());
            
            byte[] buffer = new byte[8192];
            int bytesRead;
            long totalBytes = 0;
            
            while ((bytesRead = fileInput.read(buffer)) != -1) {
                dataOutput.write(buffer, 0, bytesRead);
                totalBytes += bytesRead;
            }
            
            dataOutput.flush();
            dataOutput.close();
            fileInput.close();
            dataSocket.close();
            
            response = readResponse(reader);
            if (!response.startsWith("226") && !response.startsWith("250")) {
                System.err.println("Transfer incomplete: " + response);
                return false;
            }
            
            System.out.println("Transfer complete: " + totalBytes + " bytes sent.");
            return true;
            
        } catch (IOException e) {
            System.err.println("Error transferring file: " + e.getMessage());
            throw e;
        }
    }
    
    private static String sendCommand(BufferedWriter writer, BufferedReader reader, String command) throws IOException {
        String logCommand = command.startsWith("PASS ") ? "PASS ****" : command;
        System.out.println("Sending command: " + logCommand);
        
        writer.write(command + "\r\n");
        writer.flush();
        
        return readResponse(reader);
    }
    
    private static String readResponse(BufferedReader reader) throws IOException {
        StringBuilder response = new StringBuilder();
        String line;
        
        while ((line = reader.readLine()) != null) {
            response.append(line).append("\n");
            
            if (line.length() >= 4 && Character.isDigit(line.charAt(0)) && 
                Character.isDigit(line.charAt(1)) && Character.isDigit(line.charAt(2)) && 
                line.charAt(3) == ' ') {
                break;
            }
        }
        
        String responseStr = response.toString().trim();
        System.out.println("Server response: " + responseStr);
        return responseStr;
    }
}
