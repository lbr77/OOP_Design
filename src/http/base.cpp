#include"base.h"
bool try_file(const std::string &path, std::ifstream &file) {
    std::filesystem::path path_norm = std::filesystem::weakly_canonical(path);
    std::filesystem::path root = std::filesystem::absolute("public");
    if (!std::filesystem::equivalent(path_norm, root)) {
        return false;
    }
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
HTTPResponse BaseHandler::handle(HTTPRequest &request) { // file server
    ifstream file;
    string path = "public" + request.path;
    HTTPResponse response;
    response.version = request.version;
    response.headers["Server"] = "None";
    response.status = "404 Not Found";
    response.body = "404";
    if(try_file(path,file)) {
        response.status = "200 OK";
        std::getline(file,response.body,'\0');
        file.close();
    }
    
    return response;
}