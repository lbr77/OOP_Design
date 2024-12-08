#ifndef _HTTP_H_
#define _HTTP_H_
#include <string>
#include <map>
#include <vector>
#include<cstring>
#include<cstdio>
#include<fstream>
#include<filesystem>
#include<iostream>
struct HTTPRequest {
    std::string method;
    std::string path;
    std::string version;
    std::map<std::string,std::string> headers;
    std::string body;
};
struct HTTPResponse {
    std::string version;
    std::string status;
    std::map<std::string,std::string> headers;
    std::string body;
};
int parseRequest(const char *buffer,HTTPRequest &req);
int ResponseToBuffer(const HTTPResponse &res,char *buffer);

HTTPResponse handleRequest(const HTTPRequest &req);
#endif