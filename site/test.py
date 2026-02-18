#!/usr/bin/env python3

import cgi
import cgitb

# Enable debugging (optional, useful during development)
cgitb.enable()

# Create FieldStorage instance to parse form data
form = cgi.FieldStorage()

# Get the values of 'name' and 'message' fields
name = form.getvalue('name', '')
message = form.getvalue('message', '')

# Output HTTP header
print("Content-Type: text/html\n")

# Output HTML
print("<html>")
print("<body>")
print(f"<h1>Name: {name}</h1>")
print(f"<h1>Message: {message}</h1>")
print("</body>")
print("</html>")
