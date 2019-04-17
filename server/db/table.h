//
// Created by YangGuang on 2019-04-15.
//

#ifndef CAMPUS_CHAT_TABLE_H
#define CAMPUS_CHAT_TABLE_H

#include <string>
#include <vector>
#include <memory>

#include "base/macor.h"
#include "server/db/table_struct.h"
#include "server/status.h"
#include "mysql_interface.h"

namespace std {
inline string to_string(string val) {
    // convert int to string
    return val;
}
}   // namespace std

namespace footbook {
namespace db {

enum TableType {
    kProfile,
    kIdea,
    kComment
};



class Table {
 public:
    static std::unique_ptr<Table> New(MysqlInterface* mysql);

    template <typename Flags, typename Key, typename Value>
    Status Get(const std::string& table_name, Flags flags,
            const Key& key, std::vector<Value>* value_vec);


    template <typename Value>
    Status Put(const std::string& table_name,
            const Value& value);

    template <typename Flags, typename Value>
    Status Delete(const std::string& table_name,
            Flags flags, const Value& value);

    template <typename Flags, typename Value>
    Status Update(const std::string& table_name, Flags flags,
            const Value& old_value, const Value& new_value);

    bool IsExistTable(const std::string& table_name);
    Status Create(const std::string& table_name);

    ~Table();

 protected:
    explicit Table(MysqlInterface* mysql);
 private:
    void ToTableStruct(const std::vector<std::string>& str_vec,
                        Profile* profile);
    void ToTableStruct(const std::vector<std::string>& str_vec,
                        Idea* idea);
    void ToTableStruct(const std::vector<std::string>& str_vec,
                        Comment* comment);

    MysqlInterface* mysql_;
    DISALLOW_COPY_AND_ASSIGN(Table);
};

template<typename Flags, typename Key, typename Value>
Status Table::Get(const std::string& table_name, Flags flags,
        const Key &key, std::vector<Value> *value_vec) {
    std::string signal_quotes = "\"";
    std::string flags_name = FlagsToString(flags);
    std::string sql = "select * from " + table_name + " where " + flags_name +
            " = " + signal_quotes + std::to_string(key) + signal_quotes;

    std::vector <std::vector<std::string>> select_result;
    bool result = mysql_->ReadData(sql, select_result);

    if (!result || select_result.empty())
        return Status::HttpError(mysql_->GetLastError());
    for (auto& vec : select_result) {
        Value value;
        ToTableStruct(vec, &value);
        value_vec->push_back(std::move(value));
    }
    return Status::Ok();
}

template<typename Value>
Status Table::Put(const std::string& table_name, const Value& value) {
    std::string sql = "insert into " + table_name + value.ToInsertSql();

    if (!mysql_->WriteData(sql))
        return Status::HttpError(mysql_->GetLastError());
    return Status::Ok();
}

template<typename Flags, typename Value>
Status Table::Delete(const std::string& table_name,
        Flags flags, const Value &value) {
    std::string signal_quotes = "\"";
    std::string sql = "delete from " + table_name + "where"
            + FlagsToString(flags) + " = " + signal_quotes
            + std::to_string(value) + signal_quotes;
    if (!mysql_->DeleteData(sql))
        return Status::HttpError(mysql_->GetLastError());
    return Status::Ok();
}

template<typename Flags, typename Value>
Status Table::Update(const std::string& table_name,Flags flags,
        const Value &old_value, const Value &new_value) {
    std::string signal_quotes = "\"";
    std::string sql = "update into " + table_name + "set "
            + FlagsToString(flags) + " = " + std::to_string(new_value)
            + "where " + FlagsToString(flags) + " = " + std::to_string(old_value);
    if (!mysql_->ModifyData(sql))
        return Status::HttpError(mysql_->GetLastError());
    return Status::Ok();
}




}   // namespace db
}   // namespace footbook
#endif //CAMPUS_CHAT_TABLE_H
