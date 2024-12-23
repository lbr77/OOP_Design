#ifndef _BASH_CLASS_H_
#define _BASE_CLASS_H_

#pragma once
#include <nlohmann/json.hpp>
#include <rttr/registration>

#include "./reflection.h"
using json = nlohmann::json;
using std::string;

class Base {  // abstract one
   public:
    int getId() const {
        return id;
    }
    void setId(int id) {
        this->id = id;
    }
    RTTR_ENABLE()
   private:
    int id;  // 主键
};

class User : public Base {
   private:
    int id;
    string nickname, password, birthdate, created_at, location, platform;
    bool status;  // 1 active 0 disabled
   public:
    User() = default;
    User(int id, string nickname, string password, string birthdate, string created_at,
         string location, string platform, bool status)
        : id(id),
          nickname(nickname),
          password(password),
          birthdate(birthdate),
          created_at(created_at),
          location(location),
          platform(platform),
          status(status) {}
    int getId() const {
        return id;
    }
    void setId(int id) {
        this->id = id;
    }
    string getNickname() const {
        return nickname;
    }
    void setNickname(string nickname) {
        this->nickname = nickname;
    }
    string getPassword() const {
        return password;
    }
    void setPassword(string password) {
        this->password = password;
    }
    string getBirthdate() const {
        return birthdate;
    }
    void setBirthdate(string birthdate) {
        this->birthdate = birthdate;
    }
    string getCreatedAt() const {
        return created_at;
    }
    void setCreatedAt(string created_at) {
        this->created_at = created_at;
    }
    string getLocation() const {
        return location;
    }
    void setLocation(string location) {
        this->location = location;
    }
    string getPlatform() const {
        return platform;
    }
    void setPlatform(string platform) {
        this->platform = platform;
    }
    bool getStatus() const {
        return status;
    }
    void setStatus(bool status) {
        this->status = status;
    }
    RTTR_ENABLE(Base)
};

class Friend : public Base {
   private:
    int id, user_id, friend_id;
    string created_at, status;

   public:
    Friend() = default;
    Friend(int id, int user_id, int friend_id, string created_at, string status)
        : id(id), user_id(user_id), friend_id(friend_id), created_at(created_at), status(status) {}
    int getId() const {
        return id;
    }
    void setId(int id) {
        this->id = id;
    }
    int getUserId() const {
        return user_id;
    }
    void setUserId(int user_id) {
        this->user_id = user_id;
    }
    int getFriendId() const {
        return friend_id;
    }
    void setFriendId(int friend_id) {
        this->friend_id = friend_id;
    }
    string getCreatedAt() const {
        return created_at;
    }
    void setCreatedAt(string created_at) {
        this->created_at = created_at;
    }
    string getStatus() const {
        return status;
    }
    void setStatus(string status) {
        this->status = status;
    }
    RTTR_ENABLE(Base)
};

class Group : public Base {
   private:
    int id, owner_id;
    string group_name, platform;

   public:
    Group() = default;
    Group(int id, int owner_id, string group_name, string platform)
        : id(id), owner_id(owner_id), group_name(group_name), platform(platform) {}
    int getId() const {
        return id;
    }
    void setId(int id) {
        this->id = id;
    }
    int getOwnerId() const {
        return owner_id;
    }
    void setOwnerId(int owner_id) {
        this->owner_id = owner_id;
    }
    string getGroupName() const {
        return group_name;
    }
    void setGroupName(string group_name) {
        this->group_name = group_name;
    }
    string getPlatform() const {
        return platform;
    }
    void setPlatform(string platform) {
        this->platform = platform;
    }
    RTTR_ENABLE(Base)
};

class GroupMember : public Base {
   private:
    int id, group_id, user_id;
    string created_at, status;

   public:
    GroupMember() = default;
    GroupMember(int id, int group_id, int user_id, string created_at, string status)
        : id(id), group_id(group_id), user_id(user_id), created_at(created_at), status(status) {}
    int getId() const {
        return id;
    }
    void setId(int id) {
        this->id = id;
    }
    int getGroupId() const {
        return group_id;
    }
    void setGroupId(int group_id) {
        this->group_id = group_id;
    }
    int getUserId() const {
        return user_id;
    }
    void setUserId(int user_id) {
        this->user_id = user_id;
    }
    string getCreatedAt() const {
        return created_at;
    }
    void setCreatedAt(string created_at) {
        this->created_at = created_at;
    }
    string getStatus() const {
        return status;
    }
    void setStatus(string status) {
        this->status = status;
    }
    RTTR_ENABLE(Base)
};

