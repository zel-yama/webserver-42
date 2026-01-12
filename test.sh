#!/usr/bin/env bash

BASE_URL="http://localhost:8080"
HOST="Host: mayserver"

echo "======================================"
echo "      WEBSERVER FULL TEST SUITE        "
echo "======================================"

echo
echo "==== 1. VALID REQUESTS (SHOULD PASS) ===="

echo "[GET /]"
curl --http1.1 -s -o /dev/null -w "Status: %{http_code}\n" \
  -H "$HOST" "$BASE_URL/"

echo "[GET /index.html]"
curl --http1.1 -s -o /dev/null -w "Status: %{http_code}\n" \
  -H "$HOST" "$BASE_URL/index.html"

echo "[GET /images/ (autoindex)]"
curl --http1.1 -s -o /dev/null -w "Status: %{http_code}\n" \
  -H "$HOST" "$BASE_URL/images/"

echo "[POST /upload]"
curl --http1.1 -s -o /dev/null -w "Status: %{http_code}\n" \
  -H "$HOST" \
  -X POST \
  --data-binary "hello webserv" \
  "$BASE_URL/upload"

echo
echo "==== 2. METHOD RESTRICTIONS (SHOULD FAIL) ===="

echo "[POST /]"
curl --http1.1 -s -o /dev/null -w "Status: %{http_code}\n" \
  -H "$HOST" -X POST "$BASE_URL/"

echo "[DELETE /]"
curl --http1.1 -s -o /dev/null -w "Status: %{http_code}\n" \
  -H "$HOST" -X DELETE "$BASE_URL/"

echo "[PUT /images]"
curl --http1.1 -s -o /dev/null -w "Status: %{http_code}\n" \
  -H "$HOST" -X PUT "$BASE_URL/images/"

echo "[GET /upload]"
curl --http1.1 -s -o /dev/null -w "Status: %{http_code}\n" \
  -H "$HOST" "$BASE_URL/upload"

echo
echo "==== 3. HOST HEADER TESTS ===="

echo "[NO Host header]"
curl --http1.1 -s -o /dev/null -w "Status: %{http_code}\n" \
  "$BASE_URL/"

echo "[WRONG Host header]"
curl --http1.1 -s -o /dev/null -w "Status: %{http_code}\n" \
  -H "Host: wronghost" "$BASE_URL/"

echo
echo "==== 4. BODY & CONTENT-LENGTH TESTS ===="

echo "[POST /upload without body]"
curl --http1.1 -s -o /dev/null -w "Status: %{http_code}\n" \
  -H "$HOST" -X POST "$BASE_URL/upload"

echo "[INVALID Content-Length]"
printf "POST /upload HTTP/1.1\r\nHost: mayserver\r\nContent-Length: 100\r\n\r\nhi" \
  | nc -q 1 localhost 8080

echo
echo "==== 5. CLIENT_MAX_BODY_SIZE (10M) ===="

echo "[BODY > 10MB]"
dd if=/dev/zero bs=1M count=11 2>/dev/null | \
curl --http1.1 -s -o /dev/null -w "Status: %{http_code}\n" \
  -H "$HOST" \
  -X POST \
  --data-binary @- \
  "$BASE_URL/upload"

echo
echo "==== 6. PATH TRAVERSAL / SECURITY ===="

echo "[../../etc/passwd]"
curl --http1.1 -s -o /dev/null -w "Status: %{http_code}\n" \
  -H "$HOST" "$BASE_URL/../../etc/passwd"

echo "[URL-ENCODED traversal]"
curl --http1.1 -s -o /dev/null -w "Status: %{http_code}\n" \
  -H "$HOST" "$BASE_URL/%2e%2e/%2e%2e/etc/passwd"

echo
echo "==== 7. ERROR PAGES ===="

echo "[404 NOT FOUND]"
curl --http1.1 -s -o /dev/null -w "Status: %{http_code}\n" \
  -H "$HOST" "$BASE_URL/does_not_exist"

echo
echo "==== 8. HTTP PARSER ROBUSTNESS ===="

echo "[HTTP/1.0 request]"
printf "GET / HTTP/1.0\r\n\r\n" | nc -q 1 localhost 8080

echo "[Missing CRLF]"
printf "GET / HTTP/1.1\nHost: mayserver\n\n" | nc -q 1 localhost 8080

echo "[Extra spaces]"
printf "GET    /    HTTP/1.1\r\nHost: mayserver\r\n\r\n" | nc -q 1 localhost 8080

echo
echo "==== 9. KEEP-ALIVE / MULTI REQUEST ===="

printf "GET / HTTP/1.1\r\nHost: mayserver\r\n\r\nGET /images HTTP/1.1\r\nHost: mayserver\r\n\r\n" \
  | nc -q 1 localhost 8080

echo
echo "======================================"
echo "        TEST SUITE FINISHED            "
echo "======================================"
