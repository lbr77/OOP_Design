#ifndef _API_H
#define _API_H
#pragma once
#include "../http/handler.h"
#include "../http/json.h"
#include "./qq/message.h"
#include "./qq/user.h"
// register all api path here.
class QQHandler : public Handler {
   public:
    QQHandler() {
        add_handle("/api/qq/user/register", new qq::RegisterHandler());
        add_handle("/api/qq/user/login", new qq::LoginHandler());
        add_handle("/api/qq/users/", new qq::UserInfoHandler());
        add_handle("/api/qq/chats", new qq::ChatHandler());
        add_handle("/api/qq/messages", new qq::MessageHandler());
        default_handle(new JsonHandler());
    }
};
#endif