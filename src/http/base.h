#ifndef _BASE_H_
#define _BASE_H_
#include"struct.h"
#include<fstream>
#include<filesystem>
#include<iostream>
#include<unistd.h>
#include<fcntl.h>
#include<sys/socket.h>

using std::ifstream;
using std::string;

class BaseHandler { // by default a file server 
public:
    virtual HTTPResponse handle(HTTPRequest &request); // request to response
};
#endif