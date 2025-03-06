#!/usr/bin/env python3
backend_file = "/usr/lib/cgi-bin/backend.txt"

with open(backend_file, 'w') as file:
    file.write("2,South Africa,Pretoria")

print("Content-Type: text/html\n")
print(f"<!DOCTYPE html>")
print(f"<html lang='en'>")
print(f"<head><title>Switched to South Africa Time</title></head>")
print(f"<body>")
print(f"<h1>Switched to South Africa Time</h1>")
print(f"<p><a href='/cgi-bin/get_time.cgi'>Get Current Time</a></p>")
print(f"</body>")
print(f"</html>")