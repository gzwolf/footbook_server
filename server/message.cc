//
// Created by YangGuang on 2019-03-25.
//

#include "message.h"

namespace cchat {

Message::Message(const std::string &str)
    : payload_(str) {
}

Message::Message(const char *data, int data_len)
    : payload_(data, data_len){

}

Message::Message(uint32_t type) {
    header_.type = type;
}

Message::Message() {

}

Message::~Message() {

}

bool EncodeMessage(const Message& msg, std::string& str) {
    return false;
}
bool EncodeMessage(const Message& msg, char* str) {
    return false;
}

bool DecodeMessage(const std::string& str, Message* msg) {
    return false;
}
bool EncodeMessage(const char* str, Message* msg) {
    return false;
}

}   // namespace cchat