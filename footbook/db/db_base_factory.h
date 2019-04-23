//
// Created by YangGuang on 2019-04-23.
//

#ifndef FOOTBOOK_SERVER_DB_BASE_FACTORY_H
#define FOOTBOOK_SERVER_DB_BASE_FACTORY_H

namespace footbook {
namespace db {

enum class DBType {
    kMysql,
    kOracle,
    kLevedb
};

class SqlDB;
class DBBaseFactory {
 public:
    virtual SqlDB* CreateSqlDB() = 0;
    virtual ~DBBaseFactory() = default;
};

DBBaseFactory* NewDBFactory(DBType db_type);

}
}   // namespace footbook
#endif //FOOTBOOK_SERVER_DB_BASE_FACTORY_H
