#include"httphandler.h"
#include"../utils/log.h"
void HTTPHandlers::handle(const char *buffer,int fd) {
    HTTPRequest req;
    if(!bufferToRequest(buffer,req)) {
        LOG_ERROR("Failed to parse request");
        HTTPResponse response;
        char *res_buffer = new char[1024];
        strcpy(res_buffer,"HTTP/1.1 500 Internal Server Error\r\nContent-Type: text/html\r\n\r\n500 Internal Server Error");
        send(fd,res_buffer,strlen(res_buffer),0);
        delete[] res_buffer;
        return;
    }
    LOG_INFO("Request:" , req.method , req.path , req.version);
    HTTPResponse response;
    int called = 0;
    for(auto it = handlers.begin();it != handlers.end();it++) {
        if(req.path.substr(0,it->first.size()) == it->first) {
            LOG_DEBUG("Hit handler at path " , it->first);
            response = it->second->handle(req);
            called = 1;
            break;
        }
    }
    if(!called ) {
        LOG_DEBUG("Hit default handler");
        response = default_handler->handle(req);
    }
    LOG_INFO("Response:" , std::to_string(response.status_code), response.status);
    char *res_buffer = nullptr;
    int buffer_len = 0;
    if(responseToBuffer(response,res_buffer,buffer_len) == -1){
        LOG_ERROR("Failed to create response");
        delete [] res_buffer;
        res_buffer = new char[1024];
        strcpy(res_buffer,"HTTP/1.1 500 Internal Server Error\r\nContent-Type: text/html\r\n\r\n500 Internal Server Error");
    }
    send(fd,res_buffer,buffer_len,0);
    delete[] res_buffer;
}

void HTTPHandlers::add_handle(string path,BaseHandler *handler) {
    LOG_INFO("Add handler at path:" , path);
    handlers[path] = handler;
}

void HTTPHandlers::default_handle(BaseHandler *handler) {
    default_handler = handler;
}