//
// Created by YangGuang on 2019-04-23.
//

#ifndef FOOTBOOK_SERVER_DB_BASE_H
#define FOOTBOOK_SERVER_DB_BASE_H

#include <string>
#include "db_base_factory.h"

namespace footbook {
namespace db {

class SqlDB {
 public:
    static SqlDB *New(DBBaseFactory* factory);

    //  数据库连接函数
    virtual bool Connect(const char *sever, const char *user,
                         const char *pass, const char *database,
                         int port = 3306) = 0;
    // 数据库创建
    virtual bool CreateDatabase(const std::string &data_name) = 0;
    // 创建表
    virtual bool CreateTable(const std::string &table_name) = 0;
    // 删除表
    virtual bool DeleteTable(const std::string &table_name) = 0;
    // 删除数据库
    virtual bool DeleteDatabase(const std::string &data_name) = 0;
    virtual bool IsExistTable(const std::string &table_name) = 0;
    // 关闭数据库连接
    virtual void Close() = 0;
    // 写入数据 , query是一个sql语句
    virtual bool WriteData(const std::string &query) = 0;
    // 读取数据, query参数代表一个sql语句，result参数表示返回的结果
    virtual bool ReadData(const std::string &query,
                          std::vector<std::vector<std::string>> &result) = 0;
    // 删除数据
    virtual bool DeleteData(const std::string &query) = 0;
    // 修改数据
    virtual bool ModifyData(const std::string &query) = 0;
    // 获取错误信息
    virtual const char *GetLastError() const { return nullptr; }
    virtual ~SqlDB() = default;
};



}   // namespace db
}   // namespace footbook
#endif //FOOTBOOK_SERVER_DB_BASE_H