class Message : public Base {
   private:
    int id, sender_id, receiver_id, group_id;
    string content, created_at, message_type;

   public:
    Message() = default;
    Message(int id, int sender_id, int receiver_id, int group_id, string content, string created_at,
            string message_type)
        : id(id),
          sender_id(sender_id),
          receiver_id(receiver_id),
          group_id(group_id),
          content(content),
          created_at(created_at),
          message_type(message_type) {}
    int getId() const {
        return id;
    }
    void setId(int id) {
        this->id = id;
    }
    int getSenderId() const {
        return sender_id;
    }
    void setSenderId(int sender_id) {
        this->sender_id = sender_id;
    }
    int getReceiverId() const {
        return receiver_id;
    }
    void setReceiverId(int receiver_id) {
        this->receiver_id = receiver_id;
    }
    int getGroupId() const {
        return group_id;
    }
    void setGroupId(int group_id) {
        this->group_id = group_id;
    }
    string getContent() const {
        return content;
    }
    void setContent(string content) {
        this->content = content;
    }
    string getCreatedAt() const {
        return created_at;
    }
    void setCreatedAt(string created_at) {
        this->created_at = created_at;
    }
    string getMessageType() const {
        return message_type;
    }
    void setMessageType(string message_type) {
        this->message_type = message_type;
    }
    RTTR_ENABLE(Base)
};

RTTR_REGISTRATION {
    rttr::registration::class_<Base>("Base").property("id", &Base::getId, &Base::setId);
    rttr::registration::class_<User>("User")
        .constructor<int, string, string, string, string, string, string, bool>()
        .property("id", &User::getId, &User::setId)
        .property("nickname", &User::getNickname, &User::setNickname)
        .property("password", &User::getPassword, &User::setPassword)
        .property("birthdate", &User::getBirthdate, &User::setBirthdate)
        .property("created_at", &User::getCreatedAt, &User::setCreatedAt)
        .property("location", &User::getLocation, &User::setLocation)
        .property("platform", &User::getPlatform, &User::setPlatform)
        .property("status", &User::getStatus, &User::setStatus);
    rttr::registration::class_<Friend>("Friend")
        .constructor<int, int, int, string, string>()
        .property("id", &Friend::getId, &Friend::setId)
        .property("user_id", &Friend::getUserId, &Friend::setUserId)
        .property("friend_id", &Friend::getFriendId, &Friend::setFriendId)
        .property("created_at", &Friend::getCreatedAt, &Friend::setCreatedAt)
        .property("status", &Friend::getStatus, &Friend::setStatus);
    rttr::registration::class_<Group>("Group")
        .constructor<int, int, string, string>()
        .property("id", &Group::getId, &Group::setId)
        .property("owner_id", &Group::getOwnerId, &Group::setOwnerId)
        .property("group_name", &Group::getGroupName, &Group::setGroupName)
        .property("platform", &Group::getPlatform, &Group::setPlatform);
    rttr::registration::class_<GroupMember>("GroupMember")
        .constructor<int, int, int, string, string>()
        .property("id", &GroupMember::getId, &GroupMember::setId)
        .property("group_id", &GroupMember::getGroupId, &GroupMember::setGroupId)
        .property("user_id", &GroupMember::getUserId, &GroupMember::setUserId)
        .property("created_at", &GroupMember::getCreatedAt, &GroupMember::setCreatedAt)
        .property("status", &GroupMember::getStatus, &GroupMember::setStatus);
    rttr::registration::class_<Message>("Message")
        .constructor<int, int, int, int, string, string, string>()
        .property("id", &Message::getId, &Message::setId)
        .property("sender_id", &Message::getSenderId, &Message::setSenderId)
        .property("receiver_id", &Message::getReceiverId, &Message::setReceiverId)
        .property("group_id", &Message::getGroupId, &Message::setGroupId)
        .property("content", &Message::getContent, &Message::setContent)
        .property("created_at", &Message::getCreatedAt, &Message::setCreatedAt)
        .property("message_type", &Message::getMessageType, &Message::setMessageType);
}

#endif