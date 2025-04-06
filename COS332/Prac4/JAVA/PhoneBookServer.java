import java.io.*;
import java.net.*;
import java.util.*;
import java.nio.file.*;
import java.nio.charset.StandardCharsets;
import java.util.concurrent.ConcurrentHashMap;

public class PhoneBookServer {
  private static final int PORT = 55555;
  private static final Map<String, Contact> phoneBook = new ConcurrentHashMap<>();
  private static final String UPLOAD_DIR = "uploads/";
  private static final String DATA_FILE = "phonebook.dat";
  private static final int MAX_FILE_SIZE = 1024 * 5000; // 5MB

  public static void main(String[] args) {
    // Create upload directory if it doesn't exist
    new File(UPLOAD_DIR).mkdirs();

    // Load existing data
    loadPhoneBook();

    try (ServerSocket serverSocket = new ServerSocket(PORT)) {
      System.out.println("PhoneBook server running on port " + PORT);
      System.out.println("Upload directory: " + new File(UPLOAD_DIR).getAbsolutePath());

      while (true) {
        Socket clientSocket = serverSocket.accept();
        new Thread(new ClientHandler(clientSocket)).start();
      }
    } catch (IOException e) {
      System.err.println("Server error: " + e.getMessage());
    }
  }

  private static void loadPhoneBook() {
    File file = new File(DATA_FILE);
    if (file.exists()) {
      try (ObjectInputStream ois = new ObjectInputStream(new FileInputStream(file))) {
        @SuppressWarnings("unchecked")
        Map<String, Contact> loaded = (Map<String, Contact>) ois.readObject();
        phoneBook.putAll(loaded);
        System.out.println("Loaded " + phoneBook.size() + " contacts from disk");
      } catch (Exception e) {
        System.err.println("Error loading phone book: " + e.getMessage());
      }
    }
  }

  private static void savePhoneBook() {
    try (ObjectOutputStream oos = new ObjectOutputStream(new FileOutputStream(DATA_FILE))) {
      oos.writeObject(phoneBook);
    } catch (IOException e) {
      System.err.println("Error saving phone book: " + e.getMessage());
    }
  }

  static class Contact implements Serializable {
    String name;
    String surname;
    String phoneNumber;
    String email;
    String imagePath;

    public Contact(String name, String surname, String phoneNumber, String email) {
      this.name = name;
      this.surname = surname;
      this.phoneNumber = phoneNumber;
      this.email = email;
    }
  }

  static class ClientHandler implements Runnable {
    private final Socket clientSocket;

    public ClientHandler(Socket clientSocket) {
      this.clientSocket = clientSocket;
    }

    @Override
    public void run() {
      try (BufferedReader in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
          PrintWriter out = new PrintWriter(new OutputStreamWriter(clientSocket.getOutputStream()), true)) {

        String requestLine = in.readLine();
        String[] requestParts = requestLine.split(" ");
        if (requestParts.length != 3) {
          sendErrorResponse(out, 400, "Bad Request");
          return;
        }

        String method = requestParts[0];
        String path = requestParts[1];

        if (method.equals("GET") && path.startsWith("/image")) {
          handleImageRequest(path, out);
          return;
        }

        if (method.equals("POST") && path.equals("/upload")) {
          handleFileUpload(in, out);
          return;
        }

        Request request = parseRequest(requestLine, in);
        if (request == null) {
          sendErrorResponse(out, 400, "Bad Request");
          return;
        }

        Response response = processRequest(request);
        sendResponse(out, response);

      } catch (IOException e) {
        System.err.println("Request handling error: " + e.getMessage());
      } finally {
        try {
          clientSocket.close();
        } catch (IOException e) {
          System.err.println("Error closing client socket: " + e.getMessage());
        }
      }
    }

    private void handleImageRequest(String path, PrintWriter out) throws IOException {
      Map<String, String> params = parseQueryParams(path);
      String name = params.get("name");
      if (name == null || !phoneBook.containsKey(name)) {
        sendErrorResponse(out, 404, "Not Found");
        return;
      }

      Contact contact = phoneBook.get(name);
      if (contact.imagePath == null) {
        sendErrorResponse(out, 404, "No Image Found");
        return;
      }

      File imageFile = new File(contact.imagePath);
      if (!imageFile.exists()) {
        sendErrorResponse(out, 404, "Image File Not Found");
        return;
      }

      String mimeType = Files.probeContentType(imageFile.toPath());
      if (mimeType == null) {
        mimeType = "application/octet-stream";
      }

      byte[] imageData = Files.readAllBytes(imageFile.toPath());

      OutputStream output = clientSocket.getOutputStream();
      output.write(("HTTP/1.1 200 OK\r\n").getBytes());
      output.write(("Content-Type: " + mimeType + "\r\n").getBytes());
      output.write(("Content-Length: " + imageData.length + "\r\n").getBytes());
      output.write("\r\n".getBytes());
      output.write(imageData);
      output.flush();
    }

