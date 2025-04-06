#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <sstream>
#include <map>

const int PORT = 3000;
const int BUFFER_SIZE = 4096;

class Friend
{
public:
  std::string name;
  std::string surname;
  std::string phoneNumber;
  std::string fileName;

  Friend() = default;
  Friend(std::string name, std::string surname, std::string phoneNumber, std::string fileName)
  {
    this->name = name;
    this->surname = surname;
    this->phoneNumber = phoneNumber;
    this->fileName = fileName;
  }
};

std::map<std::string, Friend> phoneBook;

void log_request(const std::string& request) 
{
  size_t line_end = request.find("\r\n");
  if (line_end == std::string::npos) return;
  
  std::string request_line = request.substr(0, line_end);

  time_t now = time(0);
  char time_str[100];
  strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", localtime(&now));

  std::cout << "[" << time_str << "] " << request_line << std::endl;
}

std::string read_full_request(int client_socket)
{
  std::string full_request;
  char buffer[BUFFER_SIZE];
  while (true)
  {
    ssize_t bytes_read = recv(client_socket, buffer, BUFFER_SIZE, 0);
    if (bytes_read <= 0)
      break;

    full_request.append(buffer, bytes_read);

    if (full_request.find("\r\n\r\n") != std::string::npos)
    {
      size_t content_length = 0;
      size_t cl_pos = full_request.find("Content-Length: ");
      if (cl_pos != std::string::npos)
      {
        size_t cl_end = full_request.find("\r\n", cl_pos);
        std::string cl_str = full_request.substr(cl_pos + 16, cl_end - (cl_pos + 16));
        content_length = std::stoul(cl_str);

        size_t header_end = full_request.find("\r\n\r\n") + 4;
        if (full_request.size() - header_end >= content_length)
        {
          break;
        }
      }
      else
      {
        break;
      }
    }
  }
  return full_request;
}

std::string extract_form_data(const std::string &content, const std::string &boundary, const std::string &field_name)
{
  std::string pattern = "--" + boundary + "\r\nContent-Disposition: form-data; name=\"" + field_name + "\"\r\n\r\n";
  size_t start_pos = content.find(pattern);
  if (start_pos == std::string::npos)
    return "";

  start_pos += pattern.length();
  size_t end_pos = content.find("\r\n--" + boundary, start_pos);
  if (end_pos == std::string::npos)
    return "";

  return content.substr(start_pos, end_pos - start_pos);
}

void handle_image_upload(int client_socket, const std::string &boundary, const std::string &content)
{
  std::string name = extract_form_data(content, boundary, "name");

  if (phoneBook.count(name) > 0)
  {
    std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
    response += "<html><body><h1>User Already Exists</h1>";
    response += "<a href=\"/\">Back to contacts</a>";
    response += "</body></html>";
    send(client_socket, response.c_str(), response.size(), 0);
    return;
  }
  
  std::string surname = extract_form_data(content, boundary, "surname");
  std::string phone = extract_form_data(content, boundary, "phone");

  std::string file_part = content.substr(content.find("filename=\""));
  size_t filename_start = file_part.find("filename=\"") + 10;
  size_t filename_end = file_part.find("\"", filename_start);
  std::string filename = file_part.substr(filename_start, filename_end - filename_start);

  size_t file_content_start = file_part.find("\r\n\r\n") + 4;
  size_t file_content_end = file_part.rfind("\r\n--" + boundary);
  std::string file_content = file_part.substr(file_content_start, file_content_end - file_content_start);

  std::ofstream outfile("uploads/" + filename, std::ios::binary);
  if (!outfile)
  {
    const char *response = "HTTP/1.1 500 Internal Server Error\r\nContent-Type: text/plain\r\n\r\nFailed to save file";
    send(client_socket, response, strlen(response), 0);
    return;
  }
  outfile.write(file_content.c_str(), file_content.size());
  outfile.close();

  phoneBook.insert({name, Friend(name, surname, phone, "uploads/" + filename)});

  std::string redirect = "HTTP/1.1 303 See Other\r\nLocation: /\r\n\r\n";
  send(client_socket, redirect.c_str(), redirect.size(), 0);
}

