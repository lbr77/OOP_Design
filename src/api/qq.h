#ifndef _API_H
#define _API_H
#pragma once
#include "../db/interact.h"
#include "../http/file.h"
#include "../http/handler.h"
#include "../http/json.h"
#include "../utils/log.h"
#include "../utils/map.h"
#include "../utils/uuid.h"
class QQHandler : public Handler {
   public:
    QQHandler() {
        // POST
        post_handler["register"] = &QQHandler::handleRegister;          // 注册
        post_handler["login"] = &QQHandler::handleLogin;                // 登录
        post_handler["addFriend"] = &QQHandler::handleAddFriend;        // 添加好友
        post_handler["verifyFriend"] = &QQHandler::handleVerifyFriend;  // 验证好友
        post_handler["deleteFriend"] = &QQHandler::handleDeleteFriend;  // 删除好友
        post_handler["createGroup"] = &QQHandler::handleCreateGroup;    // 创建群(群管理员)
        post_handler["addGroup"] = &QQHandler::handleAddGroup;          // 添加群(用户)
        post_handler["verifyGroup"] = &QQHandler::handleVerifyGroup;    // 验证群(群管理员)
        post_handler["deleteGroup"] = &QQHandler::handleDeleteGroup;    // 删除群(群管理员)
        post_handler["kickGroupmember"] =
            &QQHandler::handleKickGroupMember;  // 踢出群成员(群管理员)
        post_handler["addGroupmember"] = &QQHandler::handleAddGroup;  // 添加群成员(群管理员)
        post_handler["searchFriend"] = &QQHandler::handleSearchFriend;
        post_handler["searchGroup"] = &QQHandler::handleSearchGroup;
        post_handler["sendMessage"] = &QQHandler::handleSendMessage;
        post_handler["sendGroupMessage"] = &QQHandler::handleSendGroupMessage;
        // GET // 获取信息
        default_handle(new FileHandler);
    }
    virtual HTTPResponse handle(const HTTPRequest &req) {
        int user_id = -1;
        if (req.cookies.find("session") != req.cookies.end()) {
            auto session = req.cookies["session"];
            auto user_id_s = GET(session);
            if (user_id_s.has_value() && !user_id_s.value().empty()) {
                user_id = stoi(user_id_s.value());
            }
        }
        LOG_DEBUG("User ID:", user_id);
        if (req.path == "/qq/api") {
            if (req.method == "POST") {
                if (req.data.find("action") == req.data.end()) {
                    return makeJson(400, {{"error", "Bad Request"}});
                }
                auto action = req.data["action"].get<string>();

                LOG_INFO("handle POST action: " + action);
                if (post_handler.find(action) != post_handler.end()) {
                    return (this->*post_handler[action])(req.data, user_id);
                } else {
                    return makeJson(404, {{"error", "Action Not Found"}});
                }
            } else if (req.method == "GET") {
                if (req.data.find("action") == req.data.end()) {
                    return makeJson(400, {{"error", "Bad Request"}});
                }
                auto action = req.data["action"].get<string>();
                LOG_INFO("handle GET action: " + action);
                if (get_handler.find(action) != get_handler.end()) {
                    return (this->*get_handler[action])(req.data, user_id);
                } else {
                    return makeJson(404, {{"error", "Action Not Found"}});
                }

            } else {
                return makeJson(405, {{"error", "Method Not Allowed"}});
            }
        }
        // return makeJson(404, {{"error", "Not Found"}});
        return default_handler->handle(req);
    }
    HTTPResponse handleGetInfo(const json &data, int user_id) {
        return makeJson(200, {{"message", "Get Info Success"}});
    }
    HTTPResponse handleGetMessage(const json &data, int user_id) {
        // return all the  about user
        return makeJson(200, {{"message", "Get Message Success"}});
    }
    HTTPResponse handleRegister(const json &data, int user_id) {
        if (user_id != -1) return makeJson(403, {{"error", "Already Logged In"}});
        if (data.find("username") == data.end() || data.find("password") == data.end()) {
            return makeJson(400, {{"error", "Bad Request"}});
        }
        auto username = data["username"].get<string>();
        auto password = data["password"].get<string>();
        auto db = SQL::getInstance();
        if (insertUser(username, password, "QQ")) {
            return makeJson(200, {{"message", "Register Success"}});
        } else {
            return makeJson(500, {{"error", "Internal Server Error"}});
        }
    }
    HTTPResponse handleLogin(const json &data, int user_id) {
        if (user_id != -1) return makeJson(403, {{"error", "Already Logged In"}});
        if (data.find("username") == data.end() || data.find("password") == data.end()) {
            return makeJson(400, {{"error", "Bad Request"}});
        }
        auto username = data["username"].get<string>();
        auto password = data["password"].get<string>();
        auto res = selectUserByNamePassword(username, password, "QQ");
        if (res->next()) {
            auto user_id = res->getInt("id");
            auto session = UUID(4);
            SET(session, std::to_string(user_id));
            auto response = makeJson(200, {{"message", "Login Success"}});
            response.headers["Set-Cookie"] = "session=" + session;
            return response;
        } else {
            return makeJson(401, {{"error", "Username or Password Incorrect"}});
        }
    }
    HTTPResponse handleAddFriend(const json &data, int user_id) {  // 插入数据库
        if (user_id == -1) return makeJson(403, {{"error", "Not Logged In"}});
        if (data.find("friend_id") == data.end()) {
            return makeJson(400, {{"error", "Bad Request"}});
        }
        auto friend_id = data["friend_id"].get<int>();
        if (!selectUserById(friend_id, "QQ")->next()) {  // 在qq上有这个用户
            return makeJson(404, {{"error", "User Not Found"}});
        }
        if (selectFriend(user_id, friend_id)->next()) {
            return makeJson(400, {{"error", "Friend Already Added"}});
        }
        if (insertFriend(user_id, friend_id)) {  // 插入，默认是pending
            return makeJson(200, {{"message", "Add Friend Success"}});
        } else {
            return makeJson(500, {{"error", "Internal Server Error"}});
        }
    }
    HTTPResponse handleVerifyFriend(const json &data, int user_id) {
        if (user_id == -1) return makeJson(403, {{"error", "Not Logged In"}});
        if (data.find("friend_id") == data.end()) {
            return makeJson(400, {{"error", "Bad Request"}});
        }
        auto friend_id = data["friend_id"].get<int>();
        if (!selectUserById(friend_id, "QQ")->next()) {  // 在qq上有这个用户
            return makeJson(404, {{"error", "User Not Found"}});
        }
        if (!updateFriend(friend_id, user_id)) {  // 正向添加了
            return makeJson(500, {{"error", "update failed"}});
        }
        if (insertFriend(user_id, friend_id, "active")) {  // 反向添加
            return makeJson(200, {{"message", "Verify Friend Success"}});
        } else {
            return makeJson(500, {{"error", "Internal Server Error"}});
        }
    }
    HTTPResponse handleDeleteFriend(const json &data, int user_id) {
        if (user_id == -1) return makeJson(403, {{"error", "Not Logged In"}});
        if (data.find("friend_id") == data.end()) {
            return makeJson(400, {{"error", "Bad Request"}});
        }
        auto friend_id = data["friend_id"].get<int>();
        auto res = selectFriend(user_id, friend_id);
        if (!res->next()) {
            return makeJson(404, {{"error", "Friend Not Found"}});
        }
        if (!deleteFriend(user_id, friend_id)) {
            return makeJson(500, {{"error", "Internal Server Error"}});
        }
        if (!deleteFriend(friend_id, user_id)) {
            return makeJson(500, {{"error", "Internal Server Error"}});
        }
        return makeJson(200, {{"message", "Delete Friend Success"}});
    }
    HTTPResponse handleCreateGroup(const json &data, int user_id) {
        if (user_id == -1) return makeJson(403, {{"error", "Not Logged In"}});
        if (data.find("group_name") == data.end()) {
            return makeJson(400, {{"error", "Bad Request"}});
        }
        auto group_name = data["group_name"].get<string>();
        if (!createGroup(user_id, group_name, "QQ")) {
            return makeJson(500, {{"error", "Internal Server Error"}});
        }
        auto g = selectGroupByCreator(user_id, group_name);
        if (g->next()) {
            auto group_id = g->getInt("id");
            if (insertGroupMember(group_id, user_id, "owner")) {
                return makeJson(200, {{"msg", "Create Group Success", "group_id", group_id}});
            }
        }
        return makeJson(500, {{"error", "Internal Server Error"}});
    }
    HTTPResponse handleAddGroup(const json &data, int user_id) {
        if (user_id == -1) return makeJson(403, {{"error", "Not Logged In"}});
        if (data.find("group_id") == data.end()) {
            return makeJson(400, {{"error", "Bad Request"}});
        }
        auto group_id = data["group_id"].get<int>();
        auto res = selectGroupById(group_id);
        if (!res->next()) {
            return makeJson(404, {{"error", "Group Not Found"}});
        }
        if (!insertGroupMember(group_id, user_id)) {  // default pending
            return makeJson(500, {{"error", "Internal Server Error"}});
        }
        return makeJson(200, {{"message", "Add Group Success"}});
    }
    HTTPResponse handleVerifyGroup(const json &data, int user_id) {
        if (user_id == -1) return makeJson(403, {{"error", "Not Logged In"}});
        if (data.find("group_id") == data.end() || data.find("user_id") == data.end()) {
            return makeJson(400, {{"error", "Bad Request"}});
        }
        auto group_id = data["group_id"].get<int>();
        auto app_user_id = data["user_id"].get<int>();  // 申请加入的用户
        auto res = selectGroupById(group_id);
        if (!res->next()) {
            return makeJson(404, {{"error", "Group Not Found"}});
        }
        auto ress = selectGroupMember(group_id, user_id);
        if (!ress->next() ||
            (ress->getString("role") != "owner" && ress->getString("role") != "admin")) {
            LOG_INFO(ress->getString("role"));
            return makeJson(403, {{"error", "Permission Denied"}});
        }
        auto resss = selectGroupMember(group_id, app_user_id);
        if (!resss->next()) {
            return makeJson(404, {{"error", "User Not Found"}});
        }
        if (!updateGroupMember(group_id, app_user_id, "member")) {
            return makeJson(500, {{"error", "Internal Server Error"}});
        }
        return makeJson(200, {{"message", "Verify Group Success"}});
    }
    HTTPResponse handleDeleteGroup(const json &data, int user_id) {
        if (user_id == -1) return makeJson(403, {{"error", "Not Logged In"}});
        if (data.find("group_id") == data.end()) {
            return makeJson(400, {{"error", "Bad Request"}});
        }
        auto group_id = data["group_id"].get<int>();
        auto res = selectGroupById(group_id);
        if (!res->next()) {
            return makeJson(404, {{"error", "Group Not Found"}});
        }
        auto ress = selectGroupMember(group_id, user_id);
        if (!ress->next() || ress->getString("role") != "owner") {
            return makeJson(403, {{"error", "Permission Denied"}});
        }
        if (!deleteGroupById(group_id)) {
            return makeJson(500, {{"error", "Internal Server Error"}});
        }
        return makeJson(200, {{"message", "Delete Group Success"}});
    }
    HTTPResponse handleKickGroupMember(const json &data, int user_id) {
        if (user_id == -1) return makeJson(403, {{"error", "Not Logged In"}});
        if (data.find("group_id") == data.end() || data.find("user_id") == data.end()) {
            return makeJson(400, {{"error", "Bad Request"}});
        }
        auto group_id = data["group_id"].get<int>();
        auto app_user_id = data["user_id"].get<int>();  // 被踢出的用户
        auto res = selectGroupById(group_id);
        if (!res->next()) {
            return makeJson(404, {{"error", "Group Not Found"}});
        }
        auto ress = selectGroupMember(group_id, user_id);
        if (!ress->next() ||
            (ress->getString("role") != "owner" && ress->getString("role") != "admin")) {
            return makeJson(403, {{"error", "Permission Denied"}});
        }
        auto resss = selectGroupMember(group_id, app_user_id);
        if (!resss->next()) {
            return makeJson(404, {{"error", "User Not Found"}});
        }
        if (!deleteGroupMember(group_id, app_user_id)) {
            return makeJson(500, {{"error", "Internal Server Error"}});
        }
        return makeJson(200, {{"message", "Kick Group Member Success"}});
    }
    HTTPResponse handleSearchFriend(const json &data, int user_id) {
        if (user_id == -1) return makeJson(403, {{"error", "Not Logged In"}});
        if (data.find("username") == data.end()) {
            return makeJson(400, {{"error", "Bad Request"}});
        }
        auto username = data["username"].get<string>();
        auto res = searchUser(username, "QQ");
        auto result = json::array();
        while (res->next()) {
            result.push_back({{"id", res->getInt("id")}, {"username", res->getString("nickname")}});
        }
        return makeJson(200, {{"message", "Search Friend Success"}, {"result", result}});
    }
    HTTPResponse handleSearchGroup(const json &data, int user_id) {
        if (user_id == -1) return makeJson(403, {{"error", "Not Logged In"}});
        if (data.find("group_name") == data.end()) {
            return makeJson(400, {{"error", "Bad Request"}});
        }
        auto group_name = data["group_name"].get<string>();
        auto res = searchGroup(group_name, "QQ");
        auto result = json::array();
        while (res->next()) {
            result.push_back(
                {{"id", res->getInt("id")}, {"group_name", res->getString("group_name")}});
        }
        return makeJson(200, {{"message", "Search Group Success"}, {"result", result}});
    }
    HTTPResponse handleSendMessage(const json &data, int user_id) {
        if (user_id == -1) return makeJson(403, {{"error", "Not Logged In"}});
        if (data.find("friend_id") == data.end() || data.find("message") == data.end()) {
            return makeJson(400, {{"error", "Bad Request"}});
        }
        auto friend_id = data["friend_id"].get<int>();
        auto message = data["message"].get<string>();
        auto res = selectFriend(user_id, friend_id);
        if (!res->next()) {
            return makeJson(404, {{"error", "Friend Not Found"}});
        }
        if (!insertPrivateMessage(user_id, friend_id, message)) {
            return makeJson(500, {{"error", "Internal Server Error"}});
        }
        return makeJson(200, {{"message", "Send Message Success"}});
    }
    HTTPResponse handleSendGroupMessage(const json &data, int user_id) {
        if (user_id == -1) return makeJson(403, {{"error", "Not Logged In"}});
        if (data.find("group_id") == data.end() || data.find("message") == data.end()) {
            return makeJson(400, {{"error", "Bad Request"}});
        }
        auto group_id = data["group_id"].get<int>();
        auto message = data["message"].get<string>();
        auto res = selectGroupMember(group_id, user_id);
        if (!res->next()) {
            return makeJson(404, {{"error", "Group Not Found"}});
        }
        if (!insertGroupMessage(user_id, group_id, message)) {
            return makeJson(500, {{"error", "Internal Server Error"}});
        }
        return makeJson(200, {{"message", "Send Group Message Success"}});
    }
    HTTPResponse handleAddGroupMember(const json &data, int user_id) {
        if (user_id == -1) return makeJson(403, {{"error", "Not Logged In"}});
        if (data.find("group_id") == data.end() || data.find("user_id") == data.end()) {
            return makeJson(400, {{"error", "Bad Request"}});
        }
        auto group_id = data["group_id"].get<int>();
        auto app_user_id = data["user_id"].get<int>();
        auto res = selectGroupById(group_id);
        if (!res->next()) {
            return makeJson(404, {{"error", "Group Not Found"}});
        }
        auto ress = selectGroupMember(group_id, user_id);
        if (!ress->next() ||
            (ress->getString("role") != "owner" && ress->getString("role") != "admin")) {
            return makeJson(403, {{"error", "Permission Denied"}});
        }
        if (!insertGroupMember(group_id, app_user_id, "active")) {
            return makeJson(500, {{"error", "Internal Server Error"}});
        }
        return makeJson(200, {{"message", "Add Group Member Success"}});
    }

   private:
    map<string, HTTPResponse (QQHandler::*)(const json &, int)> post_handler;
    map<string, HTTPResponse (QQHandler::*)(const json &, int)> get_handler;
};
#endif