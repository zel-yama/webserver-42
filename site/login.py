#!/usr/bin/env python3
import os
import sys
import urllib.parse
import json
import time

SESSION_DIR = "/tmp/webserv_sessions"

def parse_body():
    data = {}
    try:
        length = int(os.environ.get("CONTENT_LENGTH", 0))
        if length > 0:
            body = sys.stdin.read(length)
            for pair in body.split("&"):
                if "=" in pair:
                    k, v = pair.split("=", 1)
                    data[urllib.parse.unquote_plus(k)] = urllib.parse.unquote_plus(v)
    except:
        pass
    return data

def parse_cookies():
    cookies = {}
    raw = os.environ.get("HTTP_COOKIE", "")
    for pair in raw.split(";"):
        pair = pair.strip()
        if "=" in pair:
            k, v = pair.split("=", 1)
            cookies[k.strip()] = v.strip()
    return cookies

def save_session(sid, data):
    try:
        if not os.path.exists(SESSION_DIR):
            os.makedirs(SESSION_DIR)
        path = os.path.join(SESSION_DIR, sid + ".json")
        with open(path, "w") as f:
            json.dump(data, f)
    except:
        pass

form    = parse_body()
cookies = parse_cookies()
username = form.get("username", "").strip()

if not username:
    print("Status: 400 Bad Request")
    print("Content-Type: text/html")
    print()
    print("""<!DOCTYPE html><html><body style="background:#0f0c29;color:#fff;font-family:sans-serif;
          display:flex;align-items:center;justify-content:center;height:100vh;margin:0">
          <div style="text-align:center">
            <h2 style="color:#f87171">Missing username</h2>
            <a href="/login.html" style="color:#a78bfa">Back to login</a>
          </div></body></html>""")
    sys.exit(0)

# Save username into session file keyed by WEBSERV_SID (set by C++ server)
sid = cookies.get("WEBSERV_SID", "")
if sid:
    save_session(sid, {
        "username": username,
        "logged_in": True,
        "login_time": time.strftime("%Y-%m-%d %H:%M:%S")
    })

print("Status: 302 Found")
print("Content-Type: text/html")
print("Location: ./dashboard.py")
print()