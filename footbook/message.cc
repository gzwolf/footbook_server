//
// Created by YangGuang on 2019-03-25.
//

#include "message.h"

#include <cstring>

#include "footbook/config.h"

namespace footbook {

namespace {
constexpr std::size_t kHeaderSize = 17;



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
    PutFixed8(str, msg.status());
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
    //EncodeFixed32(str + offset, msg.status());
    auto status = msg.status();
    memcpy(str + offset, &status, cur_len);
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
    msg->set_status(DecodeFixed8(str + offset));
    offset += sizeof(uint8_t);
    std::string payload(str + offset, paload_size);
    msg->set_payload(payload);
    return true;
}

bool DecodeMessage(const std::string& str, Message* msg) {
    return DecodeMessage(str.data(), str.size(), msg);
}

bool PutPayloadPart(const std::string& key,
                    const std::string& value,
                    std::string* res) {
    // key_size(6 bit) | value_size(10 bit) | key | value
    // 0x3f = 63, 0x3ff = 1023
    if (key.size() > 0x3f || value.size() > 0x3ff)
        return false;
    // 取key.size()的最后6位
    uint16_t header = key.size() & 0x3f;
    // 将key_size 左移10位，空出10位存放value_size.
    header = header << 10 | (value.size() & 0x3ff);
    char buf[sizeof(header)];
    memcpy(buf, &header, sizeof(header));
    res->append(buf, sizeof(header));
    res->append(key);
    res->append(value);
    return true;
}

bool DecodePayload(const std::string& str,
                   std::map<std::string, std::string>* padload) {
    // offset 表示当前偏移量
    int offset = 0;
    // 如果str剩余的大小不足2个字节，结束
    while (str.size() >= offset + 2) {
        uint16_t header;
        memcpy(&header, str.c_str() + offset, sizeof(header));
        // 分别取出key_size 和 value_size.
        uint16_t key_size = header >> 10;
        uint16_t value_size = header & 0x3ff;
        // key_size 和value_size 应该在指定的范围大小，并且str剩余的大小
        // 需要足够大.
        if (key_size > 0x3f || value_size > 0x3ff ||
            str.size() < offset + 2 + key_size + value_size)
            return false;
        // 取出key和value
        int start = offset + 2;
        int end = start + key_size;
        std::string key(str.begin() + start, str.begin() + end);
        start = end;
        end = start + value_size;
        std::string value(str.begin() + start, str.begin() + end);
        padload->insert(std::make_pair(std::move(key), std::move(value)));

        offset = end;
    }

    return true;
}


}   // namespace footbook