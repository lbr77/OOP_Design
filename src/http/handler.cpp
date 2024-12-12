#include "handler.h"

#include "../utils/log.h"

HTTPResponse Handler::handle(const HTTPRequest &request) {
    // return makeResponse(404,"Not Found","text/html");
    for (auto it = handlers.begin(); it != handlers.end(); it++) {
        if (request.path.substr(0, it->first.size()) == it->first) {
            LOG_DEBUG("Hit path", it->first);
            return it->second->handle(request);
        }
    }
    LOG_DEBUG("Fallback handler.");
    return default_handler->handle(request);
}

void Handler::add_handle(string path, Handler *handler) {
    LOG_INFO("Add handler at path:", path);
    handlers[path] = handler;
}

void Handler::default_handle(Handler *handler) {
    default_handler = handler;
}