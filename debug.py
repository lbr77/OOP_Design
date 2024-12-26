import httpx
import hashlib
import base64
import random
import string
import time
import json
t = httpx.HTTPTransport(retries=10)
s = httpx.Client(transport=t)
url = 'http://localhost:8080'

def do_qq_action(action, **kwargs):
    time.sleep(0.1)
    return json.dumps(s.post(f"{url}/qq/api",json={"action":action,**kwargs}).json(),indent=4)

def md5(s):
    return hashlib.md5(s.encode()).hexdigest()


username = "ak8Mki8q"
password = "yqsbgLKp"
print(f"username: {username}, password: {password}")
# print(do_qq_action("register",username=username,password=md5(password)))
print(do_qq_action("login",username=username,password=md5(password)))
print(do_qq_action("searchFriend",username="libr"))
print(do_qq_action("addFriend",friend_id=703))
print(do_qq_action("logout"))
print(do_qq_action("login",username="libr",password=md5("libr")))
print(do_qq_action("getMessage"))
print(do_qq_action("verifyFriend",friend_id=707))
print(do_qq_action("getInfo"))
print(do_qq_action("sendMessage",friend_id=707,message="hello"))
print(do_qq_action("deleteFriend",friend_id=707))
print(do_qq_action("logout"))

