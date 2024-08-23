#include "ChatService.hpp"
#include "public.hpp"

#include <muduo/base/Logging.h>

// 获取单例对象接口
ChatService* ChatService::instance() {
    static ChatService service;
    return &service;
}

// 构造函数
ChatService::ChatService() {
    // 绑定业务处理器
    msgHandlerMap_[LOGIN_MSG] = std::bind(
        &ChatService::login, this, 
        std::placeholders::_1, std::placeholders::_2, std::placeholders::_3
    );
    msgHandlerMap_[REG_MSG] = std::bind(
        &ChatService::reg, this, 
        std::placeholders::_1, std::placeholders::_2, std::placeholders::_3
    );
    msgHandlerMap_[ONE_CHAT] = std::bind(
        &ChatService::oneChat, this, 
        std::placeholders::_1, std::placeholders::_2, std::placeholders::_3
    );
    msgHandlerMap_[ADD_FRIEND] = std::bind(
        &ChatService::addFriend, this, 
        std::placeholders::_1, std::placeholders::_2, std::placeholders::_3
    );
    msgHandlerMap_[CREATE_GROUP] = std::bind(
        &ChatService::createGroup, this, 
        std::placeholders::_1, std::placeholders::_2, std::placeholders::_3
    );
    msgHandlerMap_[ADD_GROUP] = std::bind(
        &ChatService::addGroup, this, 
        std::placeholders::_1, std::placeholders::_2, std::placeholders::_3
    );
    msgHandlerMap_[GROUP_CHAT] = std::bind(
        &ChatService::groupChat, this, 
        std::placeholders::_1, std::placeholders::_2, std::placeholders::_3
    );
    msgHandlerMap_[LOGOUT_MSG] = std::bind(
        &ChatService::loginout, this, 
        std::placeholders::_1, std::placeholders::_2, std::placeholders::_3
    );
}

// 登陆业务处理
void ChatService::login(const muduo::net::TcpConnectionPtr& conn, json& js, muduo::Timestamp time) {
    int id = js["id"];
    std::string password = js["password"];

    User user = _usermodel.query(id);
    if (user.getId() != -1 && user.getPassword() == password) {
        // 登陆成功
        if (user.getState() == "online") {
            // 该用户已经在线
            json response;
            response["msgid"] = LOGIN_ACK;
            response["error"] = 2;
            response["errmsg"] = "该用户已经在线";
            conn->send(response.dump());
        }
        else {
            // 登陆成功，记录用户连接信息
            {
                std::lock_guard<std::mutex> lock(_connMutex);
                _userConnMap.insert({id, conn});
            }
            // 更新用户状态信息
            user.setState("online");
            _usermodel.updateState(user);
            json response;
            response["msgid"] = LOGIN_ACK;
            response["error"] = 0;
            response["id"] = id;
            response["name"] = user.getName();
            // 接受离线消息
            std::vector<std::string> vec = _offlinemsgmodel.query(id);
            if (!vec.empty()) {
                response["offlinemsg"] = vec;
                _offlinemsgmodel.remove(id);
            }
            // 查询用户的好友信息
            std::vector<User> userVec = _friendmodel.query(id);
            if (userVec.size() > 0) {
                std::vector<std::string> vec2;
                for (User &user : userVec) {
                    json js;
                    js["id"] = user.getId();
                    js["name"] = user.getName();
                    js["state"] = user.getState();
                    vec2.push_back(js.dump());
                }
                response["friends"] = vec2;
            }
            // 查询用户的群组信息
            std::vector<Group> groupVec = _groupmodel.queryGroups(id);
            if (groupVec.size() > 0) {
                std::vector<std::string> vec3;
                for (Group &group : groupVec) {
                    json js;
                    js["id"] = group.getId();
                    js["groupname"] = group.getName();
                    js["groupdesc"] = group.getDesc();
                    std::vector<std::string> vec4;
                    for (GroupUser &groupUser : group.getUser()) {
                        json js;
                        js["id"] = groupUser.getId();
                        js["name"] = groupUser.getName();
                        js["state"] = groupUser.getState();
                        js["role"] = groupUser.getRole();
                        vec4.push_back(js.dump());
                    }
                    js["users"] = vec4;
                    vec3.push_back(js.dump());
                }
                response["groups"] = vec3;
            }
            conn->send(response.dump());
        }
    }
    else {
        // 登陆失败
        json response;
        response["msgid"] = LOGIN_ACK;
        response["error"] = 1;
        response["errmsg"] = "用户名或密码错误";
        conn->send(response.dump());
    }
}

