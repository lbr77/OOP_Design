#ifndef _QQ_CLASS_H_
#define _QQ_CLASS_H_
#pragma once
#include "base.h"
namespace qq {
class User : public Base {
   private:
    int id;
    std::string username, password, created_at, birthday, address;

   public:
    User(int id = 0, std::string username = "", std::string password = "",
         std::string created_at = "", std::string birthday = "", std::string address = "") {
        this->id = id;
        this->username = username;
        this->password = password;
        this->created_at = created_at;
        this->birthday = birthday;
        this->address = address;
    }
    int getId() {
        return id;
    }
    std::string getUsername() {
        return username;
    }
    std::string getPassword() {
        return password;
    }
    std::string getCreatedAt() {
        return created_at;
    }
    std::string getBirthday() {
        return birthday;
    }
    std::string getAddress() {
        return address;
    }
    void setId(int id) {
        this->id = id;
    }
    void setUsername(std::string username) {
        this->username = username;
    }
    void setPassword(std::string password) {
        this->password = password;
    }
    void setCreatedAt(std::string created_at) {
        this->created_at = created_at;
    }
    void setBirthday(std::string birthday) {
        this->birthday = birthday;
    }
    void setAddress(std::string address) {
        this->address = address;
    }
    json to_json() {
        json j;
        j["id"] = id;
        j["username"] = username;
        j["created_at"] = created_at;
        j["birthday"] = birthday;
        j["address"] = address;
        return j;
    }
};
class Chat : public Base {
   private:
    int id;
    std::string type, title, description, created_at;
    int creator_id;

   public:
    Chat() {}
    Chat(int id, std::string type, std::string title, std::string description,
         std::string created_at, int creator_id) {
        this->id = id;
        this->type = type;
        this->title = title;
        this->description = description;
        this->created_at = created_at;
        this->creator_id = creator_id;
    }
    int getId() {
        return id;
    }
    std::string getType() {
        return type;
    }
    std::string getTitle() {
        return title;
    }
    std::string getDescription() {
        return description;
    }
    std::string getCreatedAt() {
        return created_at;
    }
    int getCreatorId() {
        return creator_id;
    }
    void setId(int id) {
        this->id = id;
    }
    void setType(std::string type) {
        this->type = type;
    }
    void setTitle(std::string title) {
        this->title = title;
    }
    void setDescription(std::string description) {
        this->description = description;
    }
    void setCreatedAt(std::string created_at) {
        this->created_at = created_at;
    }
    void setCreatorId(int creator_id) {
        this->creator_id = creator_id;
    }
    json to_json() {
        json j;
        j["id"] = id;
        j["type"] = type;
        j["title"] = title;
        j["description"] = description;
        j["created_at"] = created_at;
        j["creator_id"] = creator_id;
        return j;
    }
};
class ChatMember : public Base {
   private:
    int chat_id, user_id;
    std::string role, joined_at;

   public:
    ChatMember() {}
    ChatMember(int chat_id, int user_id, std::string role, std::string joined_at) {
        this->chat_id = chat_id;
        this->user_id = user_id;
        this->role = role;
        this->joined_at = joined_at;
    }
    int getChatId() {
        return chat_id;
    }
    int getUserId() {
        return user_id;
    }
    std::string getRole() {
        return role;
    }
    std::string getJoinedAt() {
        return joined_at;
    }
    void setChatId(int chat_id) {
        this->chat_id = chat_id;
    }
    void setUserId(int user_id) {
        this->user_id = user_id;
    }
    void setRole(std::string role) {
        this->role = role;
    }
    void setJoinedAt(std::string joined_at) {
        this->joined_at = joined_at;
    }
    json to_json() {
        json j;
        j["chat_id"] = chat_id;
        j["user_id"] = user_id;
        j["role"] = role;
        j["joined_at"] = joined_at;
        return j;
    }
};
class Message : public Base {
   private:
    int id, sender_id, chat_id;
    std::string type, content, send_at;
    bool is_deleted;

   public:
    Message() {}
    Message(int id, int sender_id, int chat_id, std::string type, std::string content,
            std::string send_at, bool is_deleted) {
        this->id = id;
        this->sender_id = sender_id;
        this->chat_id = chat_id;
        this->type = type;
        this->content = content;
        this->send_at = send_at;
        this->is_deleted = is_deleted;
    }
    int getId() {
        return id;
    }
    int getSenderId() {
        return sender_id;
    }
    int getChatId() {
        return chat_id;
    }
    std::string getType() {
        return type;
    }
    std::string getContent() {
        return content;
    }
    std::string getSendAt() {
        return send_at;
    }
    bool getIsDeleted() {
        return is_deleted;
    }
    void setId(int id) {
        this->id = id;
    }
    void setSenderId(int sender_id) {
        this->sender_id = sender_id;
    }
    void setChatId(int chat_id) {
        this->chat_id = chat_id;
    }
    void setType(std::string type) {
        this->type = type;
    }
    void setContent(std::string content) {
        this->content = content;
    }
    void setSendAt(std::string send_at) {
        this->send_at = send_at;
    }
    void setIsDeleted(bool is_deleted) {
        this->is_deleted = is_deleted;
    }
    json to_json() {
        json j;
        j["id"] = id;
        j["sender_id"] = sender_id;
        j["chat_id"] = chat_id;
        j["type"] = type;
        j["content"] = content;
        j["send_at"] = send_at;
        j["is_deleted"] = is_deleted;
        return j;
    }
};
class MessageStatus : public Base {
   private:
    int message_id, user_id;
    bool is_read;
    std::string read_at;

   public:
    MessageStatus() {}
    MessageStatus(int message_id, int user_id, bool is_read, std::string read_at) {
        this->message_id = message_id;
        this->user_id = user_id;
        this->is_read = is_read;
        this->read_at = read_at;
    }
    int getMessageId() {
        return message_id;
    }
    int getUserId() {
        return user_id;
    }
    bool getIsRead() {
        return is_read;
    }
    std::string getReadAt() {
        return read_at;
    }
    void setMessageId(int message_id) {
        this->message_id = message_id;
    }
    void setUserId(int user_id) {
        this->user_id = user_id;
    }
    void setIsRead(bool is_read) {
        this->is_read = is_read;
    }
    void setReadAt(std::string read_at) {
        this->read_at = read_at;
    }
    json to_json() {
        json j;
        j["message_id"] = message_id;
        j["user_id"] = user_id;
        j["is_read"] = is_read;
        j["read_at"] = read_at;
        return j;
    }
};
}  // namespace qq

#endif