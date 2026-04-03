#!/usr/bin/env python3
import os
import sys
import json

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

def load_session(sid):
    try:
        path = os.path.join(SESSION_DIR, sid + ".json")
        if os.path.exists(path):
            with open(path, "r") as f:
                return json.load(f)
    except:
        pass
    return None

cookies      = parse_cookies()
session_id   = cookies.get("WEBSERV_SID", None)
session_data = load_session(session_id) if session_id else None

# ── No cookie OR no session file → redirect immediately BEFORE printing headers
if not session_id or not session_data:
    print("Status: 302 Found")
    print("Content-Type: text/html")
    print("Location: /login.html")
    print()
    sys.exit(0)

# ── Logged in — now safe to print 200 ────────────────────────────────────────
username   = session_data.get("username", "unknown")
login_time = session_data.get("login_time", "—")

cookie_raw = os.environ.get("HTTP_COOKIE", "—")
method     = os.environ.get("REQUEST_METHOD", "—")
protocol   = os.environ.get("SERVER_PROTOCOL", "—")
remote     = os.environ.get("REMOTE_ADDR", "—")
script     = os.environ.get("SCRIPT_NAME", "—")
query      = os.environ.get("QUERY_STRING", "") or "—"

print("Status: 200 OK")
print("Content-Type: text/html")
print()
print(f"""<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Webserv — Dashboard</title>
    <style>
        * {{ margin: 0; padding: 0; box-sizing: border-box; }}
        body {{
            min-height: 100vh;
            background: linear-gradient(135deg, #0f0c29, #302b63, #24243e);
            font-family: 'Segoe UI', sans-serif;
            color: #fff;
            padding: 40px 20px;
        }}
        .container {{ max-width: 700px; margin: 0 auto; }}
        .header {{
            display: flex;
            justify-content: space-between;
            align-items: center;
            margin-bottom: 40px;
        }}
        .header h1 {{ font-size: 24px; font-weight: 700; letter-spacing: 2px; }}
        .header a {{
            background: rgba(255,255,255,0.08);
            color: rgba(255,255,255,0.6);
            text-decoration: none;
            padding: 8px 18px;
            border-radius: 8px;
            font-size: 13px;
            border: 1px solid rgba(255,255,255,0.1);
        }}
        .header a:hover {{
            background: rgba(239,68,68,0.2);
            color: #f87171;
            border-color: #f87171;
        }}
        .welcome-card {{
            background: rgba(255,255,255,0.05);
            border: 1px solid rgba(255,255,255,0.1);
            border-radius: 20px;
            padding: 32px;
            margin-bottom: 24px;
        }}
        .welcome-card h2 {{ font-size: 22px; margin-bottom: 8px; color: #a78bfa; }}
        .welcome-card p {{ color: rgba(255,255,255,0.5); font-size: 14px; line-height: 1.6; }}
        .username-highlight {{ color: #60a5fa; font-weight: 700; }}
        .card {{
            background: rgba(255,255,255,0.03);
            border: 1px solid rgba(255,255,255,0.08);
            border-radius: 16px;
            padding: 24px 32px;
            margin-bottom: 24px;
        }}
        .card h3 {{
            font-size: 12px;
            font-weight: 700;
            color: rgba(255,255,255,0.4);
            text-transform: uppercase;
            letter-spacing: 1.5px;
            margin-bottom: 16px;
        }}
        .row {{
            display: flex;
            justify-content: space-between;
            align-items: center;
            padding: 10px 0;
            border-bottom: 1px solid rgba(255,255,255,0.05);
            font-size: 13px;
        }}
        .row:last-child {{ border-bottom: none; }}
        .row .k {{ color: rgba(255,255,255,0.4); }}
        .v {{
            font-family: monospace;
            font-size: 13px;
            color: #60a5fa;
            background: rgba(96,165,250,0.08);
            padding: 3px 10px;
            border-radius: 6px;
            max-width: 420px;
            overflow: hidden;
            text-overflow: ellipsis;
            white-space: nowrap;
        }}
        .v.green  {{ color: #4ade80; background: rgba(74,222,128,0.08); }}
        .v.purple {{ color: #a78bfa; background: rgba(167,139,250,0.08); }}
        .v.yellow {{ color: #fbbf24; background: rgba(251,191,36,0.08); }}
    </style>
</head>
<body>
    <div class="container">
        <div class="header">
            <h1>WEBSERV</h1>
            <a href="./logout.py">Logout →</a>
        </div>

        <div class="welcome-card">
            <h2>Welcome, <span class="username-highlight">{username}</span> 👋</h2>
            <p>You are authenticated. Your session is active and being tracked via an HTTP cookie.
            This page is served by a CGI Python script running on your custom C++ HTTP server.</p>
        </div>

        <div class="card">
            <h3>🍪 Session Info</h3>
            <div class="row">
                <span class="k">Username</span>
                <span class="v yellow">{username}</span>
            </div>
            <div class="row">
                <span class="k">Session ID (WEBSERV_SID)</span>
                <span class="v purple">{session_id}</span>
            </div>
            <div class="row">
                <span class="k">Status</span>
                <span class="v green">Active ✓</span>
            </div>
            <div class="row">
                <span class="k">Login Time</span>
                <span class="v">{login_time}</span>
            </div>
            <div class="row">
                <span class="k">Cookie Header</span>
                <span class="v">{cookie_raw}</span>
            </div>
        </div>

        <div class="card">
            <h3>⚙️ CGI Environment</h3>
            <div class="row"><span class="k">REQUEST_METHOD</span><span class="v">{method}</span></div>
            <div class="row"><span class="k">SERVER_PROTOCOL</span><span class="v">{protocol}</span></div>
            <div class="row"><span class="k">REMOTE_ADDR</span><span class="v">{remote}</span></div>
            <div class="row"><span class="k">SCRIPT_NAME</span><span class="v">{script}</span></div>
            <div class="row"><span class="k">QUERY_STRING</span><span class="v">{query}</span></div>
        </div>
    </div>
</body>
</html>
""")