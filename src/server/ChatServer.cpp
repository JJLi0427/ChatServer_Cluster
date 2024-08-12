#include "ChatServer.hpp"
#include "ChatService.hpp"

#include "json.hpp"
using json = nlohmann::json;

#include <functional>

ChatServer::ChatServer(
    muduo::net::EventLoop* loop, 
    const muduo::net::InetAddress& listenAddr, 
    const std::string& name
) : server_(loop, listenAddr, name), loop_(loop) {
    // 设置连接的回调
    server_.setConnectionCallback(
        std::bind(&ChatServer::onConnection, this, std::placeholders::_1)
    );
    // 设置消息回调
    server_.setMessageCallback(
        std::bind(&ChatServer::onMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)
    );
    // 设置线程数量
    server_.setThreadNum(4);
}

void ChatServer::start() {
    server_.start();
}

// 连接的回调
void ChatServer::onConnection(const muduo::net::TcpConnectionPtr& conn) {
    // 客户端断开连接
    if (!conn->connected()) {
        ChatService::instance()->clientCloseException(conn);
        conn->shutdown();
    }
}

// 处理消息的回调
void ChatServer::onMessage(const muduo::net::TcpConnectionPtr& conn, muduo::net::Buffer* buf, muduo::Timestamp time) {
    muduo::string msg(buf->retrieveAllAsString());
    // 数据的反序列化
    json js = json::parse(msg);

    // 通过js["msgid"]获取业务处理器
    auto msgHandler = ChatService::instance()->getMsgHandler(js["msgid"].get<int>());
    // 回调消息绑定好的事件处理器来执行相应的业务处理
    msgHandler(conn, js, time);
}

