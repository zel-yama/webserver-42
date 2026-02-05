#!/usr/bin/env python3

print("Content-Type: text/plain\n")
print("This script will timeout in 5 seconds...")

import time
while True:
    print("Running...", flush=True)
    time.sleep(1)
