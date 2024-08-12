#ifndef CHATSERVICE_H
#define CHATSERVICE_H

#include <muduo/net/TcpServer.h>

#include "json.hpp"
using json = nlohmann::json;

#include "UserModel.hpp"
#include "FriendModel.hpp"
#include "GroupModel.hpp"
#include "OfflineMsgModel.hpp"

#include <unordered_map>
#include <functional>
#include <mutex>

// 处理消息的回调类型
using MsgHandler = std::function<void(
    const muduo::net::TcpConnectionPtr& conn, 
    json& js, 
    muduo::Timestamp time
)>;

// 聊天服务类
class ChatService {
public:
    // 单例对象接口
    static ChatService* instance();
    // 处理登陆的方法
    void login(const muduo::net::TcpConnectionPtr& conn, json& js, muduo::Timestamp time);
    // 处理注册的方法
    void reg(const muduo::net::TcpConnectionPtr& conn, json& js, muduo::Timestamp time);
    // 一对一聊天业务
    void oneChat(const muduo::net::TcpConnectionPtr& conn, json& js, muduo::Timestamp time);
    // 添加好友
    void addFriend(const muduo::net::TcpConnectionPtr& conn, json& js, muduo::Timestamp time);
    // 创建群组
    void createGroup(const muduo::net::TcpConnectionPtr& conn, json& js, muduo::Timestamp time);
    // 加入群组
    void addGroup(const muduo::net::TcpConnectionPtr& conn, json& js, muduo::Timestamp time);
    // 群组聊天
    void groupChat(const muduo::net::TcpConnectionPtr& conn, json& js, muduo::Timestamp time);
    // 处理客户端异常退出
    void clientCloseException(const muduo::net::TcpConnectionPtr& conn);
    // 服务器异常退出
    void serverCloseException();
    // 获取消息处理器
    MsgHandler getMsgHandler(int msgid);
private:
    ChatService();
    // 存储消息id和其对应的业务处理方法
    std::unordered_map<int, MsgHandler> msgHandlerMap_;
    // 存储在线用户的通信连接
    std::unordered_map<int, muduo::net::TcpConnectionPtr> _userConnMap;
    std::mutex _connMutex;
    // 数据操作类对象
    UserModel _usermodel;
    FriendModel _friendmodel;
    GroupModel _groupmodel;
    OfflineMsgModel _offlinemsgmodel;
};

#endif