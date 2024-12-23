import requests as r
import hashlib
import base64
# import uuid
s = r.Session()
# test GET register
# print(s.get('http://localhost:8080/api/qq/user/register').json())
# test POST register
# print(s.post('http://localhost:8080/api/qq/user/register', json={'username': "cat", "password": hashlib.md5(b"cat").hexdigest()}).json())

# test POST login
print(s.post('http://localhost:8080/api/qq/user/login', json={'username': "cat", "password": hashlib.md5(b"cat").hexdigest()}).json())
print(s.cookies)
# test GET user_info
# print(s.get('http://localhost:8080/api/qq/users/1').json())
# print(s.get('http://localhost:8080/api/qq/users/').json())

# test POST chats
# print(s.post("http://localhost:8080/api/qq/chats",json={"type": "group","title": "114514","description": "","creator_id": 1}).json())
# test add chat member
# print(s.get("http://localhost:8080/api/qq/chats").headers)
# print(s.get("http://localhost:8080/api/qq/chats").json())
# print(s.post("http://localhost:8080/api/qq/chats/members",json={"user_id": 4,"chat_id": 1}).json())

# print(s.delete("http://localhost:8080/api/qq/chats/members",json={"chat_id": 1,"user_id": 2}).json())

# print(s.put("http://localhost:8080/api/qq/chats",json={"title": "1919810","description": "homo之家","id": 1}).json())

# print(s.get("http://localhost:8080/api/qq/chats/members?chat_id=1").json())

# test send Messages

# print(s.post("http://localhost:8080/api/qq/messages",json={"chat_id": 1,"content": "fuck"}))

# print(s.get("http://localhost:8080/api/qq/messages?chat_id=1").json())

# print(s.post("http://localhost:8080/api/qq/messages/status",json={"message_id": 5}).json())