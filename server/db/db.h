//
// Created by YangGuang on 2019-04-15.
//

#ifndef CAMPUS_CHAT_DB_H
#define CAMPUS_CHAT_DB_H

#include <string>
#include <memory>

#include "base/macor.h"
#include "server/db/mysql_interface.h"
#include "server/status.h"
#include "server/db/table.h"

namespace footbook {
namespace db {

class DB {
 public:
    static Status Open(const std::string& db_name, std::shared_ptr<DB>* db);

    template <typename TableStruct>
    Status Put(const std::string& table_name, const TableStruct& value);

    template <typename TableStruct, typename Flags, typename Key>
    Status Get(const std::string& table_name, Flags flags, const Key& key,
               std::vector<TableStruct>* value);

    template <typename Flags, typename T>
    Status Delete(const std::string& table_name, Flags flags, const T& value);

    template <typename Flags, typename Value>
    Status Update(const std::string& table_name, Flags flags,
                  const Value& old_value, const Value& new_value);

    void Destory();

    ~DB() { Destory(); }
 protected:
    explicit DB(const std::string& db_name);

 private:
    std::unique_ptr<Table> table_;
    MysqlInterface mysql_;
    DISALLOW_COPY_AND_ASSIGN(DB);

};

template<typename TableStruct>
Status DB::Put(const std::string &table_name, const TableStruct &value) {
    if (!table_->IsExistTable(table_name))
        table_->Create(table_name);
    return table_->Put(table_name, value);
}

template<typename TableStruct, typename Flags, typename Key>
Status DB::Get(const std::string &table_name,
               Flags flags, const Key &key,
               std::vector<TableStruct> *value) {
    return table_->Get(table_name, flags, key, value);
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
