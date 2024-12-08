#ifndef _HTTP_H_
#define _HTTP_H_
#include <string>
#include<nlohmann/json.hpp>
using json = nlohmann::json;
struct HTTPRequest {
    std::string method;
    std::string path;
    std::string version;
    json headers;
    std::string body;
};
struct HTTPResponse {
    std::string version;
    std::string status;
    json headers;
    std::string body;
};
#endif