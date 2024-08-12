#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>

class ChatServer {
public:
    ChatServer(
        muduo::net::EventLoop* loop, 
        const muduo::net::InetAddress& listenAddr, 
        const std::string& name
    );
    void start();
private:
    void onConnection(const muduo::net::TcpConnectionPtr& conn);
    void onMessage(const muduo::net::TcpConnectionPtr& conn, muduo::net::Buffer* buf, muduo::Timestamp time);
    muduo::net::TcpServer server_;
    muduo::net::EventLoop *loop_;
};

#endif