#ifndef _DAO_QQ_MSG_H_
#define _DAO_QQ_MSG_H_
#pragma once
#include "../../class/qq.h"
#include "../../db/db.h"
namespace qq {
typedef std::vector<Message> Messages;
bool insertMessage(Message msg) {
    auto conn = SQL::getInstance();
    std::string sql =
        "INSERT INTO messages_qq (sender_id, chat_id, message_type, content) VALUES (?, ?, ?, ?)";
    auto stmt = conn->prepareStatement(sql);
    stmt->setInt(1, msg.getSenderId());
    stmt->setInt(2, msg.getChatId());
    stmt->setString(3, msg.getType());
    stmt->setString(4, msg.getContent());
    return stmt->executeUpdate() == 1;
}
Messages selectMessage(int chat_id) {
    std::string sql =
        "SELECT * FROM messages_qq WHERE chat_id = ? AND is_deleted = FALSE ORDER BY sent_at ASC;";
    auto conn = SQL::getInstance();
    auto stmt = conn->prepareStatement(sql);
    stmt->setInt(1, chat_id);
    auto res = stmt->executeQuery();
    Messages messages;
    while (res->next()) {
        messages.push_back(Message(res->getInt(1), res->getInt(2), res->getInt(3),
                                   res->getString(4), res->getString(5), res->getString(6),
                                   res->getBoolean(7)));
    }
    return messages;
}
bool updateMessageStatus(int message_id, int user_id) {
    std::string sql =
        "INSERT INTO message_status_qq (message_id, user_id, is_read, read_at) VALUES (?, ?, TRUE, "
        "NOW())";
    auto conn = SQL::getInstance();
    auto stmt = conn->prepareStatement(sql);
    stmt->setInt(1, message_id);
    stmt->setInt(2, user_id);
    return stmt->executeUpdate() == 1;
}
bool deleteMessage(int message_id) {
    std::string sql = "UPDATE messages_qq SET is_deleted = TRUE WHERE id = ?";
    auto conn = SQL::getInstance();
    auto stmt = conn->prepareStatement(sql);
    stmt->setInt(1, message_id);
    return stmt->executeUpdate() == 1;
}
}  // namespace qq

#endif