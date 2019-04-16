//
// Created by YangGuang on 2019-04-16.
//

#ifndef CAMPUS_CHAT_TABLE_STRUCT_H
#define CAMPUS_CHAT_TABLE_STRUCT_H

#include <string>

#include "server/status.h"

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

    // 调用这个函数的时候必须初始化了
    Status GetTableName(std::string* table_name) const {
       if (school.empty())
           return Status::MsgError("school is null!");
       table_name->clear();
       *table_name = school + "_profile";
       return Status::Ok();
    }
};

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

struct Idea {
    int title_id;
    std::string release_account;
    std::string release_school;
    std::string title;
    std::string content;
    std::string dynamic_time;
    uint64_t like;
    uint64_t dislike;

    Status GetTableName(std::string* table_name) const {
        if (release_school.empty())
            return Status::MsgError("school is null!");
        table_name->clear();
        *table_name = release_school + "_idea";
        return Status::Ok();
    }
};

struct Comment {
    std::string release_school;
    int id;
    int title_id;
    std::string account;
    std::string time;
    uint64_t like;
    uint64_t dislike;

    Status GetTableName(std::string* table_name) const {
        if (release_school.empty() || time.empty())
            return Status::MsgError("school is null!");
        table_name->clear();
        *table_name = release_school + time + "_comment";
        return Status::Ok();
    }
};

std::string FlagsToString(const ProfileFlags& profile_flags) {
    std::string res;
    switch (profile_flags) {
        case ProfileFlags::kAccount:
            res = "account";
            break;
        case ProfileFlags::kName:
            res = "name";
            break;
        case ProfileFlags::kSchool:
            res = "school";
            break;
        case ProfileFlags::kStudentNum:
            res = "student_num";
        case ProfileFlags::kSex:
            res = "sex";
            break;
        case ProfileFlags::kFaculty:
            res = "faculty";
            break;
        case ProfileFlags::kSpecialty:
            res = "specialty";
            break;
        case ProfileFlags::kNickName:
            res = "nick_name";
            break;
        case ProfileFlags::kLocation:
            res = "location";
        case ProfileFlags::kWechat:
            res = "wechat";
        default:
            throw std::runtime_error("profile_flags error");
    }
    return res;
}

}   // namespace db
}   // namespace footbook
#endif //CAMPUS_CHAT_TABLE_STRUCT_H
