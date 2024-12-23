#ifndef _MESSAGE_DAO_H
#define _MESSAGE_DAO_H

#include "../utils/log.h"
#include "base.h"

bool SendMessage(int user_id, int friend_id, string content) {  //发送消息
    LOG_INFO("Send Message");
    LOG_DEBUG("Arguments: user_id=", user_id, " friend_id=", friend_id, " content=", content);
    return insertMessage(user_id, friend_id, content);
}
Messages GetPrivateMessage(int user_id, int friend_id) {  //获取私聊消息
    LOG_INFO("Get Private Message");
    LOG_DEBUG("Arguments: user_id=", user_id, " friend_id=", friend_id);
    return getPrivateMessageById(user_id, friend_id);
}
Messages GetGroupMessage(int group_id) {  //获取群聊消息
    LOG_INFO("Get Group Message");
    LOG_DEBUG("Arguments: group_id=", group_id);
    return getGroupMessageById(group_id);
}
bool DeleteMessage(int id) {  //删除消息
    LOG_INFO("Delete Message");
    LOG_DEBUG("Arguments: id=", id);
    return deleteMessage(id);
}

#endif