//
// Created by YangGuang on 2019-03-29.
//

#include "status.h"

#include <stdio.h>

namespace footbook {

Status::Status(const Status &rhs) {
    state_ = (rhs.state_ == nullptr) ? nullptr : CopyState(rhs.state_);
}

Status &Status::operator=(const Status &rhs) {
    if (this != &rhs) {
        delete []state_;
        state_ = (rhs.state_ == nullptr) ? nullptr : CopyState(rhs.state_);
    }
    return *this;
}

Status &Status::operator=(Status &&rhs) noexcept {
    if (this != &rhs) {
        state_ = rhs.state_;
        rhs.state_ = nullptr;
    }
    return *this;
}

std::string Status::ToString() const {
    if (state_ == nullptr)
        return "OK";

    const char* type;
    char tmp[30];
    switch (code()) {
        case kOk:
            type = "OK";
            break;
        case kMsgError:
            type = "Msg Error: ";
            break;
        case kHttpError:
            type = "Http Error: ";
            break;
        case kInvalidAccount:
            type = "Invalid Account: ";
            break;
        case kInvalidPassword:
            type = "Invalid Password: ";
            break;
        default:
            snprintf(tmp, sizeof(tmp), "Unknown code(%d): ",
                     static_cast<int>(code()));
            type = tmp;
            break;
    }

    std::string result(type);
    uint32_t length;
    memcpy(&length, state_, sizeof(length));
    result.append(state_ + 5, length);
    return result;
}

uint32_t Status::ToInt32() const {
    return static_cast<uint32_t>(Code());
}

Status::Status(Status::Code code, const std::string &msg) {
    const uint32_t size = msg.size();
    char* result = new char[size + 5];
    memcpy(result, &size, sizeof(uint32_t));
    result[4] = static_cast<char>(code);
    memcpy(result + 5, msg.data(), size);
    state_ = result;
}

const char *Status::CopyState(const char *state) {
    // 计算需要申请的字节数
    uint32_t size;
    memcpy(&size, state, sizeof(size));
    char* result = new char[size + 5];
    memcpy(result, state, size + 5);
    return result;
}
}   // namespace footbook