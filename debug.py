import requests as r
import hashlib
import base64
import time
s = r.Session()
url = 'http://localhost:8080'

def do_action(action, **kwargs):
    time.sleep(0.01)
    return s.post(f"{url}/qq/api",json={"action": action, **kwargs}).json()
# print(do_action("login", username="Patrick Mendoza", password=hashlib.md5(b"123456").hexdigest()))
# print(do_action("searchFriend",username="bossbaby"))
# print(do_action("searchGroup",group_name="group1"))
# print(do_action("addFriend",friend_id=703))
# print(do_action("logout"))
# print(do_action("login", username="libr", password=hashlib.md5(b"libr").hexdigest()))
# print(do_action("verifyFriend",friend_id=1))

# print(do_action("deleteFriend",friend_id=701))

# print(do_action("createGroup",group_name="group1"))
# print(do_action("addGroup",group_id=303))

# print(do_action("verifyGroup",group_id=303,user_id=701))

# print(do_action("kickGroupmember",group_id=303,user_id=701))

# print(do_action("deleteGroup",group_id=303))
# print(do_action("getInfo"))
# print(do_action("sendMessage",friend_id=701,message="hello"))
# print(do_action("sendGroupMessage",group_id=304,message="hello"))
# print(do_action("markRead",message_id=39))
# print(do_action("getMessage"))
# import json
# print(json.dumps(do_action("getHistoryMessage",friend_id=701),indent=4))