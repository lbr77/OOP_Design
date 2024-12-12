#ifndef _JSON_HANDLER_H_
#define _JSON_HANDLER_H_
#include <map>
#pragma once
#include "handler.h"
HTTPResponse makeJson(int code, json data) {
    return makeResponse(code, data.dump(), "application/json");
}
class JsonHandler : public Handler {
   public:
    virtual HTTPResponse handle(const HTTPRequest &request) override {
        return makeJson(200, json::object());
    }
    JsonHandler() {}
    virtual ~JsonHandler() {}
    virtual void add_handle(string path, Handler *handler) {}

   private:
    map<string, Handler *> handlers;
    Handler *default_handler;
};

#endif