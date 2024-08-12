#ifndef FRIENDMODEL_H
#define FRIENDMODEL_H

#include "User.hpp"

#include <vector>

class FriendModel {
public:
    // 添加好友关系
    void insert(int userid, int friendid);
    // 返回用户好友列表
    std::vector<User> query(int userid);
    // 删除好友关系
    void remove(int userid, int friendid);
private:
};

#endif