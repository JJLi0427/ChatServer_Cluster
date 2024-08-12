#include "mysqldb.hpp"

// 初始化数据库连接
MySQL::MySQL () {
    _conn = mysql_init(nullptr);
}
// 释放数据库连接资源
MySQL::~MySQL () {
    if (_conn != nullptr) {
        mysql_close(_conn);
    }
}
// 连接数据库
bool MySQL::connect () {
    MYSQL* p = mysql_real_connect(_conn, server.c_str(), user.c_str(), password.c_str(), dbname.c_str(), 3306, nullptr, 0);
    if (p == nullptr) {
        LOG_ERROR << "connect mysql error: " << mysql_error(_conn);
    }
    else {
        LOG_INFO << "connect mysql success!";
    }
    return p;
}
// 更新操作
bool MySQL::update (const std::string& sql) {
    if (mysql_query(_conn, sql.c_str())) {
        LOG_ERROR << "update error: " << sql << " " << mysql_error(_conn);
        return false;
    }
    return true;
}
// 查询操作
MYSQL_RES* MySQL::query (const std::string& sql) {
    if (mysql_query(_conn, sql.c_str())) {
        LOG_ERROR << "query error: " << sql << " " << mysql_error(_conn);
        return nullptr;
    }
    return mysql_store_result(_conn);
}
// 获取连接
MYSQL* MySQL::getConnection() {
    return _conn;
}