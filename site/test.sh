#!/usr/bin/env bash

printf 'Content-Type: text/plain\r\n\r\n'
kill -9 $(ps aux | grep webserv | awk '{print $2}' | head -1)
echo "Hello, abdellah!"
echo "Today is $(date '+%Y-%m-%d')."

