//
// Created by YangGuang on 2019-04-19.
//
#include <string>

#include "gtest/gtest.h"
#include "glog/logging.h"
#include "footbook/message.h"

TEST(Message, FaildMessage) {
    std::string str = "I love you";
    footbook::Message msg(str);
    msg.set_sender(0);
    msg.set_receiver(0);
    msg.set_type(0);
    msg.set_status(0);
    std::string encode_str;
    footbook::Message decode_msg;

    EXPECT_TRUE(footbook::EncodeMessage(msg, &encode_str));
    EXPECT_TRUE(footbook::DecodeMessage(encode_str, &decode_msg));

    EXPECT_EQ(decode_msg.sender(), msg.sender());
    EXPECT_EQ(decode_msg.receiver(), msg.receiver());
    EXPECT_EQ(decode_msg.type(), msg.type());
    EXPECT_EQ(decode_msg.status(), msg.status());
    EXPECT_EQ(decode_msg.payload(), msg.payload());
}

TEST(Message, DecodeAndEncodeForMessage) {
    // 测试消息编解码
    std::string str = "I love you!";
    footbook::Message msg(str);
    msg.set_sender(13697163989);
    msg.set_receiver(17771611074);
    msg.set_status(8);
    msg.set_type(0x12);
    std::string encode_str;
    footbook::Message decode_msg;

    EXPECT_TRUE(footbook::EncodeMessage(msg, &encode_str));
    EXPECT_TRUE(footbook::DecodeMessage(encode_str, &decode_msg));

    EXPECT_EQ(decode_msg.sender(), msg.sender());
    EXPECT_EQ(decode_msg.receiver(), msg.receiver());
    EXPECT_EQ(decode_msg.type(), msg.type());
    EXPECT_EQ(decode_msg.status(), msg.status());
    EXPECT_EQ(decode_msg.payload(), msg.payload());
}

TEST(Message, PayloadTest) {
    std::string payload;
    std::string key = "user_name";
    std::string value = "17771611074";
    EXPECT_TRUE(footbook::PutPayloadPart(key, value, &payload));

    key.clear();
    value.clear();
    key = "password";
    value = "123456";
    EXPECT_TRUE(footbook::PutPayloadPart(key, value, &payload));

    key.clear();
    value.clear();
    key = "verification_code";
    value = "320123";
    EXPECT_TRUE(footbook::PutPayloadPart(key, value, &payload));


    key.clear();
    value.clear();
    key = "verification_codeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee"
          "eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee"
          "eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee"
          "eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee";
    value = "320123";
    EXPECT_FALSE(footbook::PutPayloadPart(key, value, &payload));

    std::map<std::string, std::string> res;
    EXPECT_TRUE(footbook::DecodePayload(payload, &res));

    EXPECT_EQ(res.size(), 3);
    EXPECT_EQ(res["user_name"], "17771611074");
    EXPECT_EQ(res["password"], "123456");
    EXPECT_EQ(res["verification_code"], "320123");
}
