#include"tcp/server.h"
#include"http/httphandler.h"
#include"http/base.h"
#include"api/qq.h"
#include"api/weixin.h"
#include"api/weibo.h"
int main(int argc,char **argv) {
    int port = 8080;
    if(argc == 2) {
        port = atoi(argv[1]);
    }
    HTTPHandlers handlers;
    handlers.add_handle("/api/weixin",new WeixinHandler());
    handlers.add_handle("/api/qq",new QQHandler());
    handlers.add_handle("/api/weibo",new WeiboHandler());
    handlers.default_handle(new BaseHandler());
    TCPServer server(port,&handlers);
    server.start();
    return 0;
}