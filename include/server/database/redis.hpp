#ifndef REDIS_H
#define REDIS_H

#include <hiredis/hiredis.h>
#include <thread>
#include <functional>

class Redis {
public:
    Redis();
    ~Redis();

    // 连接redis服务器
    bool connect();

    // 向redis指定的通道channel发布消息
    bool publish(int channel, std::string message);

    // 向redis指定的通道channel订阅消息
    bool subscribe(int channel);
    // 向redis指定的通道channel取消订阅消息
    bool unsubscribe(int channel);

    // 在独立线程中接收订阅通道中的消息
    void observer_channel_message();

    // 设置消息回调
    void init_notify_handler(std::function<void(int, std::string)> fn);
private:
    // hiredis上下文对象, 负责publish消息
    redisContext *_publish_context;
    // hiredis上下文对象, 负责subscribe消息
    redisContext *_subscribe_context;
    // 回调函数, 收到消息后上报service
    std::function<void(int, std::string)> _notify_message_handler;
};

#endif