#!/usr/bin/env python3
import os
import sys
from urllib.parse import parse_qs

# Read from stdin
content_length = os.environ.get('CONTENT_LENGTH', '0')
try:
    content_length = int(content_length)
except ValueError:
    content_length = 0

data = sys.stdin.read(content_length) if content_length > 0 else ""

# Parse form data
params = parse_qs(data)
name = params.get('name', [''])[0]
message = params.get('message', [''])[0]

# Print response
print("Content-Type: text/html\n")
print("<html>")
print("<body>")
print(f"<h1>Name: {name}</h1>")
print(f"<h1>Message: {message}</h1>")
print("</body>")
print("</html>")