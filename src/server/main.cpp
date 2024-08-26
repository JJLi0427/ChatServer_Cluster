#include "ChatServer.hpp"
#include "ChatService.hpp"

#include <iostream>
#include <signal.h>

void resethandler(int) {
    ChatService::instance()->serverCloseException();
    exit(0);
}

int main(int argc, char **argv) {
    if (argc < 3) {
        std::cerr << "command invalid! example: ./ChatServer 127.0.0.1 6666" << std::endl;
        exit(-1);
    }
    signal(SIGINT, resethandler);
    char *ip = argv[1];
    uint16_t port = atoi(argv[2]);
    muduo::net::EventLoop loop;
    muduo::net::InetAddress listenAddr(ip, port);
    ChatServer server(&loop, listenAddr, "ChatServer");
    server.start();
    loop.loop();
    return 0;
}