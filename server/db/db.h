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

struct Profile {
   std::string account;
   std::string name;
   std::string school;
   std::string student_num;
   int sex;
   std::string faculty;
   std::string specialty;
   std::string grade;
   std::string entrance_time;
   std::string nick_name;
   std::string location;
   std::string wechat;
};

struct Idea {
    int title_id;
    std::string release_account;
    std::string title;
    std::string content;
    std::string dynamic_time;
    uint64_t like;
    uint64_t dislike;
};

struct Comment {
    int id;
    int title_id;
    std::string account;
    std::string time;
    uint64_t like;
    uint64_t dislike;
};

class DB {
 public:
    static DB* New();

    Status Open(const char* db_name);

    Status Put(Table* table);

    template <typename TableStruct, typename Flags,typename Key>
    Status Get(Table* table, Flags flags, const Key& key,
            std::vector<TableStruct>* value);

    template <typename Flags, typename T>
    Status Delete(Table* table, Flags flags, const T& value);

    template <typename Flags, typename Value>
    Status Update(Table* table, Flags flags, const Value& old_value, const Value& new_value);

 private:
    DB() = default;


    DISALLOW_COPY_AND_ASSIGN(DB);

};

}   // namespace db
}   // namespace footbook
#endif //CAMPUS_CHAT_DB_H
