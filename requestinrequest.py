import socket, time

s = socket.socket()
s.connect(("127.0.0.1", 8092))

# Send headers
s.send(b"POST /test HTTP/1.1\r\nHost: localhost\r\nContent-Length: 20\r\n\r\n")

# Send part of body
s.send(b"hello")
time.sleep(1)

# Send extra bytes (looks like next request)
s.send(b"GET / HTTP/1.1\r\nHost: localhost\r\n")