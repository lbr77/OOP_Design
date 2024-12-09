#include"base.h"
bool try_file(const std::string &path, std::ifstream &file) {
    std::filesystem::path path_norm = std::filesystem::weakly_canonical(path);
    std::filesystem::path root = std::filesystem::absolute("public");
    if (path_norm.string().find(root.string()) != 0) {
        // std::cout<<"path not in public"<<std::endl;
        std::cout<<path_norm<<" "<<root<<std::endl;
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
    response.status_code = "404";
    response.body = "404";
    if(try_file(path,file)) {
        response.status_code = "200";
        response.status = "200 OK";
        if(path.find('.') != string::npos) {
            string ext = path.substr(path.find_last_of('.')+1);
            if(content_type.find(ext) != content_type.end()) { 
                response.headers["Content-Type"] = content_type[ext].get<string>();
            }
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        response.body = buffer.str();
        file.close();
    }
    
    return response;
}