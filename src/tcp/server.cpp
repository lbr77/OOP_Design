#include "server.h"

#include "../http/struct.h"
#include "../utils/log.h"
TCPServer::~TCPServer() {
    if (server_fd != -1) {
        close(server_fd);
    }
    if (epoll_fd != -1) {
        close(epoll_fd);
    }
}

void TCPServer::start() {
    initSocket();
    initEpoll();
    eventLoop();
}

void TCPServer::initSocket() {
    server_fd = socket(AF_INET, SOCK_STREAM, 0);  // TCP
    if (server_fd == -1) {
        LOG_ERROR("Failed to create socket");
        exit(1);
    }
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_fd, (sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        LOG_ERROR("Failed to bind");
        close(server_fd);
        exit(1);
    }

    if (listen(server_fd, 10) == -1) {
        LOG_ERROR("Failed to listen");
        close(server_fd);
        exit(1);
    }
    LOG_INFO("Server started on port ", port);
}

void TCPServer::initEpoll() {
    epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        LOG_ERROR("Failed to create epoll");
        close(server_fd);
        exit(1);
    }
    epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = server_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &ev) == -1) {
        LOG_ERROR("Failed to add server fd to epoll");
        close(server_fd);
        close(epoll_fd);
        exit(1);
    }
    int flags = fcntl(server_fd, F_GETFL, 0);
    if (fcntl(server_fd, F_SETFL, flags | O_NONBLOCK) == -1) {
        LOG_ERROR("Failed to set server fd to non-blocking");
        close(server_fd);
        close(epoll_fd);
        exit(1);
    }
    events.resize(MAX_EVENTS);
}

void TCPServer::eventLoop() {
    while (true) {
        int nfds = epoll_wait(epoll_fd, events.data(), MAX_EVENTS, -1);
        if (nfds == -1) {
            LOG_ERROR("Failed to epoll_wait");
            close(server_fd);
            close(epoll_fd);
            exit(1);
        }
        for (int i = 0; i < nfds; i++) {
            if (events[i].data.fd == server_fd) {
                handleAccept();
            } else if (events[i].events & EPOLLIN) {
                handleClient(events[i].data.fd);
            }
        }
    }
}

void TCPServer::handleAccept() {
    sockaddr_in client_addr = {};
    socklen_t client_len = sizeof(client_addr);
    int client_fd = accept(server_fd, (sockaddr*)&client_addr, &client_len);
    if (client_fd == -1) {
        LOG_ERROR("Failed to accept");
        close(server_fd);
        close(epoll_fd);
        exit(1);
    }
    LOG_DEBUG("Accepted connection from " + std::string(inet_ntoa(client_addr.sin_addr)) + ":" +
              std::to_string(ntohs(client_addr.sin_port)));
    epoll_event ev = {};
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = client_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &ev) == -1) {
        LOG_ERROR("Failed to add client fd to epoll");
        close(server_fd);
        close(epoll_fd);
        close(client_fd);
        exit(1);
    }
    int flags = fcntl(client_fd, F_GETFL, 0);
    if (fcntl(client_fd, F_SETFL, flags | O_NONBLOCK) == -1) {
        LOG_ERROR("Failed to set client fd to non-blocking");
        close(server_fd);
        close(epoll_fd);
        close(client_fd);
        exit(1);
    }
}

void TCPServer::handleClient(int fd) {
    const size_t BUFFER_SIZE = 1024;
    const size_t MAX_REQUEST_SIZE = 1024 * 1024;  // 1MB limit

    std::unique_ptr<char[]> buffer(new char[BUFFER_SIZE]);
    std::string recvd;
    recvd.reserve(BUFFER_SIZE);

    // Receive data
    while (true) {
        if (recvd.size() >= MAX_REQUEST_SIZE) {
            LOG_ERROR("Request too large");
            const char* error_response =
                "HTTP/1.1 413 Payload Too Large\r\n"
                "Content-Type: text/html\r\n\r\n"
                "413 Payload Too Large";
            send(fd, error_response, strlen(error_response), 0);
            close(fd);
            return;
        }

        ssize_t bytes_read = recv(fd, buffer.get(), BUFFER_SIZE - 1, 0);

        if (bytes_read == 0) {
            break;  // Connection closed by peer
        } else if (bytes_read < 0) {
            if (errno != EAGAIN && errno != EWOULDBLOCK) {
                LOG_ERROR("recv() failed: ", strerror(errno));
            }
            break;
        }

        buffer[bytes_read] = '\0';
        recvd += buffer.get();
    }

    // Parse request
    HTTPRequest req;
    if (!bufferToRequest(recvd.c_str(), req)) {
        LOG_ERROR("Failed to parse request: ", recvd);
        const char* error_response =
            "HTTP/1.1 500 Internal Server Error\r\n"
            "Content-Type: text/html\r\n\r\n"
            "{\"code\":500,\"msg\":\"Internal Server Error\"}";
        send(fd, error_response, strlen(error_response), 0);
        close(fd);
        return;
    }

    // Handle request
    LOG_INFO("Request: ", req.method, req.path, req.version);
    HTTPResponse response = handle->handle(req);
    LOG_INFO("Response: ", response.status_code, response.status);

    // Prepare and send response
    char* res_buffer = nullptr;
    int buffer_len = 0;
    if (responseToBuffer(response, res_buffer, buffer_len) == -1) {
        LOG_ERROR("Failed to create response");
        const char* error_response =
            "HTTP/1.1 500 Internal Server Error\r\n"
            "Content-Type: text/html\r\n\r\n"
            "{\"code\":500,\"msg\":\"Internal Server Error\"}";
        send(fd, error_response, strlen(error_response), 0);
    } else {
        send(fd, res_buffer, buffer_len, 0);
    }
    delete[] res_buffer;
    close(fd);
    LOG_DEBUG("Closed connection");
}