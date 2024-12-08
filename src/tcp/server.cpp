#include"server.h"

HTTPServer::~HTTPServer() {
    if(server_fd != -1) {
        close(server_fd);
    }
    if(epoll_fd != -1) {
        close(epoll_fd);
    }
}

void HTTPServer::start() {
    initSocket();
    initEpoll();
    eventLoop();
}

void HTTPServer::initSocket(){
        server_fd = socket(AF_INET, SOCK_STREAM, 0);//TCP
        if(server_fd == -1) {
            std::cerr << "Failed to create socket" << std::endl;
            exit(1);
        }
        int opt = 1;
        setsockopt(server_fd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
        sockaddr_in server_addr;
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(port);
        server_addr.sin_addr.s_addr = INADDR_ANY;

        if(bind(server_fd,(sockaddr *)&server_addr,sizeof(server_addr)) == -1) {
            std::cerr << "Failed to bind" << std::endl;
            close(server_fd);
            exit(1);
        }

        if(listen(server_fd,10) == -1) {
            std::cerr << "Failed to listen" << std::endl;
            close(server_fd);
            exit(1);
        }
        std::cout<<"Server started at port "<<port<<std::endl;
}

void HTTPServer::initEpoll() {
    epoll_fd = epoll_create1(0);
    if(epoll_fd == -1) {
        std::cerr << "Failed to create epoll" << std::endl;
        close(server_fd);
        exit(1);
    }
    epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = server_fd;
    if(epoll_ctl(epoll_fd,EPOLL_CTL_ADD,server_fd,&ev) == -1) {
        std::cerr << "Failed to add server fd to epoll" << std::endl;
        close(server_fd);
        close(epoll_fd);
        exit(1);
    }
    int flags = fcntl(server_fd,F_GETFL,0);
    if(fcntl(server_fd,F_SETFL,flags | O_NONBLOCK) == -1) {
        std::cerr << "Failed to set server fd to non-blocking" << std::endl;
        close(server_fd);
        close(epoll_fd);
        exit(1);
    }
    events.resize(MAX_EVENTS);
}

void HTTPServer::eventLoop() {
    while(true) {
        int nfds = epoll_wait(epoll_fd,events.data(),MAX_EVENTS,-1);
        if(nfds == -1) {
            std::cerr << "Failed to epoll_wait" << std::endl;
            close(server_fd);
            close(epoll_fd);
            exit(1);
        }
        for(int i = 0; i < nfds; i++) {
            if(events[i].data.fd == server_fd) {
                handleAccept();
            } else if(events[i].events & EPOLLIN) {
                handleClient(events[i].data.fd);
            }
        }
    }
}

void HTTPServer::handleAccept(){
    sockaddr_in client_addr = {};
    socklen_t client_len = sizeof(client_addr);
    int client_fd = accept(server_fd,(sockaddr *)&client_addr,&client_len);
    if (client_fd == -1) {
        std::cerr << "Failed to accept" << std::endl;
        close(server_fd);
        close(epoll_fd);
        exit(1);
    }
    // std::cout << "New connection from " << inet_ntoa(client_addr.sin_addr) << ":" << ntohs(client_addr.sin_port) << std::endl;
    epoll_event ev = {};
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = client_fd;
    if(epoll_ctl(epoll_fd,EPOLL_CTL_ADD,client_fd,&ev) == -1) {
        std::cerr << "Failed to add client fd to epoll" << std::endl;
        close(server_fd);
        close(epoll_fd);
        close(client_fd);
        exit(1);
    }
    int flags = fcntl(client_fd,F_GETFL,0);
    if(fcntl(client_fd,F_SETFL,flags | O_NONBLOCK) == -1) {
        std::cerr << "Failed to set client fd to non-blocking" << std::endl;
        close(server_fd);
        close(epoll_fd);
        close(client_fd);
        exit(1);
    }
}

void HTTPServer::handleClient(int fd){
    char *buffer = new char[1024];
    while(true){
        ssize_t bytes_read = recv(fd,buffer,1024,0);
        if(bytes_read == 0){
            std::cout<<"Connection closed"<<std::endl;
            close(fd);
            break;
        } else if(bytes_read < 0) {
            if(errno == EAGAIN || errno == EWOULDBLOCK) {
                break;
            } else {
                // std::cerr << "Failed to read" << std::endl;
                close(fd);
                break;
            }
        } else {
            buffer[bytes_read] = '\0';
            // std::cout<<buffer<<std::endl;
            HTTPRequest req;
            if(parseRequest(buffer,req) == -1) {
                std::cerr << "Failed to parse request" << std::endl;
            }
            HTTPResponse res = handleRequest(req);
            std::cout<<res.version<<" "<<res.status<<std::endl;
            ResponseToBuffer(res,buffer);
            send(fd,buffer,strlen(buffer),0);
            close(fd);
        }
    }
    delete[] buffer;
}