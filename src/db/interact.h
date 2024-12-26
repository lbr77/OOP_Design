#ifndef _DB_INTERACT_H_
#define _DB_INTERACT_H_
#include <string>

#include "../utils/log.h"
#include "db.h"
using std::string;

auto selectUserById(int user_id, string platform) {
    auto query =
        SQL::getInstance()->prepareStatement("SELECT * FROM users WHERE id = ? AND platform = ?");
    query->setInt(1, user_id);
    query->setString(2, platform);
    auto res = query->executeQuery();
    return res;
}
bool updateBind(int user_id,int bind_wx_id){
    auto stmt = SQL::getInstance()->prepareStatement(
        "UPDATE users SET bind_id = ? WHERE id = ?");
    stmt->setInt(1, bind_wx_id);
    stmt->setInt(2, user_id);
    if (stmt->executeUpdate()) {
        stmt->close();
        return true;
    }
    return false;
}
bool updateUser(int user_id,string nickname, string birthdate, string location) {
    auto stmt = SQL::getInstance()->prepareStatement(
        "UPDATE users SET nickname = ?, birthdate = ?, location = ? WHERE id = ?");
    stmt->setString(1, nickname);
    stmt->setString(2, birthdate);
    stmt->setString(3, location);
    stmt->setInt(4, user_id);
    if (stmt->executeUpdate()) {
        stmt->close();
        return true;
    }
    return false;
}
auto selectUserByNamePassword(string username, string password, string platform) {
    auto query = SQL::getInstance()->prepareStatement(
        "SELECT * FROM users WHERE nickname = ? AND password = ? AND platform = ?");
    query->setString(1, username);
    query->setString(2, password);
    query->setString(3, platform);
    auto res = query->executeQuery();
    return res;
}
bool insertUser(string username, string password, string platform) {
    auto stmt = SQL::getInstance()->prepareStatement(
        "INSERT INTO users (nickname,password,platform) VALUES (?,?,?)");
    stmt->setString(1, username);
    stmt->setString(2, password);
    stmt->setString(3, platform);
    if (stmt->executeUpdate()) {
        stmt->close();
        return true;
    }
    return false;
}
bool insertFriend(int user_id, int friend_id,
                  string status = "pending") {  // default status is pending
    auto stmt = SQL::getInstance()->prepareStatement(
        "INSERT INTO friends (user_id,friend_id,status) VALUES (?,?,?)");
    stmt->setInt(1, user_id);
    stmt->setInt(2, friend_id);
    stmt->setString(3, status);
    if (stmt->executeUpdate()) {
        stmt->close();
        return true;
    }
    return false;
}
bool updateFriend(int user_id, int friend_id) {
    auto stmt = SQL::getInstance()->prepareStatement(
        "UPDATE friends SET status = ? WHERE user_id = ? AND friend_id = ?");
    stmt->setString(1, "active");
    stmt->setInt(2, user_id);
    stmt->setInt(3, friend_id);
    if (stmt->executeUpdate()) {
        stmt->close();
        return true;
    }
    return false;
}
auto selectFriend(int user_id, int friend_id) {
    auto query = SQL::getInstance()->prepareStatement(
        "SELECT * FROM friends WHERE user_id = ? AND friend_id = ?");
    query->setInt(1, user_id);
    query->setInt(2, friend_id);
    auto res = query->executeQuery();
    return res;
}
auto selectFriends(int user_id, string platform = "QQ") {
    // LOG_DEBUG("Query", user_id, platform);
    auto query = SQL::getInstance()->prepareStatement(
        "SELECT u.id, u.nickname, u.birthdate, u.created_at, u.location, u.platform, u.bind_id "
        "FROM users u "
        "INNER JOIN friends f ON u.id = f.friend_id "
        "WHERE f.user_id = ? "
        "AND f.status = ? "
        "AND u.status = 1 "
        "AND u.platform = ?");
    query->setInt(1, user_id);
    query->setString(2, "active");
    query->setString(3, platform);
    auto res = query->executeQuery();
    // cout << res->rowsCount() << endl;
    // LOG_DEBUG(res->rowsCount());
    return res;
}
auto deleteFriend(int user_id, int friend_id) {
    auto stmt = SQL::getInstance()->prepareStatement(
        "DELETE FROM friends WHERE user_id = ? AND friend_id = ?");
    stmt->setInt(1, user_id);
    stmt->setInt(2, friend_id);
    return stmt->executeUpdate();
}
bool createGroup(int user_id, string group_name, string platform) {
    auto stmt = SQL::getInstance()->prepareStatement(
        "INSERT INTO groups (owner_id,group_name,platform) VALUES (?,?,?)");
    stmt->setInt(1, user_id);
    stmt->setString(2, group_name);
    stmt->setString(3, platform);
    if (stmt->executeUpdate()) {
        stmt->close();
        return true;
    }
    return false;
}
auto selectGroupByCreator(int user_id, string group_name) {
    auto query = SQL::getInstance()->prepareStatement(
        "SELECT * FROM groups WHERE owner_id = ? AND group_name = ? ORDER BY id DESC");  // get the
                                                                                         // latest
                                                                                         // one
    query->setInt(1, user_id);
    query->setString(2, group_name);
    auto res = query->executeQuery();
    return res;
}
auto selectGroupById(int group_id) {
    auto query = SQL::getInstance()->prepareStatement("SELECT * FROM groups WHERE id = ?");
    query->setInt(1, group_id);
    auto res = query->executeQuery();
    return res;
}
auto selectGroupByUserId(int user_id) {
    auto query = SQL::getInstance()->prepareStatement(
        "SELECT g.id, g.group_name FROM groups g INNER JOIN group_members gm ON g.id = "
        "gm.group_id WHERE gm.user_id = ? AND gm.role != 'pending'");
    query->setInt(1, user_id);
    auto res = query->executeQuery();
    return res;
}
auto selectGroupMemberByGroupId(int group_id) {
    auto query = SQL::getInstance()->prepareStatement(
        "SELECT u.id, u.nickname, gm.role FROM users u INNER JOIN group_members gm ON u.id = "
        "gm.user_id WHERE gm.group_id = ?");
    query->setInt(1, group_id);
    auto res = query->executeQuery();
    return res;
}
bool insertGroupMember(int group_id, int user_id, string role = "pending") {
    LOG_DEBUG(group_id, user_id, role);
    auto stmt = SQL::getInstance()->prepareStatement(
        "INSERT INTO group_members (group_id,user_id,role) VALUES (?,?,?)");
    stmt->setInt(1, group_id);
    stmt->setInt(2, user_id);
    stmt->setString(3, role);
    if (stmt->executeUpdate()) {
        stmt->close();
        return true;
    }
    return false;
}
auto selectGroupMember(int group_id, int user_id) {
    auto query = SQL::getInstance()->prepareStatement(
        "SELECT * FROM group_members WHERE group_id = ? AND user_id = ?");
    query->setInt(1, group_id);
    query->setInt(2, user_id);
    auto res = query->executeQuery();
    return res;
}
bool updateGroupMember(int group_id, int user_id, string role) {
    auto stmt = SQL::getInstance()->prepareStatement(
        "UPDATE group_members SET role = ? WHERE group_id = ? AND user_id = ?");
    stmt->setString(1, role);
    stmt->setInt(2, group_id);
    stmt->setInt(3, user_id);
    if (stmt->executeUpdate()) {
        stmt->close();
        return true;
    }
    return false;
}
bool deleteGroupById(int group_id) {
    auto stmt =
        SQL::getInstance()->prepareStatement("DELETE FROM group_members WHERE group_id = ?");
    stmt->setInt(1, group_id);
    if (stmt->executeUpdate()) {
        stmt->close();
        auto stmt2 = SQL::getInstance()->prepareStatement("DELETE FROM groups WHERE id = ?");
        stmt2->setInt(1, group_id);
        if (stmt2->executeUpdate()) {
            stmt2->close();
            return true;
        }
    }
    return false;
}
bool deleteGroupMember(int group_id, int user_id) {
    auto stmt = SQL::getInstance()->prepareStatement(
        "DELETE FROM group_members WHERE group_id = ? AND user_id = ?");
    stmt->setInt(1, group_id);
    stmt->setInt(2, user_id);
    if (stmt->executeUpdate()) {
        stmt->close();
        return true;
    }
    return false;
}
auto searchUser(string username, string platform) {
    auto query = SQL::getInstance()->prepareStatement(
        "SELECT * FROM users WHERE nickname like ? AND platform = ?");
    query->setString(1, "%" + username + "%");  // fuzzy search
    query->setString(2, platform);
    auto res = query->executeQuery();
    return res;
}
auto searchGroup(string group_name, string platform) {
    auto query = SQL::getInstance()->prepareStatement(
        "SELECT * FROM groups WHERE group_name like ? AND platform = ?");
    query->setString(1, "%" + group_name + "%");  // fuzzy search
    query->setString(2, platform);
    auto res = query->executeQuery();
    return res;
}
bool insertPrivateMessage(int sender_id, int receiver_id, string content) {
    auto stmt = SQL::getInstance()->prepareStatement(
        "INSERT INTO messages (sender_id,receiver_id,content,message_type) VALUES "
        "(?,?,?,?)");
    stmt->setInt(1, sender_id);
    stmt->setInt(2, receiver_id);
    stmt->setString(3, content);
    stmt->setString(4, "private");
    if (stmt->executeUpdate()) {
        stmt->close();
        return true;
    }
    return false;
}
bool insertGroupMessage(int sender_id, int group_id, string content) {
    auto stmt = SQL::getInstance()->prepareStatement(
        "INSERT INTO messages (sender_id,group_id,content,message_type) VALUES (?,?,?,?)");
    stmt->setInt(1, sender_id);
    stmt->setInt(2, group_id);
    stmt->setString(3, content);
    stmt->setString(4, "group");
    if (stmt->executeUpdate()) {
        stmt->close();
        return true;
    }
    return false;
}
bool updateMessageRead(int message_id, int user_id) {
    auto stmt = SQL::getInstance()->prepareStatement(
        "INSERT INTO is_read (message_id,user_id) VALUES (?,?)");
    stmt->setInt(1, message_id);
    stmt->setInt(2, user_id);
    if (stmt->executeUpdate()) {
        stmt->close();
        return true;
    }
    return false;
}
auto selectUnreadPrivateMessage(int user_id) {
    auto stmt = SQL::getInstance()->prepareStatement(
        "SELECT m.* "
        "FROM messages m "
        "WHERE (m.receiver_id = ?) AND NOT EXISTS ("
        "	SELECT id FROM is_read ir "
        "	WHERE ir.message_id = m.id "
        "	AND ir.user_id = m.receiver_id"
        ")");
    stmt->setInt(1, user_id);
    auto res = stmt->executeQuery();
    return res;
}
auto selectLatestMessage(int user_id) {
    auto stmt = SQL::getInstance()->prepareStatement(
        "SELECT  "
        "    m.*, "
        "    CASE  "
        "        WHEN m.message_type = 'group' THEN g.group_name "
        "        ELSE ( "
        "            SELECT u.nickname  "
        "            FROM users u  "
        "            WHERE u.id = CASE  "
        "                WHEN m.sender_id = ? THEN m.receiver_id  "
        "                ELSE m.sender_id  "
        "            END "
        "        ) "
        "    END AS name, "
        "		CASE  "
        "        WHEN m.message_type = 'group' THEN ( "
        "            SELECT gm.role  "
        "            FROM group_members gm  "
        "            WHERE gm.group_id = m.group_id  "
        "              AND gm.user_id = ? "
        "        ) "
        "        ELSE NULL "
        "    END AS user_role "
        "FROM messages m "
        "LEFT JOIN groups g ON m.group_id = g.id "
        "WHERE m.id IN ( "
        "    SELECT sub.id "
        "    FROM ( "
        "        SELECT  "
        "            MAX(m_inner.id) AS id "
        "        FROM messages m_inner "
        "        WHERE  "
        "            m_inner.sender_id = ?  "
        "            OR m_inner.receiver_id = ? "
        "            OR m_inner.group_id IN ( "
        "                SELECT gm.group_id  "
        "                FROM group_members gm  "
        "                WHERE gm.user_id = ? "
        "            ) "
        "        GROUP BY  "
        "            CASE  "
        "                WHEN m_inner.message_type = 'private' THEN "
        "CONCAT(LEAST(m_inner.sender_id, m_inner.receiver_id), '_', GREATEST(m_inner.sender_id, "
        "m_inner.receiver_id)) "
        "                ELSE m_inner.group_id "
        "            END "
        "    ) sub "
        "); ");
    stmt->setInt(1, user_id);
    stmt->setInt(2, user_id);
    stmt->setInt(3, user_id);
    stmt->setInt(4, user_id);
    stmt->setInt(5, user_id);
    auto res = stmt->executeQuery();
    return res;
}
auto selectUnreadGroupMessage(int user_id) {
    auto stmt = SQL::getInstance()->prepareStatement(
        "SELECT m.* "
        "FROM messages m "
        "JOIN group_members gm ON m.group_id = gm.group_id AND gm.user_id = ? "
        "LEFT JOIN is_read ir ON m.id = ir.message_id AND ir.user_id = ? "
        "WHERE ir.message_id IS NULL;");
    stmt->setInt(1, user_id);
    stmt->setInt(2, user_id);
    auto res = stmt->executeQuery();
    return res;
}
auto selectPendingFriend(int user_id) {
    auto stmt = SQL::getInstance()->prepareStatement(
        "SELECT u.*,f.created_at AS ap_time FROM friends f JOIN users u ON f.friend_id = ? AND  "
        "f.status = 'pending' AND f.user_id = u.id");
    stmt->setInt(1, user_id);
    auto res = stmt->executeQuery();
    return res;
}

