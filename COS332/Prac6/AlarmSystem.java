package Prac6;
import java.io.*;
import java.net.*;
import java.time.*;
import java.util.*;

public class AlarmSystem {
    private static final String SMTP_SERVER = "localhost";
    private static final int SMTP_PORT = 25;
    private static final String FROM_EMAIL = "test@test";
    private static final String TO_EMAIL = "test@test";
    
    
    public static void main(String[] args) {
        System.out.println("Home Alarm System - SMTP Notification");
        System.out.println("Commands: 1=Front Door, 2=Back Door, 3=Motion, q=Quit");
        
        try (Scanner scanner = new Scanner(System.in)) {
            while (true) {
                System.out.print("> ");
                String input = scanner.nextLine().toLowerCase();
                
                if (input.equals("q")) {
                    System.out.println("Alarm system shutting down...");
                    break;
                }
                
                String alertType;
                switch (input) {
                    case "1":
                        alertType = "Front Door opened";
                        break;
                    case "2":
                        alertType = "Back Door opened";
                        break;
                    case "3":
                        alertType = "Motion detected";
                        break;
                    default:
                        System.out.println("Invalid input. Try 1, 2, 3 or q");
                        continue;
                }
                
                String timestamp = LocalDateTime.now().toString();
                String message = createEmailMessage(alertType, timestamp);
                System.out.println("Sending alert: " + alertType + " at " + timestamp);
                
                if (sendRawEmail(message)) {
                    System.out.println("Alert sent successfully!");
                } else {
                    System.out.println("Failed to send alert");
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    
    private static String createEmailMessage(String alertType, String timestamp) {
        return "From: " + FROM_EMAIL + "\r\n" +
               "To: " + TO_EMAIL + "\r\n" +
               "Date: " + new Date() + "\r\n" +
               "Subject: ALARM: " + alertType + "\r\n" +
               "\r\n" +
               "ALERT: " + alertType + "\r\n" +
               "Timestamp: " + timestamp + "\r\n" +
               "\r\n" +
               "This is an automated alert from your home alarm system.\r\n" +
               "Please investigate immediately if unexpected.\r\n";
    }
    
    private static boolean sendRawEmail(String message) {
        try (Socket socket = new Socket(SMTP_SERVER, SMTP_PORT);
             BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
             PrintWriter out = new PrintWriter(socket.getOutputStream(), true)) {

            String response = in.readLine();
            System.out.println("S: " + response);
            if (!response.startsWith("220")) return false;
            
            if (!sendCommand(out, in, "HELO " + InetAddress.getLocalHost().getHostName())) return false;
            if (!sendCommand(out, in, "MAIL FROM:<" + FROM_EMAIL + ">")) return false;
            if (!sendCommand(out, in, "RCPT TO:<" + TO_EMAIL + ">")) return false;
            if (!sendCommand(out, in, "DATA")) return false;
            
            out.print(message);
            if (!sendCommand(out, in, ".")) return false;
            
            return sendCommand(out, in, "QUIT");
            
        } catch (Exception e) {
            System.err.println("SMTP error: " + e.getMessage());
            return false;
        }
    }
    
    private static boolean sendCommand(PrintWriter out, BufferedReader in, String command) throws IOException {
        System.out.println("C: " + command);
        out.println(command);
        
        String response = in.readLine();
        System.out.println("S: " + response);
        
        return response != null && response.length() >= 3 && 
               (response.charAt(0) == '2' || response.charAt(0) == '3');
    }
}