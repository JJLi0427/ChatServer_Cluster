#ifndef GROUPUSER_H
#define GROUPUSER_H

#include "User.hpp"

class GroupUser : public User {
public:
    void setRole(int role) {
        this->role = role;
    }
private:
    std::string role;
};

#endif