// 注册业务处理
void ChatService::reg(const muduo::net::TcpConnectionPtr& conn, json& js, muduo::Timestamp time) {
    std::string name = js["name"];
    std::string password = js["password"];

    User user;
    user.setName(name);
    user.setPassword(password);
    bool flag  = _usermodel.insert(user);
    if (flag) {
        json response;
        response["msgid"] = REG_ACK;
        response["id"] = user.getId();
        response["error"] = 0;
        conn->send(response.dump());
    }
    else {
        json response;
        response["msgid"] = REG_ACK;
        response["error"] = 1;
        conn->send(response.dump());
    }
}

// 一对一聊天业务
void ChatService::oneChat(const muduo::net::TcpConnectionPtr& conn, json& js, muduo::Timestamp time) {
    int toid = js["toid"].get<int>();
    {
        std::lock_guard<std::mutex> lock(_connMutex);
        auto it = _userConnMap.find(toid);
        if (it != _userConnMap.end()) {
            // 转发消息
            it->second->send(js.dump());
            return;
        }
    }
    // 查询toid是否在线
    User user = _usermodel.query(toid);
    if (user.getId() != -1) {
        // 用户不在线, 存储离线消息
        _offlinemsgmodel.insert(toid, js.dump());
    }
    else {
        // 用户不存在
        json response;
        response["msgid"] = ONE_CHAT;
        response["error"] = 1;
        response["errmsg"] = "no user";
        conn->send(response.dump());
    }
}

// 添加好友
void ChatService::addFriend(const muduo::net::TcpConnectionPtr& conn, json& js, muduo::Timestamp time) {
    int id = js["id"];
    int friendid = js["friendid"];
    _friendmodel.insert(id, friendid);
}

// 创建群组
void ChatService::createGroup(const muduo::net::TcpConnectionPtr& conn, json& js, muduo::Timestamp time) {
    int userid = js["id"];
    std::string groupname = js["groupname"];
    std::string groupdesc = js["groupdesc"];
    Group group(-1, groupname, groupdesc);
    if (_groupmodel.createGroup(group)) {
        _groupmodel.addGroup(userid, group.getId(), "creator");
    }
}

// 加入群组
void ChatService::addGroup(const muduo::net::TcpConnectionPtr& conn, json& js, muduo::Timestamp time) {
    int userid = js["id"];
    int groupid = js["groupid"];
    _groupmodel.addGroup(userid, groupid, "normal");
}

// 群组聊天
void ChatService::groupChat(const muduo::net::TcpConnectionPtr& conn, json& js, muduo::Timestamp time) {
    int id = js["id"];
    int groupid = js["groupid"];
    std::vector<int> vec = _groupmodel.queryGroupUsers(id, groupid);
    for (int userid : vec) {
        {
            std::lock_guard<std::mutex> lock(_connMutex);
            auto it = _userConnMap.find(userid);
            if (it != _userConnMap.end()) {
                it->second->send(js.dump());
            }
            else {
                // 存储离线消息
                _offlinemsgmodel.insert(userid, js.dump());
            }
        }
    }
}

// 用户注销
void ChatService::loginout(const muduo::net::TcpConnectionPtr& conn, json& js, muduo::Timestamp time) {
    int id = js["id"];
    {
        std::lock_guard<std::mutex> lock(_connMutex);
        auto it = _userConnMap.find(id);
        if (it != _userConnMap.end()) {
            _userConnMap.erase(it);
        }
    }
    // 更新用户的状态信息
    User user;
    user.setId(id);
    user.setState("offline");
    _usermodel.updateState(user);
}

// 获取消息对应的处理器
MsgHandler ChatService::getMsgHandler(int msgid) {
    auto it = msgHandlerMap_.find(msgid);
    if (it == msgHandlerMap_.end()) {
        // 返回一个空操作
        return [&msgid](const muduo::net::TcpConnectionPtr& conn, json& js, muduo::Timestamp time) {
            LOG_ERROR << "msgid: " << msgid << " can not find handler!";
        };
    }
    return msgHandlerMap_[msgid];   
}

// 客户端异常退出
void ChatService::clientCloseException(const muduo::net::TcpConnectionPtr& conn) {
    User user;
    {
        std::lock_guard<std::mutex> lock(_connMutex);
        for (auto &it : _userConnMap) {
            if (it.second == conn) {
                user.setId(it.first);
                _userConnMap.erase(it.first);
                break;
            }
        }
    }
    // 更新用户的状态信息
    if (user.getId() != -1) {
        user.setState("offline");
        _usermodel.updateState(user);
    }
}   

// 服务器异常退出
void ChatService::serverCloseException() {
    _usermodel.resetState();
}