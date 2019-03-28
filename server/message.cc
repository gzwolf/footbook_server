//
// Created by YangGuang on 2019-03-25.
//

#include "message.h"

#include <cstring>

#include "server/config.h"

namespace cchat {

namespace {
constexpr std::size_t kHeaderSize = 20;



}   // namespace

Message::Message(const std::string &str)
    : payload_(str) {
    header_.payload_size = payload_.size();
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

bool EncodeMessage(const Message& msg, std::string* str) {
    if (!str->empty())
        str->clear();
    PutFixed32(str, msg.payload_size());
    PutFixed32(str, msg.type());
    PutFixed32(str, msg.sender());
    PutFixed32(str, msg.receiver());
    PutFixed32(str, msg.status());
    str->append(msg.payload(), msg.payload_size());
    return true;
}


bool EncodeMessage(const Message& msg, char* str, int len) {
    if (str)
        return false;
    // 压缩整个头部进str
    size_t offset = 0;
    size_t cur_len = sizeof(msg.payload_size());
    if (len < offset + cur_len)
        return false;
    EncodeFixed32(str + offset, msg.payload_size());
    offset += cur_len;
    cur_len = sizeof(msg.type());
    if (len < offset + cur_len)
        return false;
    EncodeFixed32(str + offset, msg.type());
    offset += cur_len;
    cur_len = sizeof(msg.sender());
    if (len < offset + cur_len)
        return false;
    EncodeFixed32(str + offset, msg.sender());
    offset += cur_len;
    cur_len = sizeof(msg.receiver());
    if (len < offset + cur_len)
        return false;
    EncodeFixed32(str + offset, msg.receiver());
    offset += cur_len;
    cur_len = sizeof(msg.status());
    if (len < offset + cur_len)
        return false;
    EncodeFixed32(str + offset, msg.status());
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
    uint32_t paload_size = DecodeFixed32(str + offset);
    // 如果真正的数据长度(data_len = len - msg->header_size())小于paload_size,
    // 消息格式错误
    if (len - msg->header_size() < paload_size)
        return false;
    offset += sizeof(uint32_t);
    msg->set_type(DecodeFixed32(str + offset));
    offset += sizeof(uint32_t);
    msg->set_sender(DecodeFixed32(str + offset));
    offset += sizeof(uint32_t);
    msg->set_receiver(DecodeFixed32(str + offset));
    offset += sizeof(uint32_t);
    msg->set_status(DecodeFixed32(str + offset));
    offset += sizeof(uint32_t);
    std::string payload(str + offset, paload_size);
    msg->set_payload(payload);
    return true;
}

bool DecodeMessage(const std::string& str, Message* msg) {
    return DecodeMessage(str.data(), str.size(), msg);
}


}   // namespace cchat