    private void handleFileUpload(BufferedReader in, PrintWriter out) throws IOException {
      InputStream input = clientSocket.getInputStream();
      StringBuilder headerBuffer = new StringBuilder();
      String line;
      while ((line = in.readLine()) != null && !line.isEmpty()) {
        headerBuffer.append(line).append("\r\n");
      }

      String headers = headerBuffer.toString();
      String contentType = extractHeader(headers, "Content-Type");
      String contentLengthStr = extractHeader(headers, "Content-Length");

      if (contentType == null || !contentType.contains("multipart/form-data") || !contentType.contains("boundary=")) {
        sendErrorResponse(out, 400, "Invalid or missing Content-Type");
        return;
      }

      if (contentLengthStr == null) {
        sendErrorResponse(out, 400, "Missing Content-Length header");
        return;
      }

      int contentLength = Integer.parseInt(contentLengthStr);
      if (contentLength > MAX_FILE_SIZE) {
        sendErrorResponse(out, 413, "File too large");
        return;
      }

      String boundary = "--" + contentType.split("boundary=")[1].trim();
      ByteArrayOutputStream fileHeader = new ByteArrayOutputStream();

      // Read file part headers
      while ((line = in.readLine()) != null && !line.isEmpty()) {
        fileHeader.write(line.getBytes());
        fileHeader.write("\r\n".getBytes());
      }

      // Extract filename and contact name
      String fileHeaderStr = fileHeader.toString();
      String contactName = extractContactName(fileHeaderStr);

      String filename = contactName + "_" + System.currentTimeMillis() + ".jpg";

      if (filename == null || contactName == null || !phoneBook.containsKey(contactName)) {
        sendErrorResponse(out, 400, "Invalid filename or contact name");
        return;
      }

      // Save file
      String extension = filename.substring(filename.lastIndexOf('.'));
      String uniqueFilename = contactName + "_" + System.currentTimeMillis() + extension;
      Path filePath = Paths.get(UPLOAD_DIR, uniqueFilename);

      try (OutputStream fileOut = Files.newOutputStream(filePath)) {
        byte[] buffer = new byte[8192];
        int bytesRead;
        int totalRead = 0;
        boolean boundaryFound = false;

        while (totalRead < contentLength && (bytesRead = input.read(buffer)) != -1) {
          totalRead += bytesRead;

          String dataChunk = new String(buffer, 0, bytesRead);
          int boundaryIndex = dataChunk.indexOf(boundary);

          if (boundaryIndex != -1) {
            fileOut.write(buffer, 0, boundaryIndex - 4);
            boundaryFound = true;
            break;
          } else {
            fileOut.write(buffer, 0, bytesRead);
          }
        }

        if (!boundaryFound) {
          System.err.println("Warning: Boundary not found, possible incomplete upload.");
        }
      }

      // Update contact with image path
      Contact contact = phoneBook.get(contactName);
      if (contact.imagePath != null) {
        try {
          Files.deleteIfExists(Paths.get(contact.imagePath));
        } catch (IOException e) {
          System.err.println("Failed to delete old image: " + e.getMessage());
        }
      }
      contact.imagePath = filePath.toString();
      savePhoneBook();

      // Send success response
      sendResponse(out, new Response(200, "OK", "<html><body><h1>File uploaded successfully</h1></body></html>"));
    }


    
    private String extractContactName(String fileHeader) {
      String[] parts = fileHeader.split(";");
      for (String part : parts) {
        if (part.contains("name=")) {
          return part.split("=")[1].replace("\"", "").trim();
        }
      }
      return null;
    }

    private Request parseRequest(String requestLine, BufferedReader in) throws IOException {
      String[] requestParts = requestLine.split(" ");
      String method = requestParts[0];
      String path = requestParts[1];

      // Read headers
      Map<String, String> headers = new HashMap<>();
      String line;
      while ((line = in.readLine()) != null && !line.isEmpty()) {
        int colon = line.indexOf(':');
        if (colon > 0) {
          String key = line.substring(0, colon).trim();
          String value = line.substring(colon + 1).trim();
          headers.put(key, value);
        }
      }

      // Read body if it exists
      StringBuilder body = new StringBuilder();
      if (headers.containsKey("Content-Length")) {
        int contentLength = Integer.parseInt(headers.get("Content-Length"));
        for (int i = 0; i < contentLength; i++) {
          body.append((char) in.read());
        }
      }

      return new Request(method, path, headers, body.toString());
    }