void handle_edit_request(int client_socket, const std::string &name)
{
  if (phoneBook.count(name) == 0)
  {
    const char *response = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\nContact not found";
    send(client_socket, response, strlen(response), 0);
    return;
  }

  Friend &f = phoneBook[name];
  std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
  response += "<!DOCTYPE html>";
  response += "<html><head>";
  response += "<style>";
  response += "body { font-family: Arial, sans-serif; margin: 20px; background-color: #f4f4f9; }";
  response += "h1 { color: #333; text-align: center; }";
  response += "form { background: #fff; padding: 20px; border-radius: 8px; box-shadow: 0 2px 4px rgba(0,0,0,0.1); max-width: 500px; margin: 20px auto; }";
  response += "input[type='text'], input[type='file'] { width: 100%; padding: 8px; margin: 8px 0; border: 1px solid #ddd; border-radius: 4px; }";
  response += "input[type='submit'] { background-color: #4CAF50; color: white; padding: 10px 15px; border: none; border-radius: 4px; cursor: pointer; }";
  response += "input[type='submit']:hover { background-color: #45a049; }";
  response += "img { max-width: 100px; max-height: 100px; border-radius: 4px; }";
  response += "a { color: #4CAF50; text-decoration: none; }";
  response += "a:hover { text-decoration: underline; }";
  response += "</style>";
  response += "</head><body>";
  response += "<form action=\"/update\" method=\"post\" enctype=\"multipart/form-data\">";
  response += "<h1>Edit Contact</h1>";
  response += "<input type=\"hidden\" name=\"original_name\" value=\"" + f.name + "\">";
  response += "Name: <input type=\"text\" name=\"name\" value=\"" + f.name + "\" required><br>";
  response += "Surname: <input type=\"text\" name=\"surname\" value=\"" + f.surname + "\" required><br>";
  response += "Phone: <input type=\"text\" name=\"phone\" value=\"" + f.phoneNumber + "\" required><br>";
  response += "Current Image: <img src=\"" + f.fileName + "\"><br>";
  response += "New Image (optional): <input type=\"file\" name=\"myfile\" accept=\"image/*\"><br>";
  response += "<input type=\"submit\" value=\"Update Contact\">";
  response += "<br><a href=\"/\">Cancel</a>";
  response += "</form>";
  response += "</body></html>";

  send(client_socket, response.c_str(), response.size(), 0);
}

void handle_delete(int client_socket, const std::string &name)
{
  auto it = phoneBook.find(name);
  if (it == phoneBook.end())
  {
    const char *response = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\nContact not found";
    send(client_socket, response, strlen(response), 0);
    return;
  }

  phoneBook.erase(it);

  std::string redirect = "HTTP/1.1 303 See Other\r\nLocation: /\r\n\r\n";
  send(client_socket, redirect.c_str(), redirect.size(), 0);
}

void handle_update(int client_socket, const std::string &boundary, const std::string &content)
{
  std::string original_name = extract_form_data(content, boundary, "original_name");
  std::string name = extract_form_data(content, boundary, "name");
  std::string surname = extract_form_data(content, boundary, "surname");
  std::string phone = extract_form_data(content, boundary, "phone");

  if (phoneBook.count(original_name) == 0)
  {
    const char *response = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\nContact not found";
    send(client_socket, response, strlen(response), 0);
    return;
  }

  Friend &f = phoneBook[original_name];
  f.name = name;
  f.surname = surname;
  f.phoneNumber = phone;

  if (content.find("filename=\"") != std::string::npos)
  {
    std::string file_part = content.substr(content.find("filename=\""));
    size_t filename_start = file_part.find("filename=\"") + 10;
    size_t filename_end = file_part.find("\"", filename_start);
    std::string filename = file_part.substr(filename_start, filename_end - filename_start);

    size_t file_content_start = file_part.find("\r\n\r\n") + 4;
    size_t file_content_end = file_part.rfind("\r\n--" + boundary);
    std::string file_content = file_part.substr(file_content_start, file_content_end - file_content_start);

    std::ofstream outfile("uploads/" + filename, std::ios::binary);
    if (outfile)
    {
      outfile.write(file_content.c_str(), file_content.size());
      outfile.close();
      f.fileName = "uploads/" + filename;
    }
  }

  if (original_name != name)
  {
    phoneBook[name] = f;
    phoneBook.erase(original_name);
  }

  std::string redirect = "HTTP/1.1 303 See Other\r\nLocation: /\r\n\r\n";
  send(client_socket, redirect.c_str(), redirect.size(), 0);
}

