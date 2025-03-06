#!/usr/bin/env python3
import datetime

backend_file = "/usr/lib/cgi-bin/backend.txt"

with open(backend_file, 'r') as file:
    first_line = file.readline().strip()

offset, country, city = first_line.split(',')
offset = int(offset)

current_time = datetime.datetime.utcnow() + datetime.timedelta(hours=offset)
formatted_time = current_time.strftime("%Y-%m-%d %H:%M:%S")

print("Content-Type: text/html\n")
print(f"<!DOCTYPE html>")
print(f"<html lang='en'>")
print(f"<head><title>Current Time</title></head>")
print(f"<body>")
print(f"<h1>Current Time in {city}, {country}</h1>")
print(f"<p>{formatted_time}</p>")
print(f"<p><a href='/cgi-bin/switch_to_sa.cgi'>Switch to South African Time</a></p>")
print(f"<p><a href='/cgi-bin/switch_to_gh.cgi'>Switch to Ghana Time</a></p>")
print(f"</body>")
print(f"</html>")
