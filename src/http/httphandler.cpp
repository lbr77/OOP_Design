#include"httphandler.h"

int HTTPHandlers::bufferToRequest(char *buffer,HTTPRequest &req) {
    std::string str(buffer);
    std::string delimiter = "\r\n";
    size_t pos = 0;
    std::string token;
    std::vector<std::string> lines;
    while ((pos = str.find(delimiter)) != std::string::npos) {
        token = str.substr(0, pos);
        lines.push_back(token);
        str.erase(0, pos + delimiter.length());
    }
    req.method = lines[0].substr(0,lines[0].find(' '));
    req.path = lines[0].substr(lines[0].find(' ')+1,lines[0].rfind(' ')-lines[0].find(' ')-1);
    req.version = lines[0].substr(lines[0].rfind(' ')+1);
    for(int i=1;i<lines.size();i++){
        if(lines[i].find(':') == std::string::npos) {
            continue;
        }
        std::string key = lines[i].substr(0,lines[i].find(':'));
        std::string value = lines[i].substr(lines[i].find(':')+2);
        req.headers[key] = value;
    }
    
    req.body = str;
    return 0;
}
int HTTPHandlers::responseToBuffer(HTTPResponse &res,char *buffer) {
    std::string res_str = res.version + " " + res.status + "\r\n";
    for(auto it = res.headers.begin();it != res.headers.end();it++){
        res_str += it.key() + ": " + it.value().get<std::string>() + "\r\n";
    }
    res_str += "\r\n" + res.body;
    strcpy(buffer,res_str.c_str());
    return 0;
}
void HTTPHandlers::handle(char *buffer,int fd) {
    HTTPRequest req;
    if(bufferToRequest(buffer,req) == -1) {
        HTTPResponse response;
        char res_buffer[1024];
        std::cout<<response.status<<std::endl;
        responseToBuffer(response,res_buffer);
        send(fd,res_buffer,strlen(res_buffer),0);
    }
    HTTPResponse response;
    std::cout << req.method << " " << req.path << " " << req.version << std::endl;
    int called = 0;
    for(auto it = handlers.begin();it != handlers.end();it++) {
        // std::cout<<req.path.substr(0,it->first.size())<<" "<<it->first<<std::endl;
        if(req.path.substr(0,it->first.size()) == it->first) {
            response = it->second->handle(req);
            called = 1;
            break;
        }
    }
    if(!called)response = default_handler->handle(req);
    char res_buffer[1024];
    std::cout<<response.status<<std::endl;
    responseToBuffer(response,res_buffer);
    send(fd,res_buffer,strlen(res_buffer),0);
    close(fd);
}

void HTTPHandlers::add_handle(string path,BaseHandler *handler) {
    std::cout<<"Added handler at path "<<path<<std::endl;
    handlers[path] = handler;
}

void HTTPHandlers::default_handle(BaseHandler *handler) {
    default_handler = handler;
}