//
// Created by YangGuang on 2019-04-25.
//

#include "gtest/gtest.h"

#include "footbook/client.h"

void OnLogin(const footbook::Status& status) {
    EXPECT_TRUE(status.ok());
}

TEST(Client, LoginTest) {
    footbook::Client::GetInstance();
    std::string key, value;
    key = "17771611074";
    value = "123456789";

    footbook::Client::GetInstance()->leveldb_->Put(leveldb::WriteOptions(), key, value);

    footbook::Client::GetInstance()->Login(key, value, &OnLogin);
}

