#ifndef _BASE_DAO_H_
#define _BASE_DAO_H_

#include <vector>

#include "../class/base.h"
#include "../db/db.h"
using std::vector;
typedef vector<User> Users;
typedef vector<Group> Groups;
typedef vector<Message> Messages;
Users selectUserByIDAndPassword(int id, string platform, string password) {  // 登录
    auto conn = SQL::getInstance();
    auto query = conn->prepareStatement(
        "SELECT id,nickname,birthdate,created_at,location,platform,status FROM users WHERE id = ? "
        "AND platform = ? AND password = ? AND status = 1;");
    query->setInt(1, id);
    query->setString(2, platform);
    query->setString(3, password);
    auto res = query->executeQuery();
    Users users;
    if (res->next()) {
        users.push_back(User(res->getInt("id"), res->getString("nickname"), "",
                             res->getString("birthdate"), res->getString("created_at"),
                             res->getString("location"), res->getString("platform"),
                             res->getBoolean("status")));
    }
    return users;
}
Users selectUserByNickName(string nickname, string platform) {  // 搜索好友，两个平台可以复用
    auto query = SQL::getInstance()->prepareStatement(
        "SELECT id,nickname,birthdate,created_at,location,platform,status FROM users WHERE "
        "nickname LIKE ? AND platform = ? AND status = 1;");
    nickname = "%" + nickname + "%";  // 模糊搜索
    query->setString(1, nickname);
    query->setString(2, platform);
    auto res = query->executeQuery();
    Users users;
    if (res->next()) {
        users.push_back(User(res->getInt("id"), res->getString("nickname"), "",
                             res->getString("birthdate"), res->getString("created_at"),
                             res->getString("location"), res->getString("platform"),
                             res->getBoolean("status")));
    }
    return users;
}
Users selectUserByCommonFriends(int id) {  // 从微信id选qq共同好友的微信id
    auto query = SQL::getInstance()->prepareStatement(
        "SELECT id,nickname,birthdate,created_at,location,platform,bind_id,status "
        "FROM users WHERE platform = 'WeChat' AND status = 1 AND "
        "bind_id IN ( "
        "SELECT friend_id FROM friends "
        "WHERE user_id in (SELECT bind_id FROM users WHERE id = ? AND platform=\"WeChat\" AND "
        "status = 1) AND status = \"active\""
        ");");
    query->setInt(1, id);
    auto res = query->executeQuery();
    Users users;
    while (res->next()) {
        users.push_back(User(res->getInt("id"), res->getString("nickname"), "",
                             res->getString("birthdate"), res->getString("created_at"),
                             res->getString("location"), res->getString("platform"),
                             res->getBoolean("status")));
    }
    return users;
}
Users selectFriends(int id, string platform) {  // 好友列表
    auto query = SQL::getInstance()->prepareStatement(
        "SELECT id,nickname,birthdate,created_at,location,platform,bind_id,status "
        "FROM users WHERE platform = ? AND status = 1 AND "
        "id IN ("
        "SELECT friend_id FROM friends "
        "WHERE user_id = ? AND status = \"active\""
        ")");
    query->setString(1, platform);
    query->setInt(2, id);
    auto res = query->executeQuery();
    Users users;
    while (res->next()) {
        users.push_back(User(res->getInt("id"), res->getString("nickname"), "",
                             res->getString("birthdate"), res->getString("created_at"),
                             res->getString("location"), res->getString("platform"),
                             res->getBoolean("status")));
    }
    return users;
}
Users selectGroupMembers(int group_id) {
    /*
    SELECT u.id,nickname,birthdate,created_at,location,platform,bind_id,status,gm.role
    FROM group_members gm
    JOIN users u ON gm.user_id = u.id
    WHERE gm.group_id = 201 AND u.status = 1;
    */
    auto query = SQL::getInstance()->prepareStatement(
        "SELECT u.id,nickname,birthdate,created_at,location,platform,bind_id,status,gm.role "
        "FROM group_members gm "
        "JOIN users u ON gm.user_id = u.id "
        "WHERE gm.group_id = ? AND u.status = 1;");
    query->setInt(1, group_id);
    auto res = query->executeQuery();
    Users users;
    while (res->next()) {
        users.push_back(User(res->getInt("id"), res->getString("nickname"), "",
                             res->getString("birthdate"), res->getString("created_at"),
                             res->getString("location"), res->getString("platform"),
                             res->getBoolean("status")));
    }
    return users;
}
Groups searchGroup(string group_name) {  // only QQ
    auto query = SQL::getInstance()->prepareStatement(
        "SELECT id,owner_id,group_name,platform FROM groups WHERE group_name LIKE ? AND platform = "
        "'QQ';");
    group_name = "%" + group_name + "%";  // 模糊搜索
    query->setString(1, group_name);
    auto res = query->executeQuery();
    Groups groups;
    while (res->next()) {
        groups.push_back(Group(res->getInt("id"), res->getInt("owner_id"),
                               res->getString("group_name"), res->getString("platform")));
    }
    return groups;
}
Groups searchGroupByJoined(int id) {
    auto query = SQL::getInstance()->prepareStatement(
        "SELECT id,group_name,platform,owner_id FROM groups WHERE "
        "id in ("
        "SELECT group_id FROM group_members "
        "WHERE user_id = ? AND role != 'pending'"
        ")");
    query->setInt(1, id);
    auto res = query->executeQuery();
    Groups groups;
    while (res->next()) {
        groups.push_back(Group(res->getInt("id"), res->getInt("owner_id"),
                               res->getString("group_name"), res->getString("platform")));
    }
    return groups;
}
Messages getPrivateMessageById(int sender_id, int friend_id) {
    // SELECT id,sender_id,receiver_id,content,sent_at,message_type FROM messages WHERE (sender_id =
    // 104 AND receiver_id = 359) OR (sender_id = 359 AND receiver_id = 104) AND message_type =
    // 'private' ORDER BY sent_at DESC
    auto query = SQL::getInstance()->prepareStatement(
        "SELECT id,sender_id,receiver_id,content,sent_at,message_type FROM messages "
        "WHERE (sender_id = ? AND receiver_id = ?) OR (sender_id = ? AND receiver_id = ?) "
        "AND message_type = 'private' ORDER BY sent_at DESC");
    query->setInt(1, sender_id);
    query->setInt(2, friend_id);
    query->setInt(3, friend_id);
    query->setInt(4, sender_id);
    auto res = query->executeQuery();
    Messages messages;
    while (res->next()) {
        messages.push_back(Message(res->getInt("id"), res->getInt("sender_id"),
                                   res->getInt("receiver_id"), res->getInt("group_id"),
                                   res->getString("content"), res->getString("sent_at"),
                                   res->getString("message_type")));
    }
    return messages;
}
Messages getGroupMessageById(int group_id) {
    auto query = SQL::getInstance()->prepareStatement(
        "SELECT id,sender_id,receiver_id,group_id,content,sent_at,message_type FROM messages "
        "WHERE group_id = ? AND message_type = 'group' ORDER BY sent_at DESC");
    query->setInt(1, group_id);
    auto res = query->executeQuery();
    Messages messages;
    while (res->next()) {
        messages.push_back(Message(res->getInt("id"), res->getInt("sender_id"),
                                   res->getInt("receiver_id"), res->getInt("group_id"),
                                   res->getString("content"), res->getString("sent_at"),
                                   res->getString("message_type")));
    }
    return messages;
}
User insertUser(User user) {
    auto query = SQL::getInstance()->prepareStatement(
        "INSERT INTO users(nickname,password,birthdate,created_at,location,platform,status) "
        "VALUES(?,?,?,?,?,?,1)");
    query->setString(1, user.getNickname());
    query->setString(2, user.getPassword());
    query->setString(3, user.getBirthdate());
    query->setString(4, user.getCreatedAt());
    query->setString(5, user.getLocation());
    query->setString(6, user.getPlatform());
    query->setBoolean(7, user.getStatus());
    query->executeUpdate();
    return user;
}
bool insertFriend(int user_id, int friend_id, string status) {
    auto query = SQL::getInstance()->prepareStatement(
        "INSERT INTO friends (user_id,friend_id,status) VALUES (?, ?, ?)");
    query->setInt(1, user_id);
    query->setInt(2, friend_id);
    query->setString(3, status);
    return query->executeUpdate();
}
Group createGroup(string name, string platform, int owner_id) {
    auto query = SQL::getInstance()->prepareStatement(
        "INSERT INTO groups (group_name,platform,owner_id) VALUES (?,?,?)");
    query->setString(1, name);
    query->setString(2, platform);
    query->setInt(3, owner_id);
    query->executeUpdate();

    auto res = SQL::getInstance()->prepareStatement(
        "SELECT id,owner_id,group_name,platform FROM groups WHERE group_name = ? AND platform = ? "
        "AND owner_id = ?");
    res->setString(1, name);
    res->setString(2, platform);
    res->setInt(3, owner_id);
    auto result = res->executeQuery();
    auto group = Group();
    if (result->next()) {
        group = Group(result->getInt("id"), result->getInt("owner_id"),
                      result->getString("group_name"), result->getString("platform"));
        auto q2 = SQL::getInstance()->prepareStatement(
            "INSERT INTO group_members (group_id,user_id,role) VALUES (?,?,?)");
        q2->setInt(1, group.getId());
        q2->setInt(2, owner_id);
        q2->setString(3, "owner");
    }
    return group;
}
bool insertGroupMember(int group_id, int user_id, string role) {
    auto query = SQL::getInstance()->prepareStatement(
        "INSERT INTO group_members (group_id,user_id,role) VALUES (?,?,?)");
    query->setInt(1, group_id);
    query->setInt(2, user_id);
    query->setString(3, role);
    return query->executeUpdate();
}
bool sendMessage(Message message) {
    auto query = SQL::getInstance()->prepareStatement(
        "INSERT INTO messages (sender_id,receiver_id,group_id,content,message_type) "
        "VALUES (?,?,?,?,?)");
    query->setInt(1, message.getSenderId());
    query->setInt(2, message.getReceiverId());
    query->setInt(3, message.getGroupId());
    query->setString(4, message.getContent());
    query->setString(5, message.getMessageType());
    return query->executeUpdate();
}
User updateUser(User user) {
    auto query = SQL::getInstance()->prepareStatement(
        "UPDATE users SET nickname = ?, birthdate = ?, location = ? WHERE id = ?");
    query->setString(1, user.getNickname());
    query->setString(2, user.getBirthdate());
    query->setString(3, user.getLocation());
    query->setInt(4, user.getId());
    query->executeUpdate();
    return user;
}
bool updateFriendStatus(int user_id, int friend_id, string status) {
    auto query = SQL::getInstance()->prepareStatement(
        "UPDATE friends SET status = ? WHERE user_id = ? AND friend_id = ?");
    query->setString(1, status);
    query->setInt(2, user_id);
    query->setInt(3, friend_id);
    return query->executeUpdate();
}
bool updateGroupMemberRole(int group_id, int user_id, string role) {
    auto query = SQL::getInstance()->prepareStatement(
        "UPDATE group_members SET role = ? WHERE group_id = ? AND user_id = ?");
    query->setString(1, role);
    query->setInt(2, group_id);
    query->setInt(3, user_id);
    return query->executeUpdate();
}
bool deleteFriend(int user_id, int friend_id) {
    auto query = SQL::getInstance()->prepareStatement(
        "DELETE FROM friends WHERE user_id = ? AND friend_id = ?");
    query->setInt(1, user_id);
    query->setInt(2, friend_id);
    query->executeUpdate();
    query = SQL::getInstance()->prepareStatement(
        "UPDATE friends SET status = 'blocked' WHERE user_id = ? AND friend_id = ?");
    query->setInt(1, friend_id);
    query->setInt(2, user_id);
    return query->executeUpdate();
}
bool deleteGroup(int group_id) {
    auto query = SQL::getInstance()->prepareStatement("DELETE FROM groups WHERE id = ?");
    query->setInt(1, group_id);
    query->executeUpdate();
    query = SQL::getInstance()->prepareStatement("DELETE FROM group_members WHERE group_id = ?");
    query->setInt(1, group_id);
    return query->executeUpdate();
}
bool deleteGroupMember(int group_id, int user_id) {
    auto query = SQL::getInstance()->prepareStatement(
        "DELETE FROM group_members WHERE group_id = ? AND user_id = ?");
    query->setInt(1, group_id);
    query->setInt(2, user_id);
    return query->executeUpdate();
}
bool deleteMessage(int id) {
    auto query = SQL::getInstance()->prepareStatement("DELETE FROM messages WHERE id = ?");
    query->setInt(1, id);
    return query->executeUpdate();
}
#endif