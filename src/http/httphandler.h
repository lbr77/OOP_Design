#ifndef _HTTP_HANDLER_H_
#define _HTTP_HANDLER_H_
#include "../tcp/server.h"
#include<string>
#include<map>
#include"base.h"
using std::string;
using std::map;


class HTTPHandlers:public Handler {
public:
    HTTPHandlers(map<string,BaseHandler*> handlers):handlers(handlers) {}
    HTTPHandlers() {}
    ~HTTPHandlers() {
        for(auto it = handlers.begin();it != handlers.end();it++) {
            delete it->second;
        }
    }
    void handle(char *buffer,int fd);
    void add_handle(string path,BaseHandler *handler);
    void default_handle(BaseHandler *handler);
private:
    map<string,BaseHandler*> handlers; // path -> handler
    BaseHandler *default_handler;
    int bufferToRequest(char *buffer,HTTPRequest &request);
    int responseToBuffer(HTTPResponse &response,char *buffer);
};
#endif