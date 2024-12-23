#ifndef _QQ_USER_API_H
#define _QQ_USER_API_H
#include "../../dao/user.h"
#include "../../http/handler.h"
#include "../../utils/log.h"
#include "../../utils/map.h"
#include "../../utils/uuid.h"
class QQUserHandler : public Handler {
   public:
    virtual HTTPResponse handle(const HTTPRequest &req) override {
        if (req.method == "POST") {
            if (req.path == "/qq/api/user/login") {
                if (req.data.find("user_id") == req.data.end() ||
                    req.data.find("password") == req.data.end()) {
                    return makeJson(400, {{"msg", "Missing username or password"}});
                }
                // auto username = req.data["userid"].get<std::string>();
                auto user_id = req.data["user_id"].get<int>();
                auto password = req.data["password"].get<std::string>();
                auto user = Login(user_id, password, "QQ");
                if (user.getId() == 0) {
                    return makeJson(401, {{"msg", "Invalid username or password"}});
                }
                auto cookie = UUID(4);  // as string
                SET(cookie, std::to_string(user.getId()));
                auto response = makeJson(200, to_json(user));
                response.headers["Set-Cookie"] = "session=" + cookie + "; Path=/; HttpOnly";
                return response;
            } else if (req.path == "/qq/api/user/register") {
                if (req.data.find("nickname") == req.data.end() ||
                    req.data.find("password") == req.data.end()) {
                    return makeJson(400, {{"msg", "Missing nickname or password"}});
                }
                auto nickname = req.data["nickname"].get<std::string>();
                auto password = req.data["password"].get<std::string>();
                auto user = Register(nickname, password, "QQ");
                if (user.getId() == 0) {
                    return makeJson(400, {{"msg", "User already exists"}});
                }
                auto cookie = UUID(4);  // as string
                SET(cookie, std::to_string(user.getId()));
                auto response = makeJson(200, to_json(user));
                response.headers["Set-Cookie"] = "session=" + cookie + "; Path=/; HttpOnly";
                return response;
            } else if (req.path == "/qq/api/user/logout") {
                auto response = makeJson(200, {{"msg", "Logout successfully"}});
                DEL(req.cookies["session"]);
                response.headers["Set-Cookie"] = "session=; Path=/; HttpOnly; Max-Age=0";
                return response;
            }
        } else if (req.method == "GET") {
            if (req.path == "/qq/api/user/search") {
                if (req.query.find("nickname") == req.query.end()) {
                    return makeJson(400, {{"msg", "Missing nickname"}});
                }
                auto nickname = req.query["nickname"];
                // auto users = json(Search(nickname, "QQ"));
                // json users(Search(nickname, "QQ"));
                auto users = Search(nickname, "QQ");
                json j = json::array();
                for (auto u : users) {
                    j.push_back(to_json(u));
                }
                return makeJson(200, j);
            }
        }
        return makeJson(404, {{"msg", "Not found"}});
    }
};
#endif