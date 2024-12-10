import requests as r
import hashlib
s = r.Session()

t = s.post('http://localhost:8080/api/qq/login', json={"username": "admin","password": hashlib.md5("admin".encode()).hexdigest()})
print(t.content)

r = s.get('http://localhost:8080/api/qq/info')
print(r.content)
# t = s.get('http://localhost:8080/api/qq/logout')
# print(t.headers)
# t = s.post('http://localhost:8080/api/qq/register', json={"username": "12333","password": hashlib.md5("114514".encode()).hexdigest()})
# print(t.content)