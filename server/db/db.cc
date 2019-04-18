//
// Created by YangGuang on 2019-04-15.
//

#include "db.h"

#include <exception>

namespace footbook {
namespace db {

Status DB::Open(const std::string& db_name, std::shared_ptr<DB>* db) {
    try {
        *db = std::shared_ptr<DB>(new DB(db_name));
    } catch (std::exception& err) {
        return Status::DBError(err.what());
    }
    return Status::Ok();
}

DB::DB(const std::string& db_name)
    : table_(Table::New(&mysql_)) {
    if (!mysql_.CreateDatabase(db_name))
        throw std::runtime_error(mysql_.GetLastError());
}

void DB::Destory() {
    mysql_.Close();
}



}   // namespace db
}   // namespace footbook
