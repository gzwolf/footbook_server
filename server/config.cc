//
// Created by YangGuang on 2019-03-28.
//
#include "server/config.h"

#include "server/port.h"

namespace cchat {

void EncodeFixed32(char *buf, uint32_t value) {
    if (port::IsLittleEndian()) {
        memcpy(buf, &value, sizeof(value));
    } else {
        buf[0] = value & 0xff;
        buf[1] = (value >> 8) & 0xff;
        buf[2] = (value >> 16) & 0xff;
        buf[3] = (value >> 24) & 0xff;
    }
}

void EncodeFixed64(char* buf, uint64_t value) {
    if (port::IsLittleEndian()) {
        memcpy(buf, &value, sizeof(value));
    } else {
        buf[0] = value & 0xff;
        buf[1] = (value >> 8) & 0xff;
        buf[2] = (value >> 16) & 0xff;
        buf[3] = (value >> 24) & 0xff;
        buf[4] = (value >> 32) & 0xff;
        buf[5] = (value >> 40) & 0xff;
        buf[6] = (value >> 48) & 0xff;
        buf[7] = (value >> 56) & 0xff;
    }
}

uint32_t DecodeFixed32(const char *ptr) {
    if (port::IsLittleEndian()) {
        uint32_t result;
        memcpy(&result, ptr, sizeof(result));
        return result;
    } else {
        return static_cast<uint32_t>(static_cast<unsigned char>(ptr[0])
                                     | ((static_cast<uint32_t >(ptr[1])) << 8)
                                     | ((static_cast<uint32_t>(ptr[2])) << 16)
                                     | ((static_cast<uint32_t>(ptr[3])) << 24));
    }
}

uint64_t  DecodeFixed64(const char* ptr) {
    if (port::IsLittleEndian()) {
        // Load the raw bytes
        uint64_t result;
        memcpy(&result, ptr, sizeof(result));
        return result;
    } else {
        uint64_t lo = DecodeFixed32(ptr);
        uint64_t hi = DecodeFixed32(ptr + 4);
        return (hi << 32) | lo;
    }
}

void PutFixed32(std::string *dst, uint32_t value) {
    char buf[sizeof(value)];
    EncodeFixed32(buf, value);
    dst->append(buf, sizeof(buf));
}

void PutFixed64(std::string* dst, uint64_t value) {
    char buf[sizeof(value)];
    EncodeFixed64(buf, value);
    dst->append(buf, sizeof(buf));
}
}   // namespace cchat