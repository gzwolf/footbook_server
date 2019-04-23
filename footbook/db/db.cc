//
// Created by YangGuang on 2019-04-15.
//

#include "db.h"

#include <exception>

#include "footbook/db/db_base_factory.h"
#include "footbook/db/sql_db.h"
#include "glog/logging.h"

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
    : sql_db_(SqlDB::New(NewDBFactory(DBType::kMysql))),
      table_(Table::New(sql_db_.get())) {
    DCHECK(sql_db_);
    DCHECK(table_);
    if (!sql_db_->CreateDatabase(db_name))
        throw std::runtime_error(sql_db_->GetLastError());
}

void DB::Destory() {
    sql_db_->Close();
}



}   // namespace db
}   // namespace footbook
