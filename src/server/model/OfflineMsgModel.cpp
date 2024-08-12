#include "OfflineMsgModel.hpp"
#include "mysqldb.hpp"

void OfflineMsgModel::insert(int userid, std::string msg) {
    char sql[1024] = {0};
    sprintf(sql, "insert into OfflineMessage values(%d, '%s')", userid, msg.c_str());
    MySQL mysql;
    if (mysql.connect()) {
        mysql.update(sql);
    }
}

void OfflineMsgModel::remove(int userid) {
    char sql[1024] = {0};
    sprintf(sql, "delete from OfflineMessage where userid = %d", userid);
    MySQL mysql;
    if (mysql.connect()) {
        mysql.update(sql);
    }
}

std::vector<std::string> OfflineMsgModel::query(int userid) {
    char sql[1024] = {0};
    sprintf(sql, "select message from OfflineMessage where userid = %d", userid);
    MySQL mysql;
    std::vector<std::string> vec;
    if (mysql.connect()) {
        MYSQL_RES*  res = mysql.query(sql);
        if (res != nullptr) {
            // 把离线消息存储到向量中
            MYSQL_ROW row;
            while((row = mysql_fetch_row(res)) != nullptr) {
                vec.push_back(row[0]);
            }
            mysql_free_result(res);
        }
    }
    return vec;
}