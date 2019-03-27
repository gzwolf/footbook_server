//
// Created by YangGuang on 2019-03-25.
//

#include "message.h"

#include <cstring>

namespace cchat {

namespace {
constexpr std::size_t kHeaderSize = 17;
}

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

std::size_t Message::header_size() {
    return kHeaderSize;
}

template <typename T>
void EncodeFixedInteget(char* buf, T value) {
    memcpy(buf, &value, sizeof(T));
}

template <typename T>
auto DecodeFixedInteger(const char* ptr) {
    T result;
    memcpy(&result, ptr, sizeof(T));
    return result;
}

bool EncodeMessage(const Message& msg, char* str, int len) {
    if (str)
        return false;
    // 压缩整个头部进str
    size_t offset = 0;
    size_t cur_len = sizeof(msg.payload_size());
    if (len < offset + cur_len)
        return false;
    EncodeFixedInteget(str + offset, msg.payload_size());
    offset += cur_len;
    cur_len = sizeof(msg.type());
    if (len < offset + cur_len)
        return false;
    EncodeFixedInteget(str + offset, msg.type());
    offset += cur_len;
    cur_len = sizeof(msg.sender());
    if (len < offset + cur_len)
        return false;
    EncodeFixedInteget(str + offset, msg.sender());
    offset += cur_len;
    cur_len = sizeof(msg.receiver());
    if (len < offset + cur_len)
        return false;
    EncodeFixedInteget(str + offset, msg.receiver());
    offset += cur_len;
    cur_len = sizeof(msg.flags());
    if (len < offset + cur_len)
        return false;
    EncodeFixedInteget(str + offset, msg.flags());
    offset += cur_len;
    cur_len = msg.payload_size();
    if (len < offset + cur_len)
        return false;

    // 复制payload 到str.
    memcpy(str + offset, msg.payload(), cur_len);
    return true;
}

bool DecodeMessage(const char* str, std::size_t len, Message* msg) {
    // 如果str的长度比头部长度都小，消息格式错误.
    if (len < msg->header_size())
        return false;

    // offset记录着当前str所在的偏移位置.
    size_t offset = 0;
    uint32_t paload_size = DecodeFixedInteger<uint32_t>(str + offset);
    // 如果真正的数据长度(data_len = len - msg->header_size())小于paload_size,
    // 消息格式错误
    if (len - msg->header_size() < paload_size)
        return false;
    offset += sizeof(uint32_t);
    msg->set_type(DecodeFixedInteger<uint32_t>(str + offset));
    offset += sizeof(uint32_t);
    msg->set_sender(DecodeFixedInteger<int32_t>(str + offset));
    offset += sizeof(int32_t);
    msg->set_receiver(DecodeFixedInteger<int32_t>(str + offset));
    offset += sizeof(int32_t);
    msg->set_flags(DecodeFixedInteger<uint8_t>(str + offset));
    offset += sizeof(uint8_t);
    msg->set_payload(str + offset, paload_size);
    return true;
}

bool DecodeMessage(const std::string& str, Message* msg) {
    return DecodeMessage(str.data(), str.size(), msg);
}


}   // namespace cchat