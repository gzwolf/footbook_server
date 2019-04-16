//
// Created by YangGuang on 2019-04-15.
//

#ifndef CAMPUS_CHAT_MYSQL_HELPER_H
#define CAMPUS_CHAT_MYSQL_HELPER_H

#include <mysql.h>
#include <iostream>
#include <string>
#include <vector>

namespace footbook {
namespace db {

class MysqlInterface {
 public:
    MysqlInterface();

    ~MysqlInterface() { Close(); }

    //  数据库连接函数
    bool Connect(const char *sever, const char *user,
                 const char *pass, const char *database, int port = 3306);

    // 数据库创建
    bool CreateDatabase(std::string &data_name);

    // 创建表
    bool CreateTable(const std::string &table_name);

    bool IsExistTable(const std::string &table_name);

    // 关闭数据库连接
    void Close();

    // 写入数据 , query是一个sql语句
    bool WriteData(const std::string &query);

    // 读取数据, query参数代表一个sql语句，result参数表示返回的结果
    bool ReadData(const std::string &query,
                  std::vector <std::vector<std::string>> &result);

    // 删除数据
    bool DeleteData(const std::string &query);

    // 修改数据
    bool ModifyData(const std::string &query);

    // 获取错误信息
    const char* GetLastError() const {
        return error_info_;
    }
 private:
    //  错误信息设置
    void ErrorMysqlInfo();

 private:
    MYSQL mysqlinterface_;  // MYSQL对象
    MYSQL_RES *result_;      //用于存放结果

    int errno_num_;           //错误代号
    const char *error_info_;  //错误信息
};

}   // namespace db
}   // namespace footbook

#endif //CAMPUS_CHAT_MYSQL_HELPER_H
