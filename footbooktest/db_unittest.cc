//
// Created by YangGuang on 2019-04-22.
//

#include "glog/logging.h"
#include "gtest/gtest.h"
#include "footbook/db/db.h"

void ProfileInitOne(footbook::db::Profile* profile) {
    DCHECK(profile);
    profile->account = "17771611074";
    profile->school = "汉口学院";
    profile->name = "YangGuang";
    profile->location = "WuHan";
    profile->entrance_time = "2015";
    profile->nick_name = "Sunshine";
    profile->specialty = "CS";
    profile->faculty = "CS";
    profile->grade = "2";
    profile->sex = 0;
    profile->student_num = "2015911074";
    profile->wechat = "123456";
}

void ProfileInitTo(footbook::db::Profile* profile) {
    DCHECK(profile);
    profile->account = "13371611074";
    profile->school = "汉口学院";
    profile->name = "YanngWuZhou";
    profile->location = "WuHan";
    profile->entrance_time = "2015";
    profile->nick_name = "YangWuChuan";
    profile->specialty = "CS";
    profile->faculty = "CS";
    profile->grade = "4";
    profile->sex = 0;
    profile->student_num = "2015911075";
    profile->wechat = "12345678";
}

TEST(DB, ProfileTableTest) {
    footbook::db::Profile profile;
    ProfileInitOne(&profile);
    std::shared_ptr<footbook::db::DB> db;
    auto status = footbook::db::DB::Open("footbook",
            footbook::db::DBType::kMysql, &db);
    EXPECT_TRUE(status.ok());
    std::string table_name = footbook::db::GetProfileTableName(profile.school);
    status = db->Put(table_name, profile);
    EXPECT_TRUE(status.ok());
    ProfileInitTo(&profile);
    status = db->Put(table_name, profile);
    EXPECT_TRUE(status.ok());

    std::vector<footbook::db::Profile> cs_res;
    status = db->Get(table_name, footbook::db::ProfileFlags::kSpecialty, "CS", &cs_res);
    EXPECT_TRUE(status.ok());
    EXPECT_FALSE(cs_res.empty());
    EXPECT_TRUE(cs_res.size() == 2);
    auto one_profile = cs_res.at(0);
    EXPECT_EQ(one_profile.account, "13371611074");
    auto to_profile = cs_res.at(1);
    EXPECT_EQ(to_profile.account, "17771611074");

    try {
        cs_res = db->Get<footbook::db::Profile>(table_name,
                footbook::db::ProfileFlags::kStudentNum, "2015911074");
    } catch (std::exception& exc) {
        LOG(INFO) << exc.what();
    }

    EXPECT_FALSE(cs_res.empty());
    EXPECT_TRUE(cs_res.size() == 1);
    one_profile = cs_res.at(0);
    EXPECT_EQ(one_profile.student_num, "2015911074");
    EXPECT_EQ(one_profile.account, "17771611074");




    bool res = db->DeleteTable(table_name);

    EXPECT_TRUE(res);
}

