//
// Created by YangGuang on 2019-04-15.
//

#ifndef CAMPUS_CHAT_TABLE_H
#define CAMPUS_CHAT_TABLE_H

#include <string>
#include <vector>
#include <memory>

#include "base/macor.h"
#include "footbook/db/table_struct.h"
#include "footbook/status.h"
#include "mysql_interface.h"

namespace std {
inline string to_string(string val) {
    // convert int to string
    return val;
}
}   // namespace std

namespace footbook {
namespace db {

// 数据库都表名是动态的，根据学校分表，所以需要动态创建，其中Profile
// 表根据学校分表，Idea也是根据学校分表，Comment根据学校+评论时间分表.
std::string GetProfileTableName(const std::string& school);
std::string GetIdeaTableName(const std::string& school);
std::string GetCommentTableName(const std::string& school,
                                const std::string& time);

class Table {
 public:
    // 传入MysqlInterface*，和传入的mysql共享同一个MysqlInterface,
    // 离开时不用delete这个指针
    static std::unique_ptr<Table> New(MysqlInterface* mysql);

    // 表基本操作，从数据库中获取指定到key值所对应到数据，table_name
    // 表示key值所在到表，flags表示key值到类型，最终会将结果放入
    // value_vec中，返回值Status如果是ok()，代表成功，否则失败，失败原因
    // 请查看Status类. 调用Status.ToString()获取错误信息.
    // Require : IsExistTable() is true.
    template <typename Flags, typename Key, typename Value>
    Status Get(const std::string& table_name, Flags flags,
            const Key& key, std::vector<Value>* value_vec);

    // 往表中放入一条数据，Value必须是table_sturct中到三中结构体,
    // 详情查看table_struct.h,table_name表示需要插入的表名, value
    // 表示插入的数据, 返回值Status如果是ok()，代表成功，否则失败，失败原因
    // 请查看Status类. 调用Status.ToString()获取错误信息.
    // Require : IsExistTable() is true.
    template <typename Value>
    Status Put(const std::string& table_name,
            const Value& value);

    // 从表中删除一条数据，table_name 表示需要删除的字段对应的表名，flags
    // 表示value的字段类型，value表示需要删除的值 返回值Status如果是ok()，
    // 代表成功，否则失败，失败原因 请查看Status类.调用Status.ToString()
    // 获取错误信息.
    // Require : IsExistTable() is true.
    template <typename Flags, typename Value>
    Status Delete(const std::string& table_name,
            Flags flags, const Value& value);

    // 修改表中数据，table_name 表示需要修改的字段对应的表名，flags
    // 表示value的字段类型，ole_value代表以前的值，new_value新值，
    // 返回值Status如果是ok()，代表成功，否则失败，失败原因 请查看Status类
    // 调用Status.ToString() 获取错误信息.
    // Require : IsExistTable() is true.
    template <typename Flags, typename Value>
    Status Update(const std::string& table_name, Flags flags,
            const Value& old_value, const Value& new_value);
    // 判断表是否存在，在调用这个类的表操作函数时需要确保表已经存在
    bool IsExistTable(const std::string& table_name);
    // 表不存在可以调用这个函数来创建表
    Status Create(const std::string& table_name);
    Status Destroy(const std::string& table_name);

    ~Table();

 protected:
    explicit Table(MysqlInterface* mysql);
 private:
    // 将从mysql查询到到结果转换成对应到表数据结构
    void ToTableStruct(const std::vector<std::string>& str_vec,
                        Profile* profile);
    void ToTableStruct(const std::vector<std::string>& str_vec,
                        Idea* idea);
    void ToTableStruct(const std::vector<std::string>& str_vec,
                        Comment* comment);

    MysqlInterface* mysql_;
    DISALLOW_COPY_AND_ASSIGN(Table);
};

template<typename Flags, typename Key, typename Value>
Status Table::Get(const std::string& table_name, Flags flags,
        const Key &key, std::vector<Value> *value_vec) {
    std::string signal_quotes = "\"";
    std::string flags_name = FlagsToString(flags);
    std::string sql = "select * from " + table_name + " where " + flags_name +
            " = " + signal_quotes + std::to_string(key) + signal_quotes;

    std::vector <std::vector<std::string>> select_result;
    bool result = mysql_->ReadData(sql, select_result);

    if (!result || select_result.empty())
        return Status::DBError(mysql_->GetLastError());
    for (auto& vec : select_result) {
        Value value;
        ToTableStruct(vec, &value);
        value_vec->push_back(std::move(value));
    }
    return Status::Ok();
}

template<typename Value>
Status Table::Put(const std::string& table_name, const Value& value) {
    std::string sql = "insert into " + table_name + value.ToInsertSql();

    if (!mysql_->WriteData(sql))
        return Status::HttpError(mysql_->GetLastError());
    return Status::Ok();
}

template<typename Flags, typename Value>
Status Table::Delete(const std::string& table_name,
        Flags flags, const Value &value) {
    std::string signal_quotes = "\"";
    std::string sql = "delete from " + table_name + "where"
            + FlagsToString(flags) + " = " + signal_quotes
            + std::to_string(value) + signal_quotes;
    if (!mysql_->DeleteData(sql))
        return Status::DBError(mysql_->GetLastError());
    return Status::Ok();
}

template<typename Flags, typename Value>
Status Table::Update(const std::string& table_name,Flags flags,
        const Value &old_value, const Value &new_value) {
    std::string signal_quotes = "\"";
    std::string sql = "update into " + table_name + "set "
            + FlagsToString(flags) + " = " + std::to_string(new_value)
            + "where " + FlagsToString(flags) + " = " + std::to_string(old_value);
    if (!mysql_->ModifyData(sql))
        return Status::DBError(mysql_->GetLastError());
    return Status::Ok();
}




}   // namespace db
}   // namespace footbook
#endif //CAMPUS_CHAT_TABLE_H
