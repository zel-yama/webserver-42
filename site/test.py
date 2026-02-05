
import sys
import os
from urllib.parse import parse_qs

length = int(os.environ.get("CONTENT_LENGTH", 0))

data = sys.stdin.read(length)

params = parse_qs(data)

name = params.get("name", [""])[0]
message = params.get("message", [""])[0]

print("Content-Type: text/plain")
print()
print("Name:", name)
print("Message:", message)
