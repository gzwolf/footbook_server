//
// Created by YangGuang on 2019-04-16.
//

#ifndef CAMPUS_CHAT_TABLE_STRUCT_H
#define CAMPUS_CHAT_TABLE_STRUCT_H

#include <string>
#include <sstream>
#include <ostream>

#include "footbook/status.h"

namespace footbook {
namespace db {

enum class ProfileFlags {
    kAccount,
    kName,
    kSchool,
    kStudentNum,
    kSex,
    kFaculty,
    kSpecialty,
    kGrade,
    kEntranceTime,
    kNickName,
    kLocation,
    kWechat
};

enum class IdeaFlags {
    kTitleId,
    kReleaseAccount,
    kTitle,
    kContent,
    kDynamicTime,
    kLike,
    kDislike
};

enum CommentFlags {
    kId,
    kTitleId,
    kAccount,
    kTime,
    kLike,
    kDislike
};

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


    std::string ToInsertSql() const {
        std::string sql = "(account, name, school, student_num, sex,"
                          "faculty, specialty, grade, entrance_time,"
                          "nick_name, location, wechat) values(";

        std::string signal_quotes = "\"";
        std::ostringstream ostr_sql;
        ostr_sql << sql;
        ostr_sql << signal_quotes << account << signal_quotes << ","
                 << signal_quotes << name << signal_quotes << ","
                 << signal_quotes << school << signal_quotes << ","
                 << signal_quotes << student_num << signal_quotes << ","
                 << sex << ","
                 << signal_quotes << faculty << signal_quotes << ","
                 << signal_quotes << specialty << signal_quotes << ","
                 << signal_quotes << grade << signal_quotes << ","
                 << signal_quotes << entrance_time << signal_quotes << ","
                 << signal_quotes << nick_name << signal_quotes << ","
                 << signal_quotes << location << signal_quotes << ","
                 << signal_quotes << wechat << signal_quotes << ")";

        return ostr_sql.str();
    }
};





struct Idea {
    int title_id;
    std::string release_account;
    std::string title;
    std::string content;
    std::string dynamic_time;
    int like;
    int dislike;

    Status GetTableName(std::string* table_name) const {
        return Status::Ok();
    }

    std::string ToInsertSql() const {
        std::string sql = "(title_id, release_account,"
                          "title, content, dynamic_time, like,dislike) values(";
        std::string signal_quotes = "\"";

        std::ostringstream ostr_sql;

        ostr_sql << sql;
        ostr_sql << title_id << ","
                 << signal_quotes << release_account << signal_quotes << ","
                 << signal_quotes << title << signal_quotes << ","
                 << signal_quotes << content << signal_quotes << ","
                 << signal_quotes << dynamic_time << signal_quotes << ","
                 << like << ","
                 << dislike << ")";
        return ostr_sql.str();
    }
};



struct Comment {
    int id;
    int title_id;
    std::string account;
    std::string time;
    int like;
    int dislike;

    Status GetTableName(std::string* table_name) const {
        return Status::Ok();
    }

    std::string ToInsertSql() const {
        std::string sql = "(id, title_id, account, time, like, dislike)"
                          " values(";
        std::string signal_quotes = "\"";
        std::ostringstream ostr_sql;

        ostr_sql << sql;
        ostr_sql << id << ","
                 << title_id << ","
                 << signal_quotes << account << signal_quotes << ","
                 << signal_quotes << time << signal_quotes << ","
                 << like << "," << dislike << ")";
        return ostr_sql.str();
    }
};



std::string FlagsToString(const ProfileFlags& profile_flags);
std::string FlagsToString(const IdeaFlags& idea_flags);
std::string FlagsToString(const CommentFlags& comment_flags);


inline std::string ProfileCreateTableSql() {
    return "(account varchar(16) PRIMARY KEY, name varchar(16), school varchar(40),"
           " student_num varchar(16),sex int, faculty varchar(40), "
           "specialty varchar(40), grade varchar(8), entrance_time varchar(16),"
           "nick_name varchar(64), location varchar(64), wechat varchar(32))";
}

inline std::string IdeaCreateTableSql() {
    return "(title_id varchar(32) PRIMARY KEY, release_account varchar(16),"
           " title varchar(1024), content varchar(64), dynamic_time varchar(64),"
           " like int, dislike int)";
}

inline std::string CommentCreateTableSql() {
    return "(id varchar(32) PRIMARY KEY, title_id varchar(32),"
           " account varchar(16), time varchar(64), like int, "
           "dislike int)";
}

}   // namespace db
}   // namespace footbook
#endif //CAMPUS_CHAT_TABLE_STRUCT_H
