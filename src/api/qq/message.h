#pragma once
#include "../../dao/qq/chat.h"
#include "../../dao/qq/message.h"
#include "../../dao/qq/user.h"
#include "../../http/handler.h"
#include "../../utils/log.h"
namespace qq {
class ChatHandler : public Handler {
   public:
    HTTPResponse handle(const HTTPRequest &request) override {
        if (request.cookies.find("user_id") == request.cookies.end()) {
            LOG_DEBUG(request.cookies.dump());
            return makeJson(401, json{{"error", "Unauthorized"}});
        }
        if (request.method == "POST") {
            if (request.path == "/api/qq/chats") {  // POST /api/qq/chat => create chat
                auto body = request.data;
                if (body.find("type") == body.end() || body.find("title") == body.end() ||
                    body.find("description") == body.end() ||
                    body.find("creator_id") == body.end()) {
                    return makeJson(400, json({{"error", "Bad Request"}}));
                }
                auto chat = Chat(0, body["type"], body["title"], body["description"], "",
                                 body["creator_id"]);
                auto new_chat = insertChat(chat);
                return makeJson(201, new_chat.to_json());
            }
            if (request.path == "/api/qq/chats/members") {
                auto body = request.data;
                if (body.find("chat_id") == body.end() || body.find("user_id") == body.end()) {
                    return makeJson(400, json({{"error", "Bad Request"}}));
                }
                auto chat_id = body["chat_id"];
                auto user_id = body["user_id"];
                string role = "member";
                // auto role = body["role"];
                if (body.find("role") != body.end()) {
                    role = body["role"].get<std::string>();
                }
                if (insertChatMember(chat_id, user_id, role)) {
                    return makeJson(201, json({{"message", "Add member successfully"}}));
                }
                return makeJson(500, json({{"error", "Internal Server Error"}}));
            }
        } else if (request.method == "DELETE") {
            if (request.path == "/api/qq/chats/members") {
                auto body = request.data;
                if (body.find("chat_id") == body.end() || body.find("user_id") == body.end()) {
                    return makeJson(400, json({{"error", "Bad Request"}}));
                }
                auto chat_id = body["chat_id"];
                auto user_id = body["user_id"];
                if (deleteChatMember(chat_id, user_id)) {
                    return makeJson(200, json({{"message", "Delete member successfully"}}));
                }
                return makeJson(500, json({{"error", "Internal Server Error"}}));
            }
        } else if (request.method == "PUT") {
            if (request.path == "/api/qq/chats") {
                auto body = request.data;
                if (body.find("id") == body.end() || body.find("title") == body.end() ||
                    body.find("description") == body.end()) {
                    return makeJson(400, json({{"error", "Bad Request"}}));
                }
                auto chat = selectChatById(body["id"]);
                if (chat.getId() == 0) {
                    return makeJson(404, json({{"error", "Not Found"}}));
                }
                chat.setTitle(body["title"]);
                chat.setDescription(body["description"]);
                if (updateChat(chat)) {
                    return makeJson(200, json({{"message", "Update chat successfully"}}));
                }
                return makeJson(500, json({{"error", "Internal Server Error"}}));
            }
            if (request.path == "/api/qq/chats/members") {  // 修改权限
                auto body = request.data;
                if (body.find("chat_id") == body.end() || body.find("user_id") == body.end() ||
                    body.find("role") == body.end()) {
                    return makeJson(400, json({{"error", "Bad Request"}}));
                }
                auto chat_id = body["chat_id"];
                auto user_id = body["user_id"];
                auto role = body["role"];
                if (updateChatMember(chat_id, user_id, role)) {
                    return makeJson(200, json({{"message", "Update member successfully"}}));
                }
                return makeJson(500, json({{"error", "Internal Server Error"}}));
            }
        } else if (request.method == "GET") {
            if (request.path == "/api/qq/chats/members") {
                if (request.query.find("chat_id") != request.query.end()) {
                    LOG_DEBUG(request.query["chat_id"]);
                    auto chat_id = std::stoi(request.query["chat_id"].get<std::string>());
                    auto members = selectChatMember(chat_id);
                    json response = json::array();

                    for (auto member : members) {
                        LOG_DEBUG(member.to_json());
                        response.push_back(member.to_json());
                    }
                    return makeJson(200, response);
                }
                return makeJson(400, json({{"error", "Bad Request"}}));
            }
        }
        return makeJson(405, json({{"error", "Method Not Allowed"}}));
    }
};

class MessageHandler : public Handler {
   public:
    HTTPResponse handle(const HTTPRequest &request) override {
        if (request.cookies.find("user_id") == request.cookies.end()) {
            LOG_DEBUG(request.cookies.dump());
            return makeJson(401, json{{"error", "Unauthorized"}});
        }
        auto user_id = std::stoi(request.cookies["user_id"].get<std::string>());
        if (request.method == "POST") {
            if (request.path == "/api/qq/messages") {
                auto body = request.data;
                if (body.find("chat_id") == body.end() || body.find("content") == body.end()) {
                    return makeJson(400, json{{"error", "Bad Request"}});
                }
                auto chat_members = selectChatMember(body["chat_id"]);
                bool is_member = false;
                for (auto member : chat_members) {
                    if (member.getUserId() == user_id) {
                        is_member = true;
                        break;
                    }
                }
                if (!is_member) {
                    return makeJson(403, json{{"error", "Forbidden"}});
                }
                auto message = Message(0, user_id, body["chat_id"], "text", body["content"].get<std::string>(),
                                       "", false);
                auto chat = selectChatById(body["chat_id"]);
                if (chat.getId() == 0) {
                    return makeJson(404, json{{"error", "Not Found"}});
                }
                if (body.find("type") != body.end()) {
                    message.setType(body["type"]);
                }
                auto new_message = insertMessage(message);
                // return makeJson(201, new_message.to_json());
                return makeJson(201, json{"message", "Send message successfully"});
            } else if (request.path == "/api/qq/messages/status") {
                auto body = request.data;
                if (body.find("message_id") == body.end()) {
                    return makeJson(400, json{{"error", "Bad Request"}});
                }
                auto message_id = body["message_id"];
                if (updateMessageStatus(message_id, user_id)) {
                    return makeJson(200, json{{"message", "Update message status successfully"}});
                }
                return makeJson(500, json{{"error", "Internal Server Error"}});
            }
        } else if (request.method == "GET") {
            if (request.path == "/api/qq/messages") {
                if (request.query.find("chat_id") != request.query.end()) {
                    auto chat_id = std::stoi(request.query["chat_id"].get<std::string>());
                    auto chat_members = selectChatMember(chat_id);
                    bool is_member = false;
                    for (auto member : chat_members) {
                        LOG_DEBUG(member.to_json());
                        LOG_DEBUG(user_id);
                        if (member.getUserId() == user_id) {
                            is_member = true;
                            break;
                        }
                    }
                    if (!is_member) {
                        return makeJson(403, json{{"error", "Forbidden"}});
                    }
                    auto messages = selectMessage(chat_id);
                    json response = json::array();
                    for (auto message : messages) {
                        response.push_back(message.to_json());
                    }
                    return makeJson(200, response);
                }
                return makeJson(400, json{{"error", "Bad Request"}});
            }
        }else if(request.method == "DELETE"){
            if(request.path == "/api/qq/messages"){
                auto body = request.data;
                if(body.find("message_id") == body.end()){
                    return makeJson(400, json{{"error", "Bad Request"}});
                }
                auto message_id = body["message_id"];
                if(deleteMessage(message_id)){
                    return makeJson(200, json{{"message", "Delete message successfully"}});
                }
                return makeJson(500, json{{"error", "Internal Server Error"}});
            }
        }
        return makeJson(405, json{{"error", "Method Not Allowed"}});
    }
};
}  // namespace qq