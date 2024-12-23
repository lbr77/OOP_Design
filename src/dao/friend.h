#ifndef _DAO_FRIEND_H_
#define _DAO_FRIEND_H_
#include "../utils/log.h"
#include "base.h"

bool ApplyFriend(int user_id, int friend_id) {  //申请好友
    LOG_INFO("Apply Friend");
    LOG_DEBUG("Arguments: user_id=", user_id, " friend_id=", friend_id);
    return insertFriend(user_id, friend_id, "pending");
}
bool AcceptFriend(int user_id, int friend_id) {  //接受好友
    LOG_INFO("Accept Friend");
    LOG_DEBUG("Arguments: user_id=", user_id, " friend_id=", friend_id);
    updateFriendStatus(user_id, friend_id, "active");
    insertFriend(friend_id, user_id, "active");
}
bool RejectFriend(int user_id, int friend_id) {  //拒绝好友
    LOG_INFO("Reject Friend");
    LOG_DEBUG("Arguments: user_id=", user_id, " friend_id=", friend_id);
    // return updateFriendStatus(user_id,friend_id,"");
    return deleteFriend(user_id, friend_id);
}
bool BlockFriend(int user_id, int friend_id) {  //拉黑好友
    LOG_INFO("Block Friend");
    LOG_DEBUG("Arguments: user_id=", user_id, " friend_id=", friend_id);
    return updateFriendStatus(user_id, friend_id, "blocked");
}

#endif  // _DAO_FRIEND_H_