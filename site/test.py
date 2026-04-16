#!/usr/bin/env python3

import cgi
import cgitb

cgitb.enable()

form = cgi.FieldStorage()

name = form.getvalue('name', '')
message = form.getvalue('message', '')

print("Content-Type: text/html\n")

print("<html>")
print("<body>")
print(f"<h1>Name: {name}</h1>")
print(f"<h1>Message: {message}</h1>")
print("</body>")
print("</html>")
