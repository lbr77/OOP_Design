#ifndef _HTTP_H_
#define _HTTP_H_
#include <string>
#include<nlohmann/json.hpp>
using json = nlohmann::json;
const json status_code = {
    {"100","100 Continue"},
    {"101","101 Switching Protocols"},
    {"102","102 Processing"},
    {"103","103 Early Hints"},
    {"200","200 OK"},
    {"201","201 Created"},
    {"202","202 Accepted"},
    {"203","203 Non-Authoritative Information"},
    {"204","204 No Content"},
    {"205","205 Reset Content"},
    {"206","206 Partial Content"},
    {"207","207 Multi-Status"},
    {"208","208 Already Reported"},
    {"226","226 IM Used"},
    {"300","300 Multiple Choices"},
    {"301","301 Moved Permanently"},
    {"302","302 Found"},
    {"303","303 See Other"},
    {"304","304 Not Modified"},
    {"400","400 Bad Request"},
    {"401","401 Unauthorized"},
    {"403","403 Forbidden"},
    {"404","404 Not Found"},
    {"500","500 Internal Server Error"}
};

const json content_type = {
    {"html","text/html"},
    {"css","text/css"},
    {"js","application/javascript"},
    {"json","application/json"},
    {"jpg","image/jpeg"},
    {"jpeg","image/jpeg"},
    {"png","image/png"},
    {"gif","image/gif"},
    {"svg","image/svg+xml"},
    {"ico","image/x-icon"},
    {"mp3","audio/mpeg"},
    {"mp4","video/mp4"},
    {"webm","video/webm"},
    {"pdf","application/pdf"},
    {"zip","application/zip"},
    {"tar","application/x-tar"},
    {"gz","application/gzip"},
    {"bz2","application/x-bzip2"},
    {"rar","application/vnd.rar"},
    {"7z","application/x-7z-compressed"},
    {"exe","application/x-msdownload"},
    {"doc","application/msword"},
    {"docx","application/vnd.openxmlformats-officedocument.wordprocessingml.document"},
    {"xls","application/vnd.ms-excel"},
    {"xlsx","application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"},
    {"ppt","application/vnd.ms-powerpoint"},
    {"pptx","application/vnd.openxmlformats-officedocument.presentationml.presentation"},
    {"csv","text/csv"},
    {"xml","application/xml"},
    {"json","application/json"},
    {"yaml","application/x-yaml"},
    {"txt","text/plain"},
    {"rtf","application/rtf"},

};
struct HTTPRequest {
    std::string method;
    std::string path;
    std::string version;
    json headers;
    std::string body;
};
struct HTTPResponse {
    std::string version;
    std::string status_code;
    std::string status;
    json headers;
    std::string body;
};
#endif