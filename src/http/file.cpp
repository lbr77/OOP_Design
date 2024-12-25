#include "file.h"

#include "../utils/log.h"
#include "struct.h"
bool try_file(const std::string &path, std::ifstream &file) {
    std::filesystem::path path_norm = std::filesystem::weakly_canonical(path);
    std::filesystem::path root = std::filesystem::absolute("public");
    if (path_norm.string().find(root.string()) != 0) {
        return false;
    }
    if (!std::filesystem::exists(path)) {
        return false;
    }
    if (std::filesystem::is_directory(path)) {
        std::string index_path = path + "/index.html";
        LOG_DEBUG("Try file:", index_path);
        if (std::filesystem::exists(index_path)) {
            file.clear();
            file.open(index_path, std::ios::in | std::ios::binary);
            return file.is_open();
        }
        return false;
    }
    LOG_DEBUG("Try file:", path);
    if (std::filesystem::is_regular_file(path)) {
        file.open(path, std::ios::in | std::ios::binary);
        return file.is_open();
    }

    return false;
}
HTTPResponse FileHandler::handle(const HTTPRequest &request) {  // file server
    ifstream file;
    string path = root + request.path;
    HTTPResponse response =
        makeResponse(404, "Not Found", "text/plain");  // assert all files are binary.
    if (try_file(path, file)) {
        response.status_code = 200;
        response.status = "OK";
        if (path.find('.') != string::npos) {
            string ext = path.substr(path.find_last_of('.') + 1);
            if (content_type.find(ext) != content_type.end()) {  // 如果有对应的content-type
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