-- select 语句
-- select user by nickname and password (登录)(qq/wechat)
SELECT id,nickname,birthdate,created_at,location,platform,bind_id,status
FROM users
WHERE nickname = ? AND password = ? AND platform = ? AND status = 1;
-- search user by nickname (搜索好友)(qq/wechat)
SELECT id,nickname,birthdate,created_at,location,platform,bind_id,status
FROM users
WHERE nickname LIKE ? AND platform = ? AND status = 1
-- search user by common friends in wechat (推荐qq共同好友)
SELECT id,nickname,birthdate,created_at,location,platform,bind_id,status
FROM users
WHERE bind_id -- bind id in qq -> wechat user.
IN (
    SELECT friend_id -- qq friends
    FROM friends
    WHERE user_id IN (
        SELECT bind_id -- bind qq id
        FROM users
        WHERE id = ? AND platform = 'WeChat' AND status = 1
        -- wechat user id
    ) AND status = 'active'
)
AND platform = 'WeChat' AND status = 1;
-- select friends by user id (好友列表)(qq/wechat)
SELECT id,nickname,birthdate,created_at,location,platform,bind_id,status
FROM users
WHERE id IN (
    SELECT friend_id
    FROM friends
    WHERE user_id = 1
		AND status = 'active'
)
AND status = 1
AND platform = 'QQ';
-- search group by name (搜索群)(qq)
SELECT id,group_name,platform,owner_id,created_at
FROM groups
WHERE group_name LIKE ? AND platform = 'QQ';
-- select private messages (私聊消息)
SELECT id,sender_id,receiver_id,content,sent_at
FROM messages
WHERE (sender_id = ? AND receiver_id = ?)  
OR (sender_id = ? AND receiver_id = ?) -- 发送者和接收者互换
AND message_type = 'private'
ORDER BY sent_at DESC
-- select group messages (群聊消息)
SELECT id,sender_id,group_id,content,sent_at
FROM messages
WHERE group_id = ?
AND message_type = 'group'
ORDER BY sent_at DESC
-- select group members (群成员)
SELECT user_id,role
FROM group_members
WHERE group_id = ? 
AND role != 'pending';

-- insert 语句
-- insert user (注册)(qq/wechat)
INSERT INTO users (nickname,password,platform)
VALUES (?,?,?);
-- insert friend (申请添加好友)(qq/wechat)
INSERT INTO friends (user_id,friend_id)
VALUES (?,?);
-- insert group (创建群)(qq)
INSERT INTO groups (group_name,platform,owner_id)
VALUES (?,?,?);
INSERT INTO group_members (group_id,user_id,role)
VALUES (?,?,"owner");
-- insert group member (加入群)(qq/wechat)
INSERT INTO group_members (group_id,user_id)
VALUES (?,?);
-- insert message (发送私聊消息)
INSERT INTO messages (sender_id,receiver_id,content,message_type)
VALUES (?,?,?,'private');
-- insert message (发送群聊消息)
INSERT INTO messages (sender_id,group_id,content,message_type)
VALUES (?,?,?,'group');


-- update 语句
-- update user (修改用户信息)(qq/wechat)
UPDATE users
SET nickname = ?,birthdate = ?,location = ?
WHERE id = ? AND platform = ?;
-- update friend (同意好友申请)(qq/wechat)
UPDATE friends
SET status = 'active'
WHERE user_id = ? AND friend_id = ?;
INSERT INTO friends (user_id,friend_id)
VALUES (?,?);
-- block friend (拉黑好友)(qq/wechat)
UPDATE friends
SET status = 'blocked'
WHERE user_id = ? AND friend_id = ?;
-- update group member (修改群成员角色)(qq)
UPDATE group_members
SET role = ?
WHERE group_id = ? AND user_id = ?;
-- 注销用户
UPDATE users
SET status = 0
WHERE id = ?;

-- delete 语句
-- delete friend (删除好友)(qq/wechat)
DELETE FROM friends
WHERE user_id = ? AND friend_id = ?;
-- delete group (解散群)(qq)
DELETE FROM groups
WHERE id = ?;
DELETE FROM group_members
WHERE group_id = ?;
-- delete group member (退出群)(qq/wechat)
DELETE FROM group_members
WHERE group_id = ? AND user_id = ?;
-- delete message (删除消息)
DELETE FROM messages
WHERE id = ?;
