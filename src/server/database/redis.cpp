#include "redis.hpp"
#include <iostream>

Redis::Redis() : _publish_context(nullptr), _subscribe_context(nullptr) {}

Redis::~Redis() {
    if (_publish_context != nullptr) {
        redisFree(_publish_context);
    }
    if (_subscribe_context != nullptr) {
        redisFree(_subscribe_context);
    }
}

bool Redis::connect() {
    // 创建redis上下文对象
    _publish_context = redisConnect("127.0.0.1", 6379);
    if (_publish_context == nullptr) {
        std::cerr << "connect redis failed!" << std::endl;
        return false;
    }
    _subscribe_context = redisConnect("127.0.0.1", 6379);
    if (_subscribe_context == nullptr) {
        std::cerr << "connect redis failed!" << std::endl;
        return false;
    }

    // 在单独线程中处理订阅通道中的消息
    std::thread t([&](){
        observer_channel_message();
    });
    t.detach();

    return true;
}

bool Redis::publish(int channel, std::string message) {
    // PUBLISH channel message
    redisReply *reply = (redisReply*)redisCommand(_publish_context, "PUBLISH %d %s", channel, message.c_str());
    if (reply == nullptr) {
        std::cerr << "publish command failed!" << std::endl;
        return false;
    }
    freeReplyObject(reply);
    return true;
}

bool Redis::subscribe(int channel) {
    // SUBSCRIBE channel
    // 只做订阅操作, 不接收消息
    if (REDIS_ERR == redisAppendCommand(_subscribe_context, "SUBSCRIBE %d", channel)) {
        std::cerr << "subscribe command failed!" << std::endl;
        return false;
    }
    int done = 0;
    // 循环发送，完成后done为1
    while (!done) {
        if (REDIS_ERR == redisBufferWrite(_subscribe_context, &done)) {
            std::cerr << "subscribe command failed!" << std::endl;
            return false;
        }
    }
    return true;
}

bool Redis::unsubscribe(int channel) {
    // UNSUBSCRIBE channel
    // 取消订阅
    if (REDIS_ERR == redisAppendCommand(_subscribe_context, "UNSUBSCRIBE %d", channel)) {
        std::cerr << "unsubscribe command failed!" << std::endl;
        return false;
    }
    int done = 0;
    while (!done) {
        if (REDIS_ERR == redisBufferWrite(_subscribe_context, &done)) {
            std::cerr << "unsubscribe command failed!" << std::endl;
            return false;
        }
    }
    return true;
}

void Redis::observer_channel_message() {
    redisReply *reply = nullptr;
    while (REDIS_OK == redisGetReply(_subscribe_context, (void**)&reply)) {
        if (reply != nullptr && reply->element[2] != nullptr && reply->element[2]->str != nullptr) {
            // 调用回调函数
            _notify_message_handler(atoi(reply->element[1]->str), reply->element[2]->str);
        }
        freeReplyObject(reply);
    }
}

void Redis::init_notify_handler(std::function<void(int, std::string)> fn) {
    this->_notify_message_handler = fn;
}