#ifndef _JSON_HANDLER_H_
#define _JSON_HANDLER_H_
#include "base.h"
#include<map>
HTTPResponse makeJson(int code,json data) {
    return makeResponse(code,data.dump(),"application/json");
}
class JsonHandler: public BaseHandler{
public:
    virtual HTTPResponse handle(HTTPRequest &request) override {
        return makeJson(200,json::object());
    }
    JsonHandler() {}
    virtual ~JsonHandler() {}
    virtual void add_handle(string path,BaseHandler *handler) {}
private:
    map<string,JsonHandler *> handlers;
    JsonHandler *default_handler;
};

#endif