//
// Created by YangGuang on 2019-04-15.
//

#include "table.h"

#include <algorithm>

#include "mysql_interface.h"
#include "glog/logging.h"

namespace footbook {
namespace db {

namespace {
const char kProfileTableSuffix[] = "profile";
const char kIdeaTableSuffix[] = "idea";
const char kCommentTableSuffix[] = "comment";
const char kTableSeparator = '_';

}

std::string GetProfileTableName(const std::string& school) {
    // Profile table name format : 汉口学院_profile
    return school + kTableSeparator + kProfileTableSuffix;
}

std::string GetIdeaTableName(const std::string& school) {
    // Idea table name format : 汉口学院_idea
    return school + kTableSeparator + kIdeaTableSuffix;
}

std::string GetCommentTableName(const std::string& school,
                                const std::string& time) {
    // comment table name format : 汉口学院2020_comment
    return school + time + kTableSeparator + kCommentTableSuffix;
}

bool Table::IsExistTable(const std::string &table_name) {
   return mysql_->IsExistTable(table_name);
}

Status Table::Create(const std::string &table_name) {
    auto pos = table_name.find(kTableSeparator);
    if (pos == std::string::npos)
        return Status::InvalidData("table_name is invalid!");
    std::string table_suffix = table_name.substr(pos + 1);
    std::string sql;
    if (table_suffix == kProfileTableSuffix) {
        sql = ProfileCreateTableSql();
    } else if (table_suffix == kIdeaTableSuffix) {
        sql = IdeaCreateTableSql();
    } else if (table_suffix == kCommentTableSuffix) {
        sql = CommentCreateTableSql();
    } else {
        return Status::InvalidData("table_name is invaild!");
    }

    if (mysql_->CreateTable(sql))
        return Status::DBError(mysql_->GetLastError());
    return Status::Ok();
}

void Table::ToTableStruct(const std::vector<std::string> &str_vec,
                          Profile *profile) {
    DCHECK(str_vec.size() >= 12);

    profile->account = str_vec.at(0);
    profile->name = str_vec.at(1);
    profile->school = str_vec.at(2);
    profile->student_num = str_vec.at(3);
    profile->sex = atoi(str_vec.at(4).c_str());
    profile->faculty = str_vec.at(5);
    profile->specialty = str_vec.at(6);
    profile->grade = str_vec.at(7);
    profile->entrance_time = str_vec.at(8);
    profile->nick_name = str_vec.at(9);
    profile->location = str_vec.at(10);
    profile->wechat = str_vec.at(11);
}



void Table::ToTableStruct(const std::vector<std::string> &str_vec,
                          Idea *idea) {
    DCHECK(str_vec.size() >= 7);

    idea->title_id = atoi(str_vec.at(0).c_str());
    idea->release_account = str_vec.at(1);
    // 这两行采用move操作，由于数据量有点大，如果所有字段都采用move操作
    // 显得有点麻烦.
    idea->title = std::move(const_cast<std::string&>(str_vec[2]));
    idea->content = std::move(const_cast<std::string&>(str_vec[3]));
    idea->dynamic_time = str_vec.at(4);
    idea->like = atoi(str_vec.at(5).c_str());
    idea->dislike = atoi(str_vec.at(6).c_str());
}

void Table::ToTableStruct(const std::vector<std::string> &str_vec,
                          Comment *comment) {
    DCHECK(str_vec.size() >= 6);

    comment->id = atoi(str_vec.at(0).c_str());
    comment->title_id = atoi(str_vec.at(1).c_str());
    comment->account = str_vec.at(2);
    comment->time = str_vec.at(3);
    comment->like = atoi(str_vec.at(4).c_str());
    comment->dislike = atoi(str_vec.at(5).c_str());
}

Table::~Table() {

}


Table::Table(MysqlInterface *mysql)
    : mysql_(mysql) {
}

std::unique_ptr<Table> Table::New(MysqlInterface *mysql) {
    return std::unique_ptr<Table>(new Table(mysql));
}


}   // namespace db
}   // namespace footbook
