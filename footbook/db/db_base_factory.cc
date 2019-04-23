//
// Created by YangGuang on 2019-04-23.
//

#include "footbook/db/db_base_factory.h"

#include "sql_db.h"
#include "mysql_interface.h"

namespace footbook {
namespace db {

class MysqlFactory : public DBBaseFactory {
    SqlDB* CreateSqlDB() override {
        return new MysqlInterface;
    }
};


DBBaseFactory* NewDBFactory(DBType db_type) {
    DBBaseFactory* db_factory = nullptr;
    switch (db_type) {
        case DBType::kMysql:
            db_factory = new MysqlFactory;
            break;
        case DBType::kOracle:
            // ...
            break;
        case DBType::kLevedb:
            //...
            break;
    }
    return db_factory;
}

}   // namespace db
}   // namespace footbook



