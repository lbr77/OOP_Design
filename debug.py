import socket

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect(('localhost', 8080))
s.send("""GET /1.jpg HTTP/1.1
Host: localhost:8081
User-Agent: curl/7.74.0
Accept: */*

""".replace('\n', '\r\n').encode())

recv = bytearray()
while True:
    data = s.recv(1024)
    if not data:
        break
    recv += data

print(len(recv))
print(recv[:100])