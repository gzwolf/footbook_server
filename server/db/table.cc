//
// Created by YangGuang on 2019-04-15.
//

#include "table.h"

#include "mysql_interface.h"

namespace footbook {
namespace db {


class ProfileTable : public Table {
 public:
    explicit ProfileTable(MysqlInterface* mysql)
        : mysql_(mysql), Table() {}

    bool IsExistTable(const std::string& table_name) override {
        return mysql_->IsExistTable(table_name);
    }

    Status Create(const std::string& table_name) override {
        if (mysql_->CreateTable(table_name)) {
            return Status::Ok();
        } else {
            return Status::HttpError("Create table "
            + table_name + "error : " + mysql_->GetLastError());
        }
    }

 protected:
    ~ProfileTable() override {
    }

 private:
    Status DoGet(const std::string& table_name,
                 int flags,
                 const std::string& value,
                 std::vector<std::vector<
                 std::string>>* table_struct) override {
        std::string signal_quotes = "\"";
        std::string flags_name = FlagsToString(static_cast<ProfileFlags>(flags));
        std::string sql = "select * from " + table_name + " where "
                + flags_name + " = " + signal_quotes + value + signal_quotes;

        std::vector <std::vector<std::string>> select_result;
        bool result = mysql_->ReadData(sql.c_str(), select_result);

        if (!result || select_result.empty())
            return Status::HttpError(mysql_->GetLastError());
        return Status::Ok();
    }

    Status DoPut(const std::string& table_name,
                 const std::string& value) override {

    }

    Status DoDelete(const std::string& table_name,
                    int flags,
                    const std::string& value) override {

    }

    Status DoUpdate(const std::string& table_name,
                    int flags, const std::string& old_value,
                    const std::string& new_value) override {

    }

    MysqlInterface* mysql_;
    DISALLOW_COPY_AND_ASSIGN(ProfileTable);
};

std::unique_ptr<Table *> Table::New(TableType type) {
    return std::unique_ptr<Table *>();
}

Table::~Table() {

}



}   // namespace db
}   // namespace footbook
