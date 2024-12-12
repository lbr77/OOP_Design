#include "src/api/qq.h"
#include "src/db/db.h"
#include "src/http/base.h"
#include "src/tcp/server.h"
#include "src/utils/log.h"
int main(int argc, char **argv) {
    SET_LOG_LEVEL(LogLevel::DEBUG);
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
    db->connect("tcp://localhost:3306", user, pass, "test");
    Handler handlers;
    TCPServer server(port, &handlers);
    handlers.add_handle("/api/qq", new QQHandler());
    handlers.default_handle(new FileHandler());
    server.start();
    return 0;
}