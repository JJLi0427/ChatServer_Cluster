#ifndef DB_H
#define DB_H

#include <mysql/mysql.h>
#include <muduo/base/Logging.h>

#include <string>

// 数据库配置信息
static std::string server = "127.0.0.1";
static std::string user = "root";
static std::string password = "427515";
static std::string dbname = "ChatServer";

// 数据操作类
class MySQL {
public:
    // 初始化数据库连接
    MySQL();
    // 释放数据库连接资源
    ~MySQL();
    // 连接数据库
    bool connect();
    // 更新操作
    bool update(const std::string& sql);
    // 查询操作
    MYSQL_RES* query(const std::string& sql);
    // 获取连接
    MYSQL* getConnection();
private:
    MYSQL* _conn;
};

#endif