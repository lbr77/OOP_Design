#pragma once
#include "../../dao/qq/user.h"
#include "../../http/handler.h"

namespace qq {
class RegisterHandler : public Handler {
   public:
    HTTPResponse handle(const HTTPRequest &request) override {
        if (request.method == "POST") {
            json body = request.data;
            if (body.find("username") == body.end() || body.find("password") == body.end()) {
                return makeJson(400, json{{"error", "Bad Request"}});
            }
            User user = User(0, body["username"], body["password"], "", "2000-01-01", "China");
            user = insertUser(user);
            return makeJson(200, user.to_json());
        }
        return makeJson(405, json{{"error", "Method Not Allowed"}});
    }
};
class LoginHandler : public Handler {
   public:
    HTTPResponse handle(const HTTPRequest &request) override {
        if (request.method == "POST") {
            json body = request.data;
            if (body.find("username") == body.end() || body.find("password") == body.end()) {
                return makeJson(400, json{{"error", "Bad Request"}});
            }
            User user = selectUserByUsername(body["username"]);
            if (user.getId() == 0) {
                return makeJson(404, json{{"error", "Not Found"}});
            }
            if (user.getPassword() != body["password"]) {
                return makeJson(401, json{{"error", "Unauthorized"}});
            }
            auto response = makeJson(200, user.to_json());
            response.headers["Set-Cookie"] = "user_id=" + std::to_string(user.getId()) + "; Path=/";
            return response;
        }
        return makeJson(405, json{{"error", "Method Not Allowed"}});
    }
};
class UserInfoHandler : public Handler {
   public:
    HTTPResponse handle(const HTTPRequest &request) override {
        if (request.method == "GET") {
            std::string path = request.path;
            std::string user_id = path.substr(path.find_last_of("/") + 1);
            if (user_id == "") {
                if (request.cookies.find("user_id") == request.cookies.end()) {
                    return makeJson(401, json{{"error", "Unauthorized"}});
                }
                int current_user_id = std::stoi(request.cookies["user_id"].get<std::string>());
                User user = selectUserById(current_user_id);
                if (user.getId() == 0) {
                    return makeJson(404, json{{"error", "Not Found"}});
                }
                return makeJson(200, user.to_json());
            }
            User user = selectUserById(std::stoi(user_id));
            if (user.getId() == 0) {
                return makeJson(404, json{{"error", "Not Found"}});
            }
            return makeJson(200, user.to_json());
        } else if (request.method == "PUT") {
            std::string path = request.path;
            std::string user_id = path.substr(path.find_last_of("/") + 1);
            User user = selectUserById(std::stoi(user_id));
            if (user.getId() == 0) {
                return makeJson(404, json{{"error", "Not Found"}});
            }
            json body = request.data;
            if (body.find("username") != body.end()) {
                user.setUsername(body["username"]);
            }
            if (body.find("password") != body.end()) {
                user.setPassword(body["password"]);
            }
            if (body.find("birthday") != body.end()) {
                user.setBirthday(body["birthday"]);
            }
            if (body.find("address") != body.end()) {
                user.setAddress(body["address"]);
            }
            if (updateUser(user)) {
                return makeJson(200, user.to_json());
            }
            return makeJson(500, json{{"error", "Internal Server Error"}});
        }
        return makeJson(405, json{{"error", "Method Not Allowed"}});
    }
};
}  // namespace qq