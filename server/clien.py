import socket

target_host = "0.0.0.0"
target_port = 8080

cleint = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

cleint.connect((target_host, target_port))

cleint.send("GET / http/1.0 \n\rhost: 0.0.0.0 ")

response = cleint.resv(989)

print(response)