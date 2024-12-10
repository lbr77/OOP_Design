#include"src/tcp/server.h"
#include"src/http/httphandler.h"
#include"src/http/base.h"
#include"src/api/qq.h"
// #include"src/api/weixin.h"
// #include"api/weibo.h"
#include"src/db/db.h"
#include"src/utils/log.h"
int main(int argc,char **argv) {
    SET_LOG_LEVEL(LogLevel::DEBUG);
    auto db = SQL::getInstance();
    int port = 8080;
    if(argc >= 2) {
        port = atoi(argv[1]);
    }
    std::string user="root";
    std::string pass="liborui77";
    if(argc >= 4){
        user = argv[2];
        pass = argv[3];
    }
    db->connect("tcp://localhost:3306",user,pass,"test");
    HTTPHandlers handlers;
    // handlers.add_handle("/api/weixin",new WeixinHandler());
    // handlers.add_handle("/api/qq",new QQHandler());
    // handlers.add_handle("/api/weibo",new WeiboHandler());
    handlers.default_handle(new BaseHandler());
    TCPServer server(port,&handlers);
    server.start();
    return 0;
}