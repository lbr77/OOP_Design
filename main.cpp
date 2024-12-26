#include <unistd.h>

#include <csignal>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "src/api/qq.h"
#include "src/api/weibo.h"
#include "src/api/wx.h"
#include "src/db/db.h"
#include "src/http/file.h"
#include "src/tcp/server.h"
#include "src/utils/log.h"
using std::cin, std::cout, std::endl;
int main(int argc, char **argv) {
    // SET_LOG_LEVEL(LogLevel::DEBUG);
    signal(SIGINT, [](int sig) {
        LOG_INFO("SIGINT received, exiting...");
        exit(0);
    });
    auto db = SQL::getInstance();
    int port = 8080;
    if (argc >= 2) {
        port = atoi(argv[1]);
    }
    std::string user = "root";
    std::string pass = "liborui77";
    if (argc >= 4) {
        user = argv[2];
        pass = argv[3];
    }
    db->connect("tcp://localhost:3306", user, pass, "database");
    Handler handlers;
    TCPServer server(port, &handlers);
    handlers.add_handle("/qq", new QQHandler());
    handlers.add_handle("/wb", new WeiboHandler());
    handlers.add_handle("/wx", new WeChatHandler());
    handlers.default_handle(new FileHandler());
    server.start();
    return 0;
}