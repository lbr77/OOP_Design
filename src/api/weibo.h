#ifndef _API_WB_H
#define _API_WB_H
#include "../db/interact.h"
#include "../http/file.h"
#include "../http/handler.h"
#include "../http/json.h"
#include "../utils/log.h"
#include "../utils/map.h"
#include "../utils/uuid.h"

class WeiboHandler : public Handler {
   public:
    WeiboHandler() {  // 微博的帖子什么的不做了（（没时间了
        post_handler["login"] = &WeiboHandler::handleLogin;
        // post_handler["logout"] = &WeiboHandler::handleLogout;
        // post_handler["post"] = &WeiboHandler::handlePost;
        // post_handler["delete"] = &WeiboHandler::handleDelete;
        // post_handler["update"] = &WeiboHandler::handleUpdate;
        // post_handler["comment"] = &WeiboHandler::handleComment;
        // post_handler["like"] = &WeiboHandler::handleLike;
        // post_handler["follow"] = &WeiboHandler::handleFollow;
        // post_handler["unfollow"] = &WeiboHandler::handleUnfollow;
        // post_handler["search"] = &WeiboHandler::handleSearch;
    }
    virtual HTTPResponse handle(const HTTPRequest &req) {
        int user_id = -1;
        if (req.cookies.find("qq_session") != req.cookies.end()) {
            auto session = req.cookies["session"];
            auto user_id_s = GET(session);
            if (user_id_s.has_value() && !user_id_s.value().empty()) {
                user_id = stoi(user_id_s.value());
            }
        }
        LOG_DEBUG("User ID:", user_id);
        if (req.path == "/wb/api") {
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
    HTTPResponse handleLogin(const json &data, int user_id) {
        if (user_id != -1) return makeJson(200, {{"error", "Already Logged In By QQ"}});
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

   private:
    map<string, HTTPResponse (WeiboHandler::*)(const json &, int)> post_handler;
    map<string, HTTPResponse (WeiboHandler::*)(const json &, int)> get_handler;
};

#endif