//
// Created by YangGuang on 2019-04-15.
//

#ifndef CAMPUS_CHAT_DB_H
#define CAMPUS_CHAT_DB_H

#include <string>
#include <map>

#include "base/macor.h"
#include "server/db/mysql_interface.h"
#include "server/status.h"
#include "server/db/table.h"


namespace footbook {
namespace db {



class DB {
 public:
    static DB* New();

    Status Open(const char* db_name);

    template <typename TableStruct>
    Status Put(TableType table_type, const TableStruct& value);

    template <typename TableStruct, typename Flags, typename Key>
    Status Get(TableType table_type, Flags flags, const Key& key,
            std::vector<TableStruct>* value);

    template <typename Flags, typename T>
    Status Delete(TableType table_type, Flags flags, const T& value);

    template <typename Flags, typename Value>
    Status Update(TableType table, Flags flags,
            const Value& old_value, const Value& new_value);

 private:
    DB() = default;

    std::map<TableType, std::unique_ptr<Table*>> tables_;
    DISALLOW_COPY_AND_ASSIGN(DB);

};

}   // namespace db
}   // namespace footbook
#endif //CAMPUS_CHAT_DB_H
