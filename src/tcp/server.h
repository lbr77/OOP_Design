#ifndef _SERVER_H_
#define _SERVER_H_
#include <iostream>
#include <sys/epoll.h>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>
#include <fcntl.h>
#include <vector>
#include"../http/http.h"
constexpr int MAX_EVENTS = 1000;
constexpr int DEFAULT_PORT = 8080;

class HTTPServer {
public:
    HTTPServer(int port = DEFAULT_PORT):port(port),server_fd(-1),epoll_fd(-1) {}
    ~HTTPServer();
    void start();
private:
    int port,server_fd,epoll_fd;
    std::vector<epoll_event> events;

    void initSocket();
    void initEpoll();
    void eventLoop();
    void handleAccept();
    void handleClient(int fd);
};
#endif