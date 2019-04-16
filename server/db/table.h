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


template<typename TableStruct>
void StringVecToTable(std::vector<std::string> str, TableStruct* table_struct);

template <typename TableStruct>
std::string TableToString(const TableStruct& table_struct);



class Table {
 public:
    static std::unique_ptr<Table*> New(TableType type);
    Table() = default;

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

    virtual bool IsExistTable(const std::string& table_name) = 0;
    virtual Status Create(const std::string& table_name) = 0;

 protected:
    virtual ~Table() {}

 private:
    virtual Status DoGet(const std::string& table_name,
                         int flags, const std::string& key,
                         std::vector<std::vector<std::string>>* value) = 0;
    virtual Status DoPut(const std::string& table_name,
            const std::string& value) = 0;
    virtual Status DoDelete(const std::string& table_name,
            int flags, const std::string& value) = 0;
    virtual Status DoUpdate(const std::string& table_name, int flags,
            const std::string& old_value, const std::string& new_value) = 0;

    DISALLOW_COPY_AND_ASSIGN(Table);
};

template<typename Flags, typename Key, typename Value>
Status Table::Get(const std::string& table_name, Flags flags,
        const Key &key, std::vector<Value> *value_vec) {
    std::vector<std::vector<std::string> > res_vec;
    Status status = DoGet(static_cast<int>(flags), std::to_string(key), &res_vec);
    // 将string 转换成 value
    for (auto& res : res_vec) {
        Value value;
        StringVecToTable(res, &value);
        value_vec->push_back(value);
    }
}

template<typename Value>
Status Table::Put(const std::string& table_name, const Value& value) {
    return DoPut(TableToString(value));
}

template<typename Flags, typename Value>
Status Table::Delete(const std::string& table_name,
        Flags flags, const Value &value) {
    return DoDelete(static_cast<int>(flags), std::to_string(value));
}

template<typename Flags, typename Value>
Status Table::Update(const std::string& table_name,Flags flags,
        const Value &old_value, const Value &new_value) {
    return DoUpdate(static_cast<int>(flags), std::to_string(old_value),
            std::to_string(new_value));
}






}   // namespace db
}   // namespace footbook
#endif //CAMPUS_CHAT_TABLE_H
