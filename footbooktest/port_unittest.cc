//
// Created by YangGuang on 2019-04-20.
//

#include "glog/logging.h"
#include "gtest/gtest.h"
#include "footbook/port.h"

TEST(Port, EndianTest) {
    LOG(INFO) << port::IsLittleEndian();
}

TEST(Port, RandomTest) {
    int pre_value = port::Random(footbook::Limit<int>(100000, 999999));
    for (int i = 0; i < 30; i++) {
        int cur_value = port::Random(footbook::Limit<int>(100000, 999999));
        EXPECT_NE(pre_value, cur_value);
        pre_value = cur_value;
    }
}

