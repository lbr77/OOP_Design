#ifndef _DAO_QQ_CHAT_H_
#define _DAO_QQ_CHAT_H_
#pragma once
#include "../../class/qq.h"
#include "../../db/db.h"

namespace qq {
typedef std::vector<ChatMember> ChatMembers;
Chat selectChatByTitle(std::string title) {
    auto conn = SQL::getInstance();
    std::string sql = "SELECT * FROM chats_qq WHERE title = ?";
    auto stmt = conn->prepareStatement(sql);
    stmt->setString(1, title);
    auto res = stmt->executeQuery();
    if (res->next()) {
        return Chat(res->getInt(1), res->getString(2), res->getString(3), res->getString(4),
                    res->getString(5), res->getInt(6));
    }
    return Chat();
}
Chat selectChatById(int id) {
    auto conn = SQL::getInstance();
    std::string sql = "SELECT * FROM chats_qq WHERE id = ?";
    auto stmt = conn->prepareStatement(sql);
    stmt->setInt(1, id);
    auto res = stmt->executeQuery();
    if (res->next()) {
        return Chat(res->getInt(1), res->getString(2), res->getString(3), res->getString(4),
                    res->getString(5), res->getInt(6));
    }
    return Chat();
}
bool updateChatMember(int chat_id, int user_id, std::string role) {
    auto conn = SQL::getInstance();
    std::string sql = "UPDATE chat_members_qq SET role = ? WHERE chat_id = ? AND user_id = ?";
    auto stmt = conn->prepareStatement(sql);
    stmt->setString(1, role);
    stmt->setInt(2, chat_id);
    stmt->setInt(3, user_id);
    return stmt->executeUpdate() == 1;
}
Chat insertChat(Chat chat) {  // 增
    auto conn = SQL::getInstance();
    std::string sql =
        "INSERT INTO chats_qq (type_, title, description , creator_id) VALUES (?, ?, ?, ?)";
    auto stmt = conn->prepareStatement(sql);
    stmt->setString(1, chat.getType());
    stmt->setString(2, chat.getTitle());
    stmt->setString(3, chat.getDescription());
    stmt->setInt(4, chat.getCreatorId());
    stmt->executeUpdate();
    return selectChatByTitle(chat.getTitle());
    // return stmt->executeUpdate() == 1;
}
bool updateChat(Chat chat) {  // 改
    auto conn = SQL::getInstance();
    std::string sql =
        "UPDATE chats_qq SET title = ?, description = ? WHERE id = ?";  // 只有 title 和 description
                                                                        // 可以修改
    auto stmt = conn->prepareStatement(sql);
    stmt->setString(1, chat.getTitle());
    stmt->setString(2, chat.getDescription());
    stmt->setInt(3, chat.getId());
    return stmt->executeUpdate() == 1;
}
bool insertChatMember(int chat_id, int user_id, std::string role) {  // 增
    auto conn = SQL::getInstance();
    std::string sql = "INSERT INTO chat_members_qq (chat_id, user_id, role) VALUES (?, ?, ?)";
    auto stmt = conn->prepareStatement(sql);
    stmt->setInt(1, chat_id);
    stmt->setInt(2, user_id);
    stmt->setString(3, role);
    return stmt->executeUpdate() == 1;
}
bool deleteChatMember(int chat_id, int user_id) {  // 删
    auto conn = SQL::getInstance();
    std::string sql = "DELETE FROM chat_members_qq WHERE chat_id = ? AND user_id = ?";
    auto stmt = conn->prepareStatement(sql);
    stmt->setInt(1, chat_id);
    stmt->setInt(2, user_id);
    return stmt->executeUpdate() == 1;
}
ChatMembers selectChatMember(int chat_id) {
    std::string sql =
        "SELECT users_qq.id, chat_members_qq.role, chat_members_qq.joined_at "
        "FROM chat_members_qq "
        "JOIN users_qq ON chat_members_qq.user_id = users_qq.id WHERE chat_members_qq.chat_id = ?;";
    auto conn = SQL::getInstance();
    auto stmt = conn->prepareStatement(sql);
    stmt->setInt(1, chat_id);
    auto res = stmt->executeQuery();
    ChatMembers chat_members;
    while (res->next()) {
        chat_members.push_back(
            ChatMember(chat_id, res->getInt(1), res->getString(2), res->getString(3)));
    }
    return chat_members;
}
std::string getRole(int chat_id, int user_id) {
    std::string sql =
        "SELECT role "
        "FROM chat_members "
        "WHERE chat_id = ? AND user_id = ?;";
    auto stmt = SQL::getInstance()->prepareStatement(sql);
    stmt->setInt(1, chat_id);
    stmt->setInt(2, user_id);
    auto res = stmt->executeQuery();
    if (res->next()) {
        return res->getString(1);
    }
    return "";
}
}  // namespace qq
#endif