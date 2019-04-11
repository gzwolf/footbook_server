//
// Created by YangGuang on 2019-03-25.
//

#ifndef CAMPUS_CHAT_MESSAGE_H
#define CAMPUS_CHAT_MESSAGE_H

#include <string>
#include <algorithm>

#include <tiff.h>
#include <glog/logging.h>

namespace footbook {

class Message {
 public:
    enum MsgType {
        kGeneralChat = 0x1234,
        kGroupChat,
        kSignIn,
        kRegister
    };

    enum Status {
        kSuccess = 0x01,
        kPasswordError,
        kUsernameError,
        kMsgFormatError
    };

    explicit Message(uint32_t type);
    explicit Message(const std::string& str);
    Message(const char *data, int data_len);

    Message();
    ~Message();

    void set_type(uint32_t type) { header()->type = type; }
    void set_sender(uint32_t sender) { header()->sender = sender; }
    void set_receiver(uint32_t receiver) { header()->receiver = receiver; }
    void set_status(uint32_t status) { header()->status = status; }

    void set_payload(const char* data, int data_len) {
        DCHECK(data);
        std::copy(data, data + data_len, payload_.begin());
        //memcpy((void*)payload_.data(), data, data_len);
        //payload_[data_len] = '\0';
        header()->payload_size = data_len;
    }

    void set_payload(const std::string& str) {
        payload_ = str;
        header()->payload_size = payload_.size();
    }


    std::size_t header_size();
    uint32_t payload_size() const { return header()->payload_size; }
    uint32_t type() const { return header()->type; }
    uint32_t sender() const { return header()->sender; }
    uint32_t receiver() const { return header()->receiver; }
    uint32_t status() const { return header()->status; }
    const char* payload() const { return payload_.data(); }
    std::string payload() { return payload_; }

 protected:
#pragma pack(push, 4)
    struct Header {
        uint32_t payload_size;        // 发送的数据的大小
        uint32_t type;                // 消息类型
        uint32_t sender;
        uint32_t receiver;
        uint32_t status;
    };
#pragma pack(pop)

    Header *header() { return &header_; }

    const Header *header() const { return &header_; }

 private:

    Header header_;
    std::string payload_;            // 实际数据

};

// message的编码解码函数
bool EncodeMessage(const Message& msg, std::string* str);
bool EncodeMessage(const Message& msg, char* str);
bool DecodeMessage(const std::string& str, Message* msg);
bool DecodeMessage(const char* str, Message* msg);

}
#endif //CAMPUS_CHAT_MESSAGE_H
