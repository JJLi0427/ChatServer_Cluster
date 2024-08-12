#include "ChatServer.hpp"
#include "ChatService.hpp"

#include <iostream>
#include <signal.h>

void resethandler(int) {
    ChatService::instance()->serverCloseException();
    exit(0);
}

int main() {
    signal(SIGINT, resethandler);

    muduo::net::EventLoop loop;
    muduo::net::InetAddress listenAddr("127.0.0.1", 6666);
    ChatServer server(&loop, listenAddr, "ChatServer");
    server.start();
    loop.loop();
    return 0;
}