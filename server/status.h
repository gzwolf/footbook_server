//
// Created by YangGuang on 2019-03-29.
//

#ifndef CAMPUS_CHAT_STATUS_H
#define CAMPUS_CHAT_STATUS_H

#include <string>
#include <algorithm>

namespace cchat {

class Status final {
 public:
    Status() noexcept : state_(nullptr) {}
    ~Status() { delete []state_; }

    Status(const Status& rhs);
    Status& operator=(const Status& rhs);

    Status(Status&& rhs) noexcept : state_(rhs.state_) { rhs.state_ = nullptr; }
    Status& operator=(Status&& rhs) noexcept;

    static Status Ok() { return Status(); }
    static Status MsgError(const std::string& msg) {
        return Status(kMsgError, msg);
    }
    static Status InValidAccount(const std::string& msg) {
        return Status(kInvalidAccount, msg);
    }
    static Status InValidPassword(const std::string& msg) {
        return Status(kInvalidPassword, msg);
    }

    bool ok() const { return state_ == nullptr; }
    bool IsMsgError() const { return code() == kMsgError; }
    bool IsInvalidAccount() const { return code() == kInvalidAccount; }
    bool IsInvalidPassword() const { return code() == kInvalidPassword; }

    std::string ToString() const;
    uint32_t ToInt32() const;
 private:
    // OK status 时state_是nullptr， 其他情况state_会新申请空间
    // state_[0...3] == length of message
    // state_[4] == code
    // state_[5...] == message
    const char* state_;
    enum Code {
        kOk = 0,
        kMsgError = 1,
        kInvalidAccount = 2,
        kInvalidPassword = 3
    };

    Code code() const {
        return (state_ == nullptr) ? kOk : static_cast<Code>(state_[4]);
    }

    Status(Code code, const std::string& msg);
    static const char* CopyState(const char* s);
};

}   // namespace cchat

#endif //CAMPUS_CHAT_STATUS_H