    private Response processRequest(Request request) {
      if (request.path.equals("/favicon.ico")) {
        return new Response(404, "Not Found", "<h1>404 Not Found</h1>");
      }

      if (request.method.equals("GET")) {
        handlePhoneBookOperations(request.path);
        return buildPhoneBookResponse();
      }

      return new Response(400, "Bad Request", "<h1>Unsupported method</h1>");
    }

    private void handlePhoneBookOperations(String path) {
      Map<String, String> params = parseQueryParams(path);

      if (path.startsWith("/add")) {
        handleAddContact(params);
      } else if (path.contains("delete")) {
        handleDeleteContact(params);
      } else if (path.contains("edit")) {
        handleEditContact(params);
      }
    }

    private void handleAddContact(Map<String, String> params) {
      String name = params.get("name");
      String surname = params.get("surname");
      String phone = params.get("phone");
      String email = params.get("email");

      if (name != null && !name.isEmpty() && surname != null && !surname.isEmpty()) {
        phoneBook.put(name, new Contact(name, surname, phone, email));
        savePhoneBook();
      }
    }

    private void handleDeleteContact(Map<String, String> params) {
      String name = params.get("name");
      if (name != null && phoneBook.containsKey(name)) {
        Contact contact = phoneBook.get(name);
        if (contact.imagePath != null) {
          try {
            Files.deleteIfExists(Paths.get(contact.imagePath));
          } catch (IOException e) {
            System.err.println("Failed to delete image: " + e.getMessage());
          }
        }
        phoneBook.remove(name);
        savePhoneBook();
      }
    }

    private void handleEditContact(Map<String, String> params) {
      String name = params.get("name");
      String newSurname = params.get("surname");
      String newPhone = params.get("phone");
      String newEmail = params.get("email");

      if (name != null && phoneBook.containsKey(name)) {
        Contact contact = phoneBook.get(name);
        if (newSurname != null && !newSurname.isEmpty()) {
          contact.surname = newSurname;
        }
        if (newPhone != null && !newPhone.isEmpty()) {
          contact.phoneNumber = newPhone;
        }
        if (newEmail != null && !newEmail.isEmpty()) {
          contact.email = newEmail;
        }
        savePhoneBook();
      }
    }

    private Map<String, String> parseQueryParams(String path) {
      Map<String, String> params = new HashMap<>();
      int queryStart = path.indexOf('?');
      if (queryStart == -1)
        return params;

      String query = path.substring(queryStart + 1);
      for (String param : query.split("&")) {
        String[] keyValue = param.split("=");
        if (keyValue.length == 2) {
          params.put(keyValue[0], URLDecoder.decode(keyValue[1], StandardCharsets.UTF_8));
        }
      }
      return params;
    }

    private Response buildPhoneBookResponse() {
      StringBuilder html = new StringBuilder();
      html.append("<html><head><title>PhoneBook Server</title>");
      html.append("<style>");
      html.append("body { font-family: Arial, sans-serif; margin: 20px; }");
      html.append("table { border-collapse: collapse; width: 100%; }");
      html.append("th, td { border: 1px solid #ddd; padding: 8px; text-align: left; }");
      html.append("th { background-color: #f2f2f2; }");
      html.append("tr:nth-child(even) { background-color: #f9f9f9; }");
      html.append("form { margin-bottom: 20px; }");
      html.append("</style></head><body>");
      html.append("<h1>PhoneBook Server</h1>");
      html.append(buildAddContactForm());
      html.append(buildImageUploadForm());
      html.append("<h2>Contact List</h2>");
      html.append(buildContactTable());
      html.append("</body></html>");

      return new Response(200, "OK", html.toString());
    }

    private String buildAddContactForm() {
      return "<form method='get' action='/add'>" +
          "Name: <input type='text' name='name' required><br>" +
          "Surname: <input type='text' name='surname' required><br>" +
          "Phone: <input type='text' name='phone'><br>" +
          "Email: <input type='email' name='email'><br>" +
          "<input type='submit' value='Add Contact'>" +
          "</form><br>";
    }

