#ifndef PUBLIC_H
#define PUBLIC_H

enum EnMsgType {
    LOGIN_MSG = 1, // 登陆消息
    REG_MSG = 2, // 注册消息
    REG_ACK = 3, // 注册响应
    LOGIN_ACK = 4, // 登陆响应
    ONE_CHAT = 5, // 聊天消息
    ADD_FRIEND = 6, // 添加好友
    CREATE_GROUP = 7, // 创建群组
    ADD_GROUP = 8, // 加入群组
    GROUP_CHAT = 9, // 群聊天
};

#endif