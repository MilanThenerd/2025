import java.io.*;
import java.net.*;
import java.util.*;
import java.time.*;

public class AlarmSystem {
    private static final String SMTP_SERVER = "localhost"; // Change to your SMTP server
    private static final int SMTP_PORT = 25;
    private static final String FROM_EMAIL = "alarm@yourhouse.com";
    private static final String TO_EMAIL = "you@yourhouse.com";
    private static final String SUBJECT = "Home Alarm Notification";
    
    public static void main(String[] args) {
        System.out.println("Home Alarm System - Email Notification Simulator");
        System.out.println("Press 1 for Front Door, 2 for Back Door, 3 for Motion Detector, q to quit");
        
        try (Scanner scanner = new Scanner(System.in)) {
            while (true) {
                String input = scanner.nextLine().toLowerCase();
                
                if (input.equals("q")) {
                    System.out.println("Alarm system shutting down...");
                    break;
                }
                
                String message = "";
                switch (input) {
                    case "1":
                        message = "ALERT: Front Door opened at " + LocalDateTime.now();
                        break;
                    case "2":
                        message = "ALERT: Back Door opened at " + LocalDateTime.now();
                        break;
                    case "3":
                        message = "ALERT: Motion detected at " + LocalDateTime.now();
                        break;
                    default:
                        System.out.println("Invalid input. Use 1, 2, 3 or q to quit.");
                        continue;
                }
                
                System.out.println("Sending alert: " + message);
                sendEmail(message);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    
    private static void sendEmail(String body) {
        try (Socket socket = new Socket(SMTP_SERVER, SMTP_PORT);
             BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
             PrintWriter out = new PrintWriter(socket.getOutputStream(), true)) {
            
            // Read server welcome message
            System.out.println("Server: " + in.readLine());
            
            // Send SMTP commands
            sendCommand(out, in, "HELO " + InetAddress.getLocalHost().getHostName());
            sendCommand(out, in, "MAIL FROM:<" + FROM_EMAIL + ">");
            sendCommand(out, in, "RCPT TO:<" + TO_EMAIL + ">");
            sendCommand(out, in, "DATA");
            
            // Send email headers and body
            out.println("From: " + FROM_EMAIL);
            out.println("To: " + TO_EMAIL);
            out.println("Subject: " + SUBJECT);
            out.println("Date: " + new Date());
            out.println(); // Empty line separates headers from body
            out.println(body);
            
            // End of message
            sendCommand(out, in, ".");
            sendCommand(out, in, "QUIT");
            
            System.out.println("Email sent successfully!");
            
        } catch (Exception e) {
            System.err.println("Failed to send email: " + e.getMessage());
        }
    }
    
    private static void sendCommand(PrintWriter out, BufferedReader in, String command) throws IOException {
        System.out.println("Client: " + command);
        out.println(command);
        String response = in.readLine();
        System.out.println("Server: " + response);
        
        // Check if response code indicates success (2xx or 3xx)
        if (response == null || response.length() < 3 || 
            (response.charAt(0) != '2' && response.charAt(0) != '3')) {
            throw new IOException("SMTP command failed: " + response);
        }
    }
}