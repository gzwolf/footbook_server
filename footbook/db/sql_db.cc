//
// Created by YangGuang on 2019-04-23.
//

#include "sql_db.h"

#include "db_base_factory.h"

namespace footbook {
namespace db {


SqlDB *SqlDB::New(DBBaseFactory *factory) {
    return factory->CreateSqlDB();
}

}   // namespace db
}   // namespace footbook