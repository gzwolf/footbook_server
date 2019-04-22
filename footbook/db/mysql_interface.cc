//
// Created by YangGuang on 2019-04-15.
//

#include "mysql_interface.h"

namespace footbook {
namespace db {

MysqlInterface::MysqlInterface() :
        errno_num_(0), error_info_("ok") {
    mysql_library_init(0, nullptr, nullptr);
    mysql_init(&mysqlinterface_);
    mysql_options(&mysqlinterface_, MYSQL_SET_CHARSET_NAME, "utf8");
}


bool MysqlInterface::Connect(const char *sever, const char *user,
                                     const char *pass, const char *database, int port) {
    if (mysql_real_connect(&mysqlinterface_, sever,
                           user, pass, database, port, 0, 0) != nullptr) {
        return true;
    } else {
        ErrorMysqlInfo();
    }

    return false;
}


bool MysqlInterface::CreateDatabase(const std::string &data_name) {
    std::string query_str = "create database if not exists";
    query_str += data_name;

    if (mysql_query(&mysqlinterface_, query_str.c_str()) == 0) {
        query_str = "use";
        query_str += data_name;

        if (mysql_query(&mysqlinterface_, query_str.c_str()) == 0) {
            return true;
        }
    }

    ErrorMysqlInfo();
    return false;
}

bool MysqlInterface::CreateTable(const std::string &table_name) {
    std::string query_str = "create table ";
    query_str += table_name;
    if (mysql_query(&mysqlinterface_, table_name.c_str()) != 0) {
        ErrorMysqlInfo();
        return false;
    }

    return true;
}

bool MysqlInterface::DeleteTable(const std::string &table_name) {
    std::string query_str = "drop table ";
    query_str += table_name;
    if (mysql_query(&mysqlinterface_, table_name.c_str()) != 0) {
        ErrorMysqlInfo();
        return false;
    }
}

bool MysqlInterface::DeleteDatabase(const std::string &data_name) {
    std::string query_str = "drop database ";
    query_str += data_name;
    if (mysql_query(&mysqlinterface_, data_name.c_str()) != 0) {
        ErrorMysqlInfo();
        return false;
    }
    return false;
}


bool MysqlInterface::WriteData(const std::string &query) {
    if (mysql_query(&mysqlinterface_, query.c_str()) != 0) {
        ErrorMysqlInfo();
        return false;
    }

    return true;
}


bool MysqlInterface::ReadData(const std::string &query,
                              std::vector<std::vector<std::string> > &result) {
    if (mysql_query(&mysqlinterface_, query.c_str()) != 0) {
        ErrorMysqlInfo();
        return false;
    }

    /* 保存结果集 */
    result_ = mysql_store_result(&mysqlinterface_);

    /* 获得列数 */
    int field = mysql_num_fields(result_);

    MYSQL_ROW line = nullptr;

    /* 将结果保存到 result ,result 是一个vector<vector<string>> */
    while ((line = mysql_fetch_row(result_)) != nullptr) {
        std::vector<std::string> line_data;
        std::string tmp;

        for (int i = 0; i < field; i++) {
            if (line[i])
                tmp = line[i];
            else
                tmp = "";

            line_data.push_back(tmp);
        }

        result.push_back(line_data);
    }

    return true;

}


bool MysqlInterface::DeleteData(const std::string &query) {
    if (mysql_query(&mysqlinterface_, query.c_str()) != 0) {
        ErrorMysqlInfo();
        return false;
    }

    return true;
}


bool MysqlInterface::ModifyData(const std::string &query) {
    if (mysql_query(&mysqlinterface_, query.c_str()) != 0) {
        ErrorMysqlInfo();
        return false;
    }
    return true;
}

void MysqlInterface::ErrorMysqlInfo() {
    errno_num_ = mysql_errno(&mysqlinterface_);
    error_info_ = mysql_error(&mysqlinterface_);
}

void MysqlInterface::Close() {
    mysql_close(&mysqlinterface_);
}

bool MysqlInterface::IsExistTable(const std::string &table_name) {

    result_ = mysql_list_tables(&mysqlinterface_, table_name.c_str());
    MYSQL_ROW row;
    int count = 0;
    while((row = mysql_fetch_row(result_)) != nullptr) {
        printf("TABLE %d: %s\n",count,row[0]);
        count++;
    }

    if (mysql_errno(&mysqlinterface_)) {  //mysql_fetch_row() failed due to an error
        ErrorMysqlInfo();
        return false;
    }

    if (count > 0)
        return true;
    return false;
}



}   // namespace db
}   // namespace footbook