void handle_client(int client_socket)
{
  std::string request = read_full_request(client_socket);

  if (request.empty())
  {
    close(client_socket);
    return;
  }

  log_request(request);

  if (request.find("GET / ") != std::string::npos || request.find("GET /index.html") != std::string::npos)
  {
    std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
    response += "<!DOCTYPE html>";
    response += "<html><head>";
    response += "<style>";
    response += "body { font-family: Arial, sans-serif; margin: 20px; background-color: #f4f4f9; }";
    response += "h1 { color: #333; text-align: center; }";
    response += "form { background: #fff; padding: 20px; border-radius: 8px; box-shadow: 0 2px 4px rgba(0,0,0,0.1); max-width: 500px; margin: 20px auto; }";
    response += "input[type='text'], input[type='file'] { width: 100%; padding: 8px; margin: 8px 0; border: 1px solid #ddd; border-radius: 4px; }";
    response += "input[type='submit'] { background-color: #4CAF50; color: white; padding: 10px 15px; border: none; border-radius: 4px; cursor: pointer; }";
    response += "input[type='submit']:hover { background-color: #45a049; }";
    response += "table { width: 100%; border-collapse: collapse; margin-top: 20px; }";
    response += "th, td { border: 1px solid #ddd; padding: 8px; text-align: left; }";
    response += "th { background-color: #f2f2f2; }";
    response += "tr:nth-child(even) { background-color: #f9f9f9; }";
    response += "img { max-width: 100px; max-height: 100px; border-radius: 4px; }";
    response += "a { color: #4CAF50; text-decoration: none; }";
    response += "a:hover { text-decoration: underline; }";
    response += ".action-btn { background-color: #2196F3; color: white; padding: 5px 10px; border: none; border-radius: 3px; cursor: pointer; }";
    response += ".action-btn:hover { background-color: #0b7dda; }";
    response += "</style>";
    response += "</head><body>";
    response += "<h1>Phone Book</h1>";
    response += "<form action=\"/upload\" method=\"post\" enctype=\"multipart/form-data\">";
    response += "<h2>Add New Contact</h2>";
    response += "Name: <input type=\"text\" name=\"name\" required><br>";
    response += "Surname: <input type=\"text\" name=\"surname\" required><br>";
    response += "Phone: <input type=\"text\" name=\"phone\" required><br>";
    response += "Profile Image: <input type=\"file\" name=\"myfile\" accept=\"image/*\" required><br>";
    response += "<input type=\"submit\" value=\"Add Contact\">";
    response += "</form>";
    response += "<h2>Contacts</h2>";
    response += "<table>";
    response += "<tr><th>Name</th><th>Surname</th><th>Phone</th><th>Profile</th><th>Actions</th></tr>";

    for (const auto &entry : phoneBook)
    {
      const Friend &f = entry.second;
      response += "<tr>";
      response += "<td>" + f.name + "</td>";
      response += "<td>" + f.surname + "</td>";
      response += "<td>" + f.phoneNumber + "</td>";
      response += "<td><img src=\"" + f.fileName + "\" alt=\"ProfilePicture\"></td>";
      response += "<td>";
      response += "<form action=\"/edit\" method=\"post\" style=\"display:inline;\">";
      response += "<input type=\"hidden\" name=\"name\" value=\"" + f.name + "\">";
      response += "<input type=\"submit\" class=\"action-btn\" value=\"Edit\">";
      response += "</form>";
      response += "<form action=\"/delete\" method=\"post\" style=\"display:inline; margin-left:5px;\">";
      response += "<input type=\"hidden\" name=\"name\" value=\"" + f.name + "\">";
      response += "<input type=\"submit\" class=\"delete-btn\" value=\"Delete\" onclick=\"return confirm('Are you sure?')\">";
      response += "</form>";
      response += "</form>";
      response += "</td>";
      response += "</tr>";
    }

    response += "</table>";
    response += "</body></html>";

    send(client_socket, response.c_str(), response.size(), 0);
  }
  else if (request.find("POST /upload") != std::string::npos)
  {
    size_t boundary_start = request.find("boundary=");
    if (boundary_start == std::string::npos)
    {
      const char *response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\n\r\nInvalid content type";
      send(client_socket, response, strlen(response), 0);
      close(client_socket);
      return;
    }
    boundary_start += 9;
    size_t boundary_end = request.find("\r\n", boundary_start);
    std::string boundary = request.substr(boundary_start, boundary_end - boundary_start);

    size_t content_start = request.find("\r\n\r\n");
    if (content_start != std::string::npos)
    {
      handle_image_upload(client_socket, boundary, request.substr(content_start + 4));
    }
  }
  else if (request.find("POST /edit") != std::string::npos)
  {
    size_t content_start = request.find("\r\n\r\n");
    if (content_start != std::string::npos)
    {
      std::string content = request.substr(content_start + 4);
      std::string name = content.substr(content.find("name=") + 5);
      name = name.substr(0, name.find("\r\n"));
      handle_edit_request(client_socket, name);
    }
  }
  else if (request.find("POST /update") != std::string::npos)
  {
    size_t boundary_start = request.find("boundary=");
    if (boundary_start == std::string::npos)
    {
      const char *response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\n\r\nInvalid content type";
      send(client_socket, response, strlen(response), 0);
      close(client_socket);
      return;
    }
    boundary_start += 9;
    size_t boundary_end = request.find("\r\n", boundary_start);
    std::string boundary = request.substr(boundary_start, boundary_end - boundary_start);

    size_t content_start = request.find("\r\n\r\n");
    if (content_start != std::string::npos)
    {
      handle_update(client_socket, boundary, request.substr(content_start + 4));
    }
  }
  else if (request.find("POST /delete") != std::string::npos)
  {
    size_t content_start = request.find("\r\n\r\n");
    if (content_start != std::string::npos)
    {
      std::string content = request.substr(content_start + 4);
      std::string name = content.substr(content.find("name=") + 5);
      name = name.substr(0, name.find("\r\n"));
      handle_delete(client_socket, name);
    }
  }
  else if (request.find("GET /uploads/") != std::string::npos)
  {
    size_t start = request.find("GET /") + 5;
    size_t end = request.find(" ", start);
    std::string filepath = request.substr(start, end - start);

    std::ifstream file(filepath, std::ios::binary);
    if (file)
    {
      std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

      std::string content_type = "application/octet-stream";
      if (filepath.find(".jpg") != std::string::npos || filepath.find(".jpeg") != std::string::npos)
      {
        content_type = "image/jpeg";
      }
      else if (filepath.find(".png") != std::string::npos)
      {
        content_type = "image/png";
      }

      std::string response =
          "HTTP/1.1 200 OK\r\n"
          "Content-Type: " +
          content_type + "\r\n"
                         "Content-Length: " +
          std::to_string(content.size()) + "\r\n"
                                           "\r\n";

      send(client_socket, response.c_str(), response.size(), 0);
      send(client_socket, content.c_str(), content.size(), 0);
    }
    else
    {
      const char *response = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\nImage not found";
      send(client_socket, response, strlen(response), 0);
    }
  }
  else
  {
    const char *response = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\n404 Not Found";
    send(client_socket, response, strlen(response), 0);
  }

  close(client_socket);
}

int main()
{
  int server_fd, client_socket;
  struct sockaddr_in address;
  int opt = 1;
  int addrlen = sizeof(address);

  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
  {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }

  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
  {
    perror("setsockopt");
    exit(EXIT_FAILURE);
  }

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);

  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
  {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }

  if (listen(server_fd, 3) < 0)
  {
    perror("listen");
    exit(EXIT_FAILURE);
  }

  std::cout << "Server running on port " << PORT << std::endl;
  std::cout << "Access it at http://localhost:" << PORT << std::endl;

  system("mkdir -p uploads");

  while (true)
  {
    if ((client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
    {
      perror("accept");
      continue;
    }
    handle_client(client_socket);
  }

  return 0;
}