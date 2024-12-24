#ifndef _HTTP_HANDLER__H_
#define _HTTP_HANDLER__H_
#pragma once
// #include "../tcp/server.h"
#include <map>
#include <string>

#include "struct.h"
using std::map;
using std::string;

class Handler {
   public:
    Handler(map<string, Handler *> handlers) : handlers(handlers) {}
    Handler() {}
    ~Handler() {
        for (auto it = handlers.begin(); it != handlers.end(); it++) {
            delete it->second;
        }
    }
    virtual HTTPResponse handle(const HTTPRequest &request);
    void add_handle(string path, Handler *handler);
    void default_handle(Handler *handler);

   protected:
    map<string, Handler *> handlers;  // path -> handler
    Handler *default_handler;         // default is file server.
};
#endif