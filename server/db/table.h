//
// Created by YangGuang on 2019-04-15.
//

#ifndef CAMPUS_CHAT_TABLE_H
#define CAMPUS_CHAT_TABLE_H

#include <string>
#include <vector>

#include "server/status.h"

namespace footbook {
namespace db {

template <typename Type>
void StringToTable(std::string str, Type* type);

class Table {
 public:
    template <typename Flags, typename Key, typename Value>
    Status Get(Flags flags, const Key& key, std::vector<Value>* value_vec) {
        std::vector<std::string> res_vec;
        Status status = DoGet(static_cast<int>(flags), &res_vec);
        // 将string 转换成 value
        for (auto& res : res_vec) {
            Value value;
            StringToTable(res, &value);
            value_vec->push_back(value);
        }
    }

    template <typename Value>
    Status Put(Value value);

    template <typename Flags, typename Value>
    Status Delete(Flags flags, const Value& value);

    template <typename Flags, typename Value>
    Status Update(Flags flags, const Value& old_value, const Value& new_value);

 private:
    virtual Status DoGet(int flags, std::vector<std::string>* value) = 0;
    virtual Status DoPut(const std::string& value) = 0;
    virtual Status DoDelete(int flags, const std::string& value) = 0;
    virtual Status DoUpdate(int flags, const std::string& old_value,
            const std::string& new_value) = 0;
    virtual std::string ToString() const = 0;
};


}   // namespace db
}   // namespace footbook
#endif //CAMPUS_CHAT_TABLE_H
