#ifndef _QQ_FRIEND_API_H
#define _QQ_FRIEND_API_H

#include "../../dao/user.h"
#include "../../http/handler.h"
#include "../../utils/log.h"
#include "../../utils/map.h"
#include "../../utils/uuid.h"
class QQFriendHandler : public Handler {
   public:
    virtual HTTPResponse handle(const HTTPRequest &req) override {
        if (req.method == "POST") {
        } else if (req.method == "GET") {
        }
        return makeJson(404, {{"error", "Not Found"}});
    }
}

#endif