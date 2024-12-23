#ifndef _API_H
#define _API_H
#pragma once
#include "../http/file.h"
#include "../http/handler.h"
#include "../http/json.h"
#include "qq/user.h"
class QQHandler : public Handler {
   public:
    QQHandler() {
        add_handle("/qq/api/user", new QQUserHandler());
        default_handle(new FileHandler());
    }
};
#endif