#ifndef _JSON_HANDLER_H_
#define _JSON_HANDLER_H_
#include "base.h"
class JsonHandler: public BaseHandler{
    public:
    HTTPResponse handle(HTTPRequest &request) override {
        HTTPResponse res;
        res.headers["Content-Type"]="application/json";
        res.body="{\"status\":\"ok\"}";
        res.version = request.version;
        res.status_code = "200";
        return res;
    }

};

#endif