//
// Created by YangGuang on 2019-03-25.
//

#ifndef CAMPUS_CHAT_MESSAGE_H
#define CAMPUS_CHAT_MESSAGE_H

#include <string>
#include <algorithm>

#include <tiff.h>
#include <glog/logging.h>

namespace cchat {

class Message {
 public:
    enum MsgType {
        kGeneralChat = 0x01,
        kGroupChat,
        kSignIn,
        kRegister,
        kError
    };

    explicit Message(uint32_t type);
    explicit Message(const std::string& str);
    Message(const char *data, int data_len);

    Message();
    ~Message();

    void set_type(uint32_t type) { header()->type = type; }
    void set_sender(int32_t sender) { header()->sender = sender; }
    void set_receiver(int32_t receiver) { header()->receiver = receiver; }
    void set_flags(uint8 flags) { header()->flags = flags; }

    void set_payload(const char* data, int data_len) {
        DCHECK(data);
        std::copy(data, data + data_len, payload_.begin());
    }

    uint32 payload_size() const { return header()->payload_size; }


    uint32 type() const { return header()->type; }

    uint8 flags() const { return header()->flags; }

 protected:
#pragma pack(push, 4)
    struct Header {
        uint32_t payload_size;        // 发送的数据的大小
        uint32_t type;                // 消息类型
        int32_t sender;
        int32_t receiver;
        uint8_t flags;
    };
#pragma pack(pop)

    Header *header() { return &header_; }

    const Header *header() const { return &header_; }

 private:

    Header header_;
    std::string payload_;            // 实际数据
};

bool EncodeMessage(const Message& msg, std::string& str);
bool EncodeMessage(const Message& msg, char* str);

bool DecodeMessage(const std::string& str, Message* msg);
bool EncodeMessage(const char* str, Message* msg);

}
#endif //CAMPUS_CHAT_MESSAGE_H
