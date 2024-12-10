#include "struct.h"
#include<iostream>
HTTPResponse makeResponse(int code=200,std::string ctx="",std::string ctx_type = "text/html") {
    HTTPResponse res;
    res.version = "HTTP/1.1";
    res.status_code = code;
    res.status = status_code[std::to_string(res.status_code)].get<std::string>();
    res.headers["Content-Type"] = ctx_type;
    res.headers["Content-Length"] = std::to_string(ctx.size());
    res.body = ctx;
    return res;
}

bool bufferToRequest(const char *buffer,HTTPRequest &req){
/**
 * GET / HTTP/1.1
 * Key: Value
 */
    if(strlen(buffer) == 0) {
        return false;
    }
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
    if(lines.size() == 0) {
        return false;
    }
    if(lines[0].find(' ') == std::string::npos) { // one space
        return false;
    }
    if(lines[0].rfind(' ') == lines[0].find(' ')) { // two space 
        return false;
    }
    req.method = lines[0].substr(0,lines[0].find(' '));
    req.path = lines[0].substr(lines[0].find(' ')+1,lines[0].rfind(' ')-lines[0].find(' ')-1);
    req.version = lines[0].substr(lines[0].rfind(' ')+1);
    for(int i=1;i<lines.size();i++){
        if(lines[i].find(':') == std::string::npos) {
            continue;
        }
        std::string key = lines[i].substr(0,lines[i].find(':'));
        std::transform(key.begin(), key.end(), key.begin(), ::tolower);
        if(lines[i].find(":")+2 >= lines[i].size()) {
            req.headers[key] = "";
            continue;
        }
        std::string value = lines[i].substr(lines[i].find(':')+2);

        req.headers[key] = value;
    }
    req.body = str;
    if(req.headers.find("cookie") != req.headers.end()) { // 解析一下 cookie。。这个好像也有用
        std::string data = req.headers["cookie"];
        std::string delimiter = "; ";
        size_t pos = 0;
        std::string token;
        while ((pos = data.find(delimiter)) != std::string::npos) {
            token = data.substr(0, pos);
            std::string key = token.substr(0,token.find('='));
            std::string value = token.substr(token.find('=')+1);
            req.cookies[key] = value;
            data.erase(0, pos + delimiter.length());
        }
        if(data.size() > 0) {
            std::string key = data.substr(0,data.find('='));
            std::string value = data.substr(data.find('=')+1);
            req.cookies[key] = value;
        }
    }
    if(req.headers.find("content-type") != req.headers.end()) { // 如果content-type 是 json / x-www-form-urlencoded，同样解析一下
        if(req.headers["content-type"] == "application/json") {
            if(req.body.length() == 0) {
                return false;
            }
            try{
                req.data = json::parse(req.body);
            }catch(const std::exception &e) { // 解析错了也算 bad request吧
                return false;
            }
        } else if(req.headers["content-type"] == "application/x-www-form-urlencoded") {
            std::string data = req.body;
            std::string delimiter = "&";
            size_t pos = 0;
            std::string token;
            while ((pos = data.find(delimiter)) != std::string::npos) {
                token = data.substr(0, pos);
                std::string key = token.substr(0,token.find('='));
                std::string value = token.substr(token.find('=')+1);
                req.data[key] = value;
                data.erase(0, pos + delimiter.length());
            }
            if(data.size() > 0) {
                std::string key = data.substr(0,data.find('='));
                std::string value = data.substr(data.find('=')+1);
                req.data[key] = value;
            }
        }
    }   
    return true;
}

int responseToBuffer(HTTPResponse &res,char * &buffer,int &buffer_len) {
    if(res.status_code == 0) {
        res.status_code = 500;
    }
    res.status = status_code[std::to_string(res.status_code)].get<std::string>();
    std::string res_str = res.version + " " + res.status + "\r\n";
    res.headers["Content-Length"] = std::to_string(res.body.size());
    for(auto it = res.headers.begin();it != res.headers.end();it++){
        res_str += it.key() + ": " + it.value().get<std::string>() + "\r\n";
    }
    res_str += "\r\n" + res.body;
    buffer = new char[res_str.size()+1];
    memcpy(buffer,res_str.c_str(),res_str.size());
    buffer_len = res_str.size();
    return 0;
}