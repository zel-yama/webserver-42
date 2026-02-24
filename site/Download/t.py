#!/usr/bin/env python3

import os
import sys

print("Content-Type: text/html; charset=utf-8\r")
print("\r")  # End of headers

print("<!DOCTYPE html>")
print("<html>")
print("<head><title>CGI Test</title></head>")
print("<body>")
print("<h1>✅ CGI is working!</h1>")

print("<h2>Environment</h2>")
print("<pre>")
for k, v in sorted(os.environ.items()):
    print(f"{k}={v}")
print("</pre>")

print("<h2>STDIN</h2>")
data = sys.stdin.read();
print("<pre>")
print(data if data else "(empty)")
print("</pre>")

print("</body>")
print("</html>")
