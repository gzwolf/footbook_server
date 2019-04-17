//
// Created by YangGuang on 2019-04-17.
//

#include "table_struct.h"

namespace footbook {
namespace db {

std::string FlagsToString(const ProfileFlags &profile_flags) {
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
        case ProfileFlags::kGrade:
            res = "kGrade";
            break;
        case ProfileFlags::kEntranceTime:
            res = "entrance_time";
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

std::string FlagsToString(const IdeaFlags& idea_flags) {
    std::string res;
    switch (idea_flags) {
        case IdeaFlags::kTitleId:
            res = "titile_id";
            break;
        case IdeaFlags::kReleaseAccount:
            res = "release_account";
            break;
        case IdeaFlags::kTitle:
            res = "title";
            break;
        case IdeaFlags::kContent:
            res = "content";
            break;
        case IdeaFlags::kDynamicTime:
            res = "dynamic_time";
            break;
        case IdeaFlags::kLike:
            res = "like";
            break;
        case IdeaFlags::kDislike:
            res = "dislike";
            break;
        default:
            throw std::runtime_error("idea_flags error");
    }
    return res;
}

std::string FlagsToString(const CommentFlags& comment_flags) {
    std::string res;
    switch (comment_flags) {
        case CommentFlags::kId:
            res = "id";
            break;
        case CommentFlags::kTitleId:
            res = "title_id";
            break;
        case CommentFlags::kAccount:
            res = "account";
            break;
        case CommentFlags::kTime:
            res = "time";
            break;
        case CommentFlags::kLike:
            res = "like";
            break;
        case CommentFlags::kDislike:
            res = "dislike";
            break;
        default:
            throw std::runtime_error("comment_flags error");
    }
    return res;
}

}   // namespace db
}   // namespace footbook