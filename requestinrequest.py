import socket, time

s = socket.socket()
s.connect(("127.0.0.1", 8080))

print("Sending headers...")
s.send(b"POST /test/ HTTP/1.1\r\nHost: localhost\r\nContent-Length: 1\r\n\r\n")

print("Sending body (longer than Content-Length)...")
s.send(b"hello")

time.sleep(1)

print("Sending second request without closing connection...")
s.send(b"GET / HTTP/1.1\r\nHost: localhost\r\n\r\n")

print("Receiving response...")
print(s.recv(4096).decode(errors="ignore"))

s.close()


# everything work good :)
