#ifndef _BASE_H_
#define _BASE_H_
#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>

#include <filesystem>
#include <fstream>
#include <iostream>

#include "handler.h"
#include "struct.h"
using std::ifstream;
using std::string;

class FileHandler : public Handler {  // by default a file server
   public:
    virtual HTTPResponse handle(const HTTPRequest &request);  // request to response
};
#endif