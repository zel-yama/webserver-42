import os
import sys
from urllib.parse import parse_qs

print("Content-Type: text/plain\r\n")

content_length = int(os.environ.get("CONTENT_LENGTH", "0") or 0)
body = sys.stdin.read(content_length) if content_length > 0 else ""
params = parse_qs(body)

print("Received POST data:")
for key, values in params.items():
	for value in values:
		print(f"{key}={value}")