auto selectPendingGroup(int user_id) {
    auto stmt = SQL::getInstance()->prepareStatement(
        "SELECT gm.*,u.nickname FROM group_members gm JOIN group_members gm2 ON gm.group_id = "
        "gm2.group_id  JOIN users u ON gm.user_id = u.id "
        "WHERE gm2.user_id = ? AND (gm2.role = 'admin' OR gm2.role = 'owner') AND gm.role = "
        "'pending';");
    stmt->setInt(1, user_id);
    auto res = stmt->executeQuery();
    return res;
}
auto selectInvitedGroup(int user_id) {
    auto stmt = SQL::getInstance()->prepareStatement(
        "SELECT * FROM group_members WHERE user_id = ? AND role ='invited';");
    stmt->setInt(1, user_id);
    auto res = stmt->executeQuery();
    return res;
}
auto selectHistoryPrivateMessageById(int user_id, int friend_id) {
    auto stmt = SQL::getInstance()->prepareStatement(
        "SELECT * FROM messages WHERE (sender_id = ? AND receiver_id = ?) OR (sender_id = ? AND "
        "receiver_id = ?) ORDER BY sent_at ASC");
    stmt->setInt(1, user_id);
    stmt->setInt(2, friend_id);
    stmt->setInt(3, friend_id);
    stmt->setInt(4, user_id);
    auto res = stmt->executeQuery();
    return res;
}
auto selectHistoryGroupMessageById(int user_id, int group_id) {
    auto stmt = SQL::getInstance()->prepareStatement(
        "SELECT * FROM messages WHERE group_id = ? ORDER BY sent_at ASC");
    stmt->setInt(1, group_id);
    auto res = stmt->executeQuery();
    return res;
}
#endif