//
// Created by YangGuang on 2019-04-15.
//

#ifndef CAMPUS_CHAT_DB_H
#define CAMPUS_CHAT_DB_H

#include <string>
#include <memory>
#include <exception>

#include "base/macor.h"
#include "footbook/status.h"
#include "footbook/db/table.h"

namespace footbook {
namespace db {

class DB {
 public:
    static Status Open(const std::string& db_name,
                       DBType db_type, std::shared_ptr<DB>* db);

    template <typename TableStruct>
    Status Put(const std::string& table_name, const TableStruct& value);

    template <typename TableStruct, typename Flags, typename Key>
    Status Get(const std::string& table_name, Flags flags, const Key& key,
               std::vector<TableStruct>* value);

    template <typename TableStruct, typename Flags, typename Key>
    auto Get(const std::string& table_name, Flags flags, const Key& key);

    template <typename Flags, typename T>
    Status Delete(const std::string& table_name, Flags flags, const T& value);

    template <typename Flags, typename Value>
    Status Update(const std::string& table_name, Flags flags,
                  const Value& old_value, const Value& new_value);

    bool DeleteTable(const std::string& table_name) {
        return sql_db_->DeleteTable(table_name);
    }

    void Destory();

    ~DB() { Destory(); }
 protected:
    explicit DB(const std::string& db_name, DBType db_type);

 private:
    std::unique_ptr<SqlDB> sql_db_;
    std::unique_ptr<Table> table_;
    DISALLOW_COPY_AND_ASSIGN(DB);

};

template<typename TableStruct>
Status DB::Put(const std::string &table_name, const TableStruct &value) {
    if (!table_->IsExistTable(table_name)) {
       Status status = table_->Create(table_name);
       if (!status.ok())
           return status;
    }
    return table_->Put(table_name, value);
}

template<typename TableStruct, typename Flags, typename Key>
Status DB::Get(const std::string &table_name,
               Flags flags, const Key &key,
               std::vector<TableStruct> *value) {
    return table_->Get(table_name, flags, key, value);
}

template<typename TableStruct, typename Flags, typename Key>
auto DB::Get(const std::string &table_name,
             Flags flags, const Key &key) {
    std::vector<TableStruct> value;
    if (!table_->Get(table_name, flags, key, &value).ok())
        throw std::runtime_error("Get is error!");
    return value;
}

template<typename Flags, typename T>
Status DB::Delete(const std::string &table_name,
                  Flags flags, const T &value) {
    return table_->Delete(table_name, flags, value);
}

template<typename Flags, typename Value>
Status DB::Update(const std::string &table_name,
                  Flags flags,
                  const Value &old_value,
                  const Value &new_value) {
    return table_->Update(table_name, flags, old_value, new_value);
}



}   // namespace db
}   // namespace footbook
#endif //CAMPUS_CHAT_DB_H
