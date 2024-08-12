#ifndef USERMODEL_H
#define USERMODEL_H

#include "User.hpp"

class UserModel {
public:
    // 插入用户
    bool insert(User &user);
    // 通过id查询用户
    User query(int id);
    // 更新用户状态信息
    bool updateState(User user);
    // 重置用户状态信息
    void resetState();
private:

};

#endif