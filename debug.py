import requests as r
import hashlib
import base64
s = r.Session()
url = 'http://localhost:8080'

# print(s.post(f"{url}/qq/api/user/login",json={"user_id": 701,"password": hashlib.md5(b"bossbaby").hexdigest()}).json())
# print(s.post(f"{url}/qq/api/user/register",json={"nickname": "orz","password": hashlib.md5(b"bossbaby").hexdigest()}).json())
print(s.get(f"{url}/qq/api/user/search?nickname=bossbaby").json())