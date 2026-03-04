#!/usr/bin/env python3
import os
import sys

SESSION_DIR = "/tmp/webserv_sessions"

def parse_cookies():
    cookies = {}
    raw = os.environ.get("HTTP_COOKIE", "")
    for pair in raw.split(";"):
        pair = pair.strip()
        if "=" in pair:
            k, v = pair.split("=", 1)
            cookies[k.strip()] = v.strip()
    return cookies

# Delete session file on logout
cookies = parse_cookies()
sid = cookies.get("WEBSERV_SID", "")
if sid:
    path = os.path.join(SESSION_DIR, sid + ".json")
    try:
        if os.path.exists(path):
            os.remove(path)
    except:
        pass

print("Status: 302 Found")
print("Content-Type: text/html")
print("Set-Cookie: WEBSERV_SID=deleted; Path=/; Max-Age=0; HttpOnly")
print("Location: ./login.html")
print()