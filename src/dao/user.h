#ifndef _DAO_USER_H_
#define _DAO_USER_H_

#include "../utils/log.h"
#include "base.h"
User Login(int id, string password, string platform) {
    LOG_INFO("User Login");
    LOG_DEBUG("Arguments: id=", id, " password=", password, " platform=", platform);
    auto users = selectUserByNickNameAndPassword(id, platform, password);
    if (users.size() == 0) {
        return User();
    }
    if (users.size() > 1) {
        return User();
    }
    return users[0];
}
User Register(string nickname, string password, string platform) {
    LOG_INFO("User Register");
    LOG_DEBUG("Arguments: nickname=", nickname, " password=", password, " platform=", platform);
    auto users = selectUserByNickName(nickname, platform);
    if (users.size() > 0) {
        return User();
    }
    auto query = SQL::getInstance()->prepareStatement(
        "INSERT INTO user(nickname,password,platform) VALUES(?,?,?);");
    query->setInt(1, id);
    query->setString(2, password);
    query->setString(3, platform);
    query->executeUpdate();
    return User(id, "", password, "", "", "", platform, true);
}
User Update(string nickname, string birthdate, string location, int id) {
    LOG_INFO("User Update");
    LOG_DEBUG("Arguments: nickname=", nickname, " birthdate=", birthdate, " location=", location,
              " id=", id);
    auto user = User(id, nickname, "", birthdate, "", location, "", true);
    return updateUser(user);
}
Users Search(string nickname, string platform) {
    LOG_INFO("User Search");
    LOG_DEBUG("Arguments: nickname=", nickname, " platform=", platform);
    return selectUserByNickName(nickname, platform);
}
Users GetCommonFriends(int id) {
    LOG_INFO("Get Common Friends");
    LOG_DEBUG("Arguments: id=", id);
    return selectUserByCommonFriends(id);
}
#endif