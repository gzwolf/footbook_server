//
// Created by YangGuang on 2019-04-20.
//

#include "glog/logging.h"
#include "gtest/gtest.h"

#include "footbook/limit.h"

TEST(Limit, ConstructTest) {
    footbook::Limit<int> limit_t(100, 200);
    footbook::Limit<double> limit_d(100.0, 200.0);
    footbook::Limit<int> limit_t2(limit_t);
    footbook::Limit<int> limit_t3;
    limit_t3 = limit_t2;
    EXPECT_EQ(limit_t.max(), 200);
    EXPECT_EQ(limit_t.min(), 100);
    EXPECT_EQ(limit_d.max(), 200.0);
    EXPECT_EQ(limit_d.min(), 100.0);
    EXPECT_EQ(limit_t2.max(), 200);
    EXPECT_EQ(limit_t2.min(), 100);
    EXPECT_EQ(limit_t3.max(), 200);
    EXPECT_EQ(limit_t3.min(), 100);
}

TEST(Limit, IntegerTest) {
    footbook::Limit<int> limit(1000, 2000);
    EXPECT_EQ(limit.max(), 2000);
    EXPECT_EQ(limit.min(), 1000);
    EXPECT_TRUE(limit.IsAmongLimit(1500));
    EXPECT_TRUE(limit.IsAmongLimit(1900));
    EXPECT_TRUE(limit.IsAmongLimit(1000));
    EXPECT_FALSE(limit.IsAmongLimit(2000));
    EXPECT_FALSE(limit.IsAmongLimit(999));
}

TEST(Limit, FloatTest) {
    footbook::Limit<double> limit(99.4, 1000.4);
    EXPECT_EQ(limit.min(), 99.4);
    EXPECT_EQ(limit.max(), 1000.4);
    EXPECT_TRUE(limit.IsAmongLimit(100.0));
    EXPECT_TRUE(limit.IsAmongLimit(600.5));
    EXPECT_TRUE(limit.IsAmongLimit(99.4));
    EXPECT_FALSE(limit.IsAmongLimit(1000.4));
    EXPECT_FALSE(limit.IsAmongLimit(1000.6));
}

