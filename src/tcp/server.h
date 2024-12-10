#ifndef _SERVER_H_
#define _SERVER_H_
#include <iostream>
#include <sys/epoll.h>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>
#include <fcntl.h>
#include <vector>
#include"../http/struct.h"
constexpr int MAX_EVENTS = 1000;
constexpr int DEFAULT_PORT = 8081;
class Handler {
public:
    virtual void handle(const char *buffer,int fd) = 0;
};
class TCPServer {
public:
    TCPServer(int port = DEFAULT_PORT,Handler *handle=nullptr):port(port),server_fd(-1),epoll_fd(-1),handle(handle) {}
    ~TCPServer();
    void start();
private:
    int port,server_fd,epoll_fd;
    Handler *handle;
    std::vector<epoll_event> events;

    void initSocket();
    void initEpoll();
    void eventLoop();
    void handleAccept();
    void handleClient(int fd);
};
#endif