    private String buildImageUploadForm() {
      StringBuilder form = new StringBuilder();
      form.append("<form method='GET' action='/upload' enctype='multipart/form-data'>");
      form.append("Select contact: <select name='contact' required>");
      for (String name : phoneBook.keySet()) {
        form.append("<option value='").append(escapeHtml(name)).append("'>")
            .append(escapeHtml(name)).append("</option>");
      }
      form.append("</select><br>");
      form.append("Upload image: <input type='file' name='file' accept='image/*' required><br>");
      form.append("<input type='submit' value='Upload'>");
      form.append("</form><br>");
      return form.toString();
    }

    private String buildContactTable() {
      if (phoneBook.isEmpty()) {
        return "<p>No contacts found</p>";
      }

      StringBuilder table = new StringBuilder();
      table.append(
          "<table><tr><th>Name</th><th>Surname</th><th>Phone</th><th>Email</th><th>Image</th><th>Actions</th></tr>");

      for (Contact contact : phoneBook.values()) {
        table.append("<tr>")
            .append("<td>").append(escapeHtml(contact.name)).append("</td>")
            .append("<td>").append(escapeHtml(contact.surname)).append("</td>")
            .append("<td>").append(escapeHtml(contact.phoneNumber != null ? contact.phoneNumber : "")).append("</td>")
            .append("<td>").append(escapeHtml(contact.email != null ? contact.email : "")).append("</td>")
            .append("<td>")
            .append(contact.imagePath != null
                ? "<img src='/image?name=" + encodeUrl(contact.name) + "' height='50' alt='Contact image'>"
                : "No image")
            .append("</td>")
            .append("<td>")
            .append("<a href='/?delete&name=").append(encodeUrl(contact.name)).append("'>Delete</a>")
            .append(" | ")
            .append(buildEditForm(contact))
            .append("</td>")
            .append("</tr>");
      }

      table.append("</table>");
      return table.toString();
    }

    private String buildEditForm(Contact contact) {
      return "<form method='get' action='/edit' style='display:inline;'>" +
          "<input type='hidden' name='name' value='" + escapeHtml(contact.name) + "'>" +
          "<input type='text' name='surname' placeholder='New surname' value='" + escapeHtml(contact.surname) + "'>" +
          "<input type='text' name='phone' placeholder='New phone' value='" +
          (contact.phoneNumber != null ? escapeHtml(contact.phoneNumber) : "") + "'>" +
          "<input type='email' name='email' placeholder='New email' value='" +
          (contact.email != null ? escapeHtml(contact.email) : "") + "'>" +
          "<input type='submit' value='Edit'>" +
          "</form>";
    }

    private String escapeHtml(String input) {
      if (input == null)
        return "";
      return input.replace("&", "&amp;")
          .replace("<", "&lt;")
          .replace(">", "&gt;")
          .replace("\"", "&quot;")
          .replace("'", "&#39;");
    }

    private String encodeUrl(String input) {
      return URLEncoder.encode(input, StandardCharsets.UTF_8);
    }

    private void sendResponse(PrintWriter out, Response response) {
      out.println("HTTP/1.1 " + response.statusCode + " " + response.statusMessage);
      out.println("Content-Type: text/html");
      out.println("Content-Length: " + response.body.length());
      out.println("Connection: close");
      out.println();
      out.println(response.body);
    }

    private void sendErrorResponse(PrintWriter out, int statusCode, String statusMessage) {
      String body = "<html><body><h1>Error " + statusCode + ": " + statusMessage + "</h1></body></html>";
      out.println("HTTP/1.1 " + statusCode + " " + statusMessage);
      out.println("Content-Type: text/html");
      out.println("Content-Length: " + body.length());
      out.println("Connection: close");
      out.println();
      out.println(body);
    }

    private String extractHeader(String headers, String headerName) {
      String[] lines = headers.split("\r\n");
      for (String line : lines) {
        if (line.startsWith(headerName + ":")) {
          return line.substring(headerName.length() + 1).trim();
        }
      }
      return null;
    }

    private String extractFilename(String fileHeader) {
      String[] parts = fileHeader.split(";");
      for (String part : parts) {
        if (part.contains("filename=")) {
          return part.split("=")[1].replace("\"", "").trim();
        }
      }
      return null;
    }
  }

  static class Request {
    final String method;
    final String path;
    final Map<String, String> headers;
    final String body;

    public Request(String method, String path, Map<String, String> headers, String body) {
      this.method = method;
      this.path = path;
      this.headers = headers;
      this.body = body;
    }
  }

  static class Response {
    final int statusCode;
    final String statusMessage;
    final String body;

    public Response(int statusCode, String statusMessage, String body) {
      this.statusCode = statusCode;
      this.statusMessage = statusMessage;
      this.body = body;
    }
  }
}