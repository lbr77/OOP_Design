#include "http.h"
int parseRequest(const char *buffer,HTTPRequest &req){
    std::string str(buffer);
    size_t pos = str.find("\r\n");
    if(pos == std::string::npos) {
        return -1;
    }
    std::string request_line = str.substr(0,pos);
    str = str.substr(pos+2);
    std::vector<std::string> parts;
    pos = 0;
    while((pos = request_line.find(" ")) != std::string::npos) {
        parts.push_back(request_line.substr(0,pos));
        request_line = request_line.substr(pos+1);
    }
    parts.push_back(request_line);
    if(parts.size() != 3) {
        return -1;
    }
    req.method = parts[0];
    req.path = parts[1];
    req.version = parts[2];
    std::cout<<req.method<<" "<<req.path<<" "<<req.version<<std::endl;
    pos = str.find("\r\n\r\n");
    if(pos == std::string::npos) {
        return -1;
    }
    std::string headers = str.substr(0,pos);
    str = str.substr(pos+4);
    while((pos = headers.find("\r\n")) != std::string::npos) {
        std::string header = headers.substr(0,pos);
        headers = headers.substr(pos+2);
        size_t colon_pos = header.find(":");
        if(colon_pos == std::string::npos) {
            return -1;
        }
        std::string key = header.substr(0,colon_pos);
        std::string value = header.substr(colon_pos+2);
        req.headers[key] = value;
    }
    req.body = str;
    return 0;
}

int ResponseToBuffer(const HTTPResponse &res,char *buffer){
    std::string str = res.version + " " + res.status + "\r\n";
    for(auto &header:res.headers) {
        str += header.first + ": " + header.second + "\r\n";
    }
    str += "\r\n" + res.body;
    strcpy(buffer,str.c_str());
    return 0;
}

bool try_file(const std::string &path, std::ifstream &file) {
    if (!std::filesystem::exists(path)) {
        return false;
    }
    if (std::filesystem::is_directory(path)) {
        std::string index_path = path + "/index.html";
        if (std::filesystem::exists(index_path)) {
            file.clear();
            file.open(index_path, std::ios::in | std::ios::binary);
            return file.is_open();
        }
        return false;
    }
    
    if (std::filesystem::is_regular_file(path)) {
        file.open(path, std::ios::in | std::ios::binary);
        return file.is_open();
    }
    
    return false;
}

HTTPResponse handleRequest(const HTTPRequest &req){
    HTTPResponse res;
    res.version = req.version;
    res.status = "404 Not found";
    res.headers["Content-Type"] = "text/html";
    res.body = "404";
    if(req.path.substr(0,5) == "/api/") {
        // do sth with api;
        res.body = "API";
    } else {
        // try file
        std::ifstream file;
        if(try_file("public" + req.path,file)) {
            res.status = "200 OK";
            res.body = "";
            std::getline(file,res.body,'\0');
            file.close();
            res.headers["Content-Type"] = "text/html";
        }
    }
    return res;
}