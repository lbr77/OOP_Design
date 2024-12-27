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
class WXHandler : public Handler {
   public:
    WXHandler() {
        // POST
        post_handler["register"] = &WXHandler::handleRegister;          // 注册1
        post_handler["login"] = &WXHandler::handleLogin;                // 登录1
        post_handler["logout"] = &WXHandler::handleLogout;              // 登出1
        post_handler["addFriend"] = &WXHandler::handleAddFriend;        // 添加好友1
        post_handler["verifyFriend"] = &WXHandler::handleVerifyFriend;  // 验证好友1
        post_handler["deleteFriend"] = &WXHandler::handleDeleteFriend;  // 删除好友1
        post_handler["createGroup"] = &WXHandler::handleCreateGroup;    // 创建群(群管理员)
        post_handler["getGroup"] = &WXHandler::handleGetGroup;          // 获取群成员(用户)
        post_handler["deleteGroup"] = &WXHandler::handleDeleteGroup;  // 删除群(群主)
        post_handler["kickGroupmember"] =
            &WXHandler::handleKickGroupMember;  // 踢出群成员(群管理员)
        post_handler["addGroupmember"] = &WXHandler::handleInviteGroup;  // 邀请加入群(用户)
        post_handler["acceptGroup"] = &WXHandler::handleAcceptGroup;  // 同意加入群(用户)
        post_handler["exitGroup"] = &WXHandler::handleExitGroup;      // 退出群(用户)
        post_handler["searchFriend"] = &WXHandler::handleSearchFriend;
        post_handler["searchGroup"] = &WXHandler::handleSearchGroup;
        post_handler["sendMessage"] = &WXHandler::handleSendMessage;
        post_handler["sendGroupMessage"] = &WXHandler::handleSendGroupMessage;
        post_handler["markRead"] = &WXHandler::handleMarkRead;  // 标记已读
        post_handler["updateProfile"] = &WXHandler::handleUpdateProfile;
        // GET // 获取信息
        post_handler["getInfo"] = &WXHandler::handleGetInfo;  // 获取信息（用户、好友、群）
        post_handler["getMessage"] =
            &WXHandler::handleGetMessage;  // 获取消息，（好友消息，群消息，添加好友，群聊申请）
        post_handler["getRecentMessage"] = &WXHandler::handleGetRecentMessage;  // 获取最近消息
        post_handler["getHistoryMessage"] = &WXHandler::handleGetHistoryMessage;  // 获取历史消息
        post_handler["bindWeChat"] = &WXHandler::handleBindWeChat;  // 绑定微信
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
        try {
            if (req.path == "/WX/api") {
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
        } catch (std::exception &e) {
            LOG_ERROR(e.what());
            return makeJson(500, {{"error", e.what()}});
        }
        // return makeJson(404, {{"error", "Not Found"}});
        return default_handler->handle(req);
    }
    HTTPResponse handleInviteGroup(const json &data, int user_id) {
        if (user_id == -1) return makeJson(403, {{"error", "Not Logged In"}});
        if (data.find("group_id") == data.end() || data.find("friend_id") == data.end()) {
            return makeJson(400, {{"error", "Bad Request"}});
        }
        auto group_id = data["group_id"].get<int>();
        auto friend_id = data["friend_id"].get<int>();  // 申请加入的用户
        auto res = selectGroupById(group_id);
        if (!res->next()) {
            return makeJson(404, {{"error", "Group Not Found"}});
        }
        res->close();
        res = selectGroupMember(group_id, user_id);
        if (!res->next() ) {
            return makeJson(403, {{"error", "Permission Denied"}});
        }
        res->close();  // 这个地方是邀请添加了
        res = selectGroupMember(group_id, friend_id);
        if (res->next()) {
            return makeJson(200, {{"error", "User Already In Group"}});
        }
        res->close();
        if (!insertGroupMember(group_id, friend_id, "invited")) {
            return makeJson(500, {{"error", "Internal Server Error"}});
        }
        return makeJson(200, {{"message", "Invite Group Success"}});
    }
    HTTPResponse handleBindWeChat(const json &data, int user_id) {
        if (user_id == -1) return makeJson(403, {{"error", "Not Logged In"}});
        if (data.find("wechat_id") == data.end()) {
            return makeJson(400, {{"error", "Bad Request"}});
        }
        auto wechat_id = data["wechat_id"].get<int>();
        if (!updateBind(user_id, wechat_id)) {
            return makeJson(500, {{"error", "Internal Server Error"}});
        }
        return makeJson(200, {{"message", "Bind WeChat Success"}});
    }
    HTTPResponse handleUpdateProfile(const json &data, int user_id) {
        if (user_id == -1) return makeJson(403, {{"error", "Not Logged In"}});
        if (data.find("username") == data.end() || data.find("location") == data.end() || data.find("birthdate") == data.end() || data.find("location") == data.end()) {
            return makeJson(400, {{"error", "Bad Request"}});
        }
        auto nickname = data["username"].get<string>();
        auto location = data["location"].get<string>();
        auto birthdate = data["birthdate"].get<string>();
        auto res = selectUserById(user_id, "WeChat");
        if (!res->next()) {
            return makeJson(404, {{"error", "User Not Found"}});
        }
        res->close();
        if (!updateUser(user_id, nickname, birthdate,nickname)) {
            return makeJson(500, {{"error", "Internal Server Error"}});
        }
        return makeJson(200, {{"message", "Update Profile Success"}});
    }
    HTTPResponse handleExitGroup(const json &data, int user_id) {
        if (user_id == -1) return makeJson(403, {{"error", "Not Logged In"}});
        if (data.find("group_id") == data.end()) {
            return makeJson(400, {{"error", "Bad Request"}});
        }
        auto group_id = data["group_id"].get<int>();
        auto res = selectGroupById(group_id);
        if (!res->next()) {
            return makeJson(404, {{"error", "Group Not Found"}});
        }
        res->close();
        if (!deleteGroupMember(group_id, user_id)) {
            return makeJson(500, {{"error", "Internal Server Error"}});
        }
        return makeJson(200, {{"message", "Exit Group Success"}});
    }
    HTTPResponse handleAcceptGroup(const json &data, int user_id) {
        if (user_id == -1) return makeJson(403, {{"error", "Not Logged In"}});
        if (data.find("group_id") == data.end()) {
            return makeJson(400, {{"error", "Bad Request"}});
        }
        auto group_id = data["group_id"].get<int>();
        auto res = selectGroupById(group_id);
        if (!res->next()) {
            return makeJson(404, {{"error", "Group Not Found"}});
        }
        res->close();
        if (!updateGroupMember(group_id, user_id, "member")) {
            return makeJson(500, {{"error", "Internal Server Error"}});
        }
        return makeJson(200, {{"message", "Accept Group Success"}});
    }
    HTTPResponse handleGetGroup(const json &data, int user_id) {
        if (user_id == -1) return makeJson(403, {{"error", "Not Logged In"}});
        if(data.find("group_id") == data.end()) {
            return makeJson(400, {{"error", "Bad Request"}});
        }
        auto group_id = data["group_id"].get<int>();
        auto res = selectGroupMemberByGroupId(group_id);
        auto group_members = json::array();
        while (res->next()) {
            json member;
            member["id"] = res->getInt("id");
            member["nickname"] = res->getString("nickname");
            member["role"] = res->getString("role");
            group_members.push_back(member);
        }
        res->close();
        return makeJson(200, group_members);
    }
    HTTPResponse handleGetRecentMessage(const json &data, int user_id) {
        if (user_id == -1) return makeJson(403, {{"error", "Not Logged In"}});
        auto res = selectLatestMessage(user_id);
        auto messages = json::array();
        while (res->next()) {
            json msg;
            msg["message_id"] = res->getInt("id");
            msg["sender_id"] = res->getInt("sender_id");
            msg["receiver_id"] = res->getInt("receiver_id");
            msg["group_id"] = res->getInt("group_id");
            msg["content"] = res->getString("content");
            msg["sent_at"] = res->getString("sent_at");
            msg["message_type"] = res->getString("message_type");
            msg["user_role"] = res->getString("user_role");
            msg["name"] = res->getString("name");
            messages.push_back(msg);
        }
        res->close();
        return makeJson(200, messages);
    }
    HTTPResponse handleGetHistoryMessage(const json &data, int user_id) {
        if (user_id == -1) return makeJson(403, {{"error", "Not Logged In"}});
        if (data.find("friend_id") != data.end()) {
            auto friend_id = data["friend_id"].get<int>();
            auto res = selectHistoryPrivateMessageById(user_id, friend_id);
            auto messages = json::array();
            while (res->next()) {
                json msg;
                msg["message_id"] = res->getInt("id");
                msg["sender_id"] = res->getInt("sender_id");
                msg["receiver_id"] = res->getInt("receiver_id");
                msg["content"] = res->getString("content");
                msg["sent_at"] = res->getString("sent_at");
                msg["message_type"] = res->getString("message_type");
                messages.push_back(msg);
            }
            res->close();
            return makeJson(200, messages);
        } else if (data.find("group_id") != data.end()) {
            auto group_id = data["group_id"].get<int>();
            auto res = selectHistoryGroupMessageById(user_id, group_id);
            auto messages = json::array();
            while (res->next()) {
                json msg;
                msg["message_id"] = res->getInt("id");
                msg["sender_id"] = res->getInt("sender_id");
                msg["group_id"] = res->getInt("group_id");
                msg["content"] = res->getString("content");
                msg["sent_at"] = res->getString("sent_at");
                msg["message_type"] = res->getString("message_type");
                messages.push_back(msg);
            }
            res->close();
            return makeJson(200, messages);
        }
        return makeJson(400, {{"error", "Bad Request"}});
    }
    HTTPResponse handleLogout(const json &data, int user_id) {
        if (user_id == -1) return makeJson(403, {{"error", "Not Logged In"}});
        auto response = makeJson(200, {{"message", "Logout Success"}});
        response.headers["Set-Cookie"] = "session=; Max-Age=0";
        return response;
    }
    HTTPResponse handleGetInfo(const json &data, int user_id) {
        if (user_id == -1) return makeJson(403, {{"error", "Not Logged In"}});
        auto user = json::object();
        auto friends = json::array();
        auto groups = json::array();
        auto res = selectUserById(user_id, "WeChat");
        if (res->next()) {
            user["id"] = res->getInt("id");
            user["username"] = res->getString("nickname");
            user["birthdate"] = res->getString("birthdate");
            user["created_at"] = res->getString("created_at");
            user["location"] = res->getString("location");
            user["bind_id"] = res->getString("bind_id");
        }
        res->close();
        auto res2 = selectFriends(user_id, "WeChat");
        while (res2->next()) {
            json friend_;
            friend_["id"] = res2->getInt("id");
            friend_["username"] = res2->getString("nickname");
            friend_["birthdate"] = res2->getString("birthdate");
            friend_["created_at"] = res2->getString("created_at");
            friend_["location"] = res2->getString("location");
            friend_["bind_id"] = res2->getString("bind_id");
            friends.push_back(friend_);
        }
        res2->close();
        auto res3 = selectGroupByUserId(user_id);
        while (res3->next()) {
            json group;
            group["id"] = res3->getInt("id");
            group["group_name"] = res3->getString("group_name");
            groups.push_back(group);
        }
        res3->close();
        return makeJson(200, {{"user", user}, {"friends", friends}, {"groups", groups}});
    }
    HTTPResponse handleMarkRead(const json &data, int user_id) {
        if (user_id == -1) return makeJson(403, {{"error", "Not Logged In"}});
        if (data.find("message_id") == data.end()) {
            return makeJson(400, {{"error", "Bad Request"}});
        }
        auto message_id = data["message_id"].get<int>();
        if (!updateMessageRead(message_id, user_id)) {
            return makeJson(500, {{"error", "Internal Server Error"}});
        }
        return makeJson(200, {{"message", "Mark Read Success"}});
    }
    HTTPResponse handleGetMessage(const json &data, int user_id) {  //
        // （好友消息，群消息，添加好友，群聊申请）
        if (user_id == -1) return makeJson(403, {{"error", "Not Logged In"}});
        json message = json::array();  //
        auto res = selectUnreadPrivateMessage(user_id);
        while (res->next()) {  // 未读好友消息
            json msg;
            msg["message_id"] = res->getInt("id");
            msg["sender_id"] = res->getInt("sender_id");
            msg["receiver_id"] = res->getInt("receiver_id");
            msg["content"] = res->getString("content");
            msg["sent_at"] = res->getString("sent_at");
            msg["message_type"] = res->getString("message_type");
            message.push_back(msg);
        }
        res->close();
        res = selectUnreadGroupMessage(user_id);
        while (res->next()) {  // 未读群消息
            json msg;
            msg["message_id"] = res->getInt("id");
            msg["sender_id"] = res->getInt("sender_id");
            msg["group_id"] = res->getInt("group_id");
            msg["content"] = res->getString("content");
            msg["sent_at"] = res->getString("sent_at");
            msg["message_type"] = res->getString("message_type");
            message.push_back(msg);
        }
        res->close();
        json notice = json::array();
        res = selectPendingFriend(user_id);
        while (res->next()) {  // 添加好友
            json msg;
            msg["user_id"] = res->getInt("id");
            msg["username"] = res->getString("nickname");
            msg["created_at"] = res->getString("ap_time");
            msg["type"] = "friend";
            notice.push_back(msg);
        }
        res->close();
        res = selectPendingGroup(user_id);
        while (res->next()) {  // 添加群
            json msg;
            msg["group_id"] = res->getInt("group_id");
            msg["user_id"] = res->getInt("user_id");
            msg["created_at"] = res->getString("joined_at");
            msg["username"] = res->getString("nickname");
            msg["type"] = "admin_group";
            notice.push_back(msg);
        }
        res->close();
        res = selectInvitedGroup(user_id);
        while (res->next()) {  // 邀请加入群
            json msg;
            msg["group_id"] = res->getInt("group_id");
            msg["user_id"] = res->getInt("user_id");
            msg["created_at"] = res->getString("joined_at");
            msg["type"] = "invite";
            notice.push_back(msg);
        }
        res->close();
        auto r_data = json::object();
        r_data["message"] = message;
        r_data["notice"] = notice;
        return makeJson(200, r_data);
    }
    HTTPResponse handleRegister(const json &data, int user_id) {
        if (user_id != -1) return makeJson(403, {{"error", "Already Logged In"}});
        if (data.find("username") == data.end() || data.find("password") == data.end()) {
            return makeJson(400, {{"error", "Bad Request"}});
        }
        auto username = data["username"].get<string>();
        auto password = data["password"].get<string>();
        auto db = SQL::getInstance();
        if (insertUser(username, password, "WeChat")) {
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
        auto res = selectUserByNamePassword(username, password, "WeChat");
        if (res->next()) {
            auto user_id = res->getInt("id");
            auto session = UUID(4);
            SET(session, std::to_string(user_id));
            auto response = makeJson(200, {{"message", "Login Success"}});
            response.headers["Set-Cookie"] = "session=" + session;
            res->close();
            return response;
        } else {
            res->close();
            return makeJson(401, {{"error", "Username or Password Incorrect"}});
        }
    }
    HTTPResponse handleAddFriend(const json &data, int user_id) {  // 插入数据库
        if (user_id == -1) return makeJson(403, {{"error", "Not Logged In"}});
        if (data.find("friend_id") == data.end()) {
            return makeJson(400, {{"error", "Bad Request"}});
        }
        auto friend_id = data["friend_id"].get<int>();
        auto res = selectUserById(friend_id, "WeChat");
        if (!res->next()) {  // 在WX上有这个用户
            res->close();
            return makeJson(404, {{"error", "User Not Found"}});
        }
        res->close();
        res = selectFriend(user_id, friend_id);
        if (res->next()) {
            res->close();
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
        auto res = selectUserById(friend_id, "WeChat");
        if (!res->next()) {  // 在WX上有这个用户
            res->close();
            return makeJson(404, {{"error", "User Not Found"}});
        }
        if (!updateFriend(friend_id, user_id)) {  // 正向添加了
            return makeJson(500, {{"error", "update failed"}});
        }
        if (insertFriend(user_id, friend_id, "active")) {  // 反向添加
            insertPrivateMessage(user_id, friend_id, "已添加好友");
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
        res->close();
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
        if (!createGroup(user_id, group_name, "WeChat")) {
            return makeJson(500, {{"error", "Internal Server Error"}});
        }
        auto g = selectGroupByCreator(user_id, group_name);
        if (g->next()) {
            auto group_id = g->getInt("id");
            if (insertGroupMember(group_id, user_id, "owner")) {
                return makeJson(200, {{"msg", "Create Group Success", "group_id", group_id}});
            }
        }
        g->close();
        return makeJson(500, {{"error", "Internal Server Error"}});
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
        res->close();
        res = selectGroupMember(group_id, user_id);
        if (!res->next() || res->getString("role") != "owner") {
            return makeJson(403, {{"error", "Permission Denied"}});
        }
        res->close();
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
        res->close();
        res = selectGroupMember(group_id, user_id);
        if (!res->next() ||
            (res->getString("role") != "owner") {
            return makeJson(403, {{"error", "Permission Denied"}});
        }
        res->close();
        res = selectGroupMember(group_id, app_user_id);
        if (!res->next()) {
            return makeJson(404, {{"error", "User Not Found"}});
        }
        res->close();
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
        auto res = searchUser(username, "WeChat");
        auto result = json::array();
        while (res->next()) {
            result.push_back({{"id", res->getInt("id")}, {"username", res->getString("nickname")}});
        }
        res->close();
        return makeJson(200, {{"message", "Search Friend Success"}, {"result", result}});
    }
    HTTPResponse handleSearchGroup(const json &data, int user_id) {
        if (user_id == -1) return makeJson(403, {{"error", "Not Logged In"}});
        if (data.find("group_name") == data.end()) {
            return makeJson(400, {{"error", "Bad Request"}});
        }
        auto group_name = data["group_name"].get<string>();
        auto res = searchGroup(group_name, "WeChat");
        auto result = json::array();
        while (res->next()) {
            result.push_back(
                {{"id", res->getInt("id")}, {"group_name", res->getString("group_name")}});
        }
        res->close();
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
        res->close();
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
        res->close();
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
        res->close();
        res = selectGroupMember(group_id, user_id);
        if (!res->next()) {
            return makeJson(403, {{"error", "Permission Denied"}});
        }
        res->close();
        if (!insertGroupMember(group_id, app_user_id, "invited")) {
            return makeJson(500, {{"error", "Internal Server Error"}});
        }
        return makeJson(200, {{"message", "Add Group Member Success"}});
    }

   private:
    map<string, HTTPResponse (WXHandler::*)(const json &, int)> post_handler;
    map<string, HTTPResponse (WXHandler::*)(const json &, int)> get_handler;
};
#endif