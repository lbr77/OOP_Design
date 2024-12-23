#ifndef _GROUP_DAO_H_
#define _GROUP_DAO_H_

#include "../utils/log.h"
#include "base.h"

bool CreateGroup(int user_id, string group_name, string platform) {  //创建群聊
    LOG_INFO("Create Group");
    LOG_DEBUG("Arguments: user_id=", user_id, " group_name=", group_name);
    auto g = createGroup(group_name, platform, user_id);
    LOG_DEBUG("Group ID=", g.getId());
    return true;
}
bool JoinGroup(int user_id, int group_id) {  //加入群聊
    LOG_INFO("Join Group");
    LOG_DEBUG("Arguments: user_id=", user_id, " group_id=", group_id);
    return insertGroupMember(group_id, user_id, "pending");
}
bool AcceptGroup(int user_id, int group_id) {  //接受群聊邀请
    LOG_INFO("Accept Group");
    LOG_DEBUG("Arguments: user_id=", user_id, " group_id=", group_id);
    return updateGroupMemberRole(group_id, user_id, "member");
}
bool RejectGroup(int user_id, int group_id) {  //拒绝群聊邀请
    LOG_INFO("Reject Group");
    LOG_DEBUG("Arguments: user_id=", user_id, " group_id=", group_id);
    return deleteGroupMember(group_id, user_id);
}

#endif