#!/usr/bin/env python3
import sys

# Read data from stdin (POST body)
body = sys.stdin.read()

# Write HTTP response to stdout
print("Content-Type: text/plain")
print()
print("Hello from CGI")
print("Received body:")
print(body)
