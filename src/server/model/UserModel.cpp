#include "UserModel.hpp"
#include "mysqldb.hpp"

bool UserModel::insert(User &user) {
    // 组装sql语句
    char sql[1024] = {0};
    sprintf(
        sql, "insert into User(name, password, state) values('%s', '%s', '%s')", 
        user.getName().c_str(), user.getPassword().c_str(), user.getState().c_str()
    );
    // 连接数据库
    MySQL mysql;
    if (mysql.connect()) {
        if (mysql.update(sql)) {
            // 获取插入成功的用户数据生成的主键id
            user.setId(mysql_insert_id(mysql.getConnection()));
            return true;
        }
    }
    return false;
}

User UserModel::query(int id) {
    char sql[1024] = {0};
    sprintf(sql, "select * from User where id = %d", id);
    MySQL mysql;
    if (mysql.connect()) {
        // 查询数据得到返回结果
        MYSQL_RES* res = mysql.query(sql);
        if (res != nullptr) {
            // 获取结果行
            MYSQL_ROW row = mysql_fetch_row(res);
            if (row != nullptr) {
                User user;
                user.setId(atoi(row[0]));
                user.setName(row[1]);
                user.setPassword(row[2]);
                user.setState(row[3]);
                mysql_free_result(res);
                return user;
            }
        }
    }
    return User();
}

bool UserModel::updateState(User user) {
    char sql[1024] = {0};
    sprintf(sql, "update User set state='%s' where id=%d", user.getState().c_str(), user.getId());
    MySQL mysql;
    if (mysql.connect()) {
        if (mysql.update(sql)) {
            return true;
        }
    }
    return false;
}

void UserModel::resetState() {
    char sql[1024] = "update User set state='offline' where state='online'";
    MySQL mysql;
    if (mysql.connect()) {
        mysql.update(sql);
    }
}