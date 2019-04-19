//
// Created by YangGuang on 2019-03-28.
//
#include "footbook/config.h"

#include "footbook/port.h"

namespace footbook {

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

void EncodeFixed8(char* dst, uint8_t value) {
    memcpy(dst, &value, sizeof(value));
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

uint8_t DecodeFixed8(const char* ptr) {
    uint8_t result;
    memcpy(&result, ptr, sizeof(result));
    return result;
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

void PutFixed8(std::string* dst, uint8_t value) {
    char buf[sizeof(value)];
    EncodeFixed8(buf, value);
    dst->append(buf, sizeof(buf));

}

std::string UrlEncode(const std::string& sz_encode) {
    std::string src = sz_encode;
    char hex[] = "0123456789ABCDEF";
    std::string dst;

    for (size_t i = 0; i < src.size(); ++i) {
        unsigned char cc = src[i];
        if ((cc >= 'A' && cc <= 'Z')
            || (cc >= 'a' && cc <= 'z')
            || (cc >= '0' && cc <= '9')
            || cc == '.'
            || cc == '_'
            || cc == '-'
            || cc == '*') {
            if (cc == ' ') {
                dst += "+";
            } else {
                dst += cc;
            }
        } else {
            unsigned char c = static_cast<unsigned char>(src[i]);
            dst += '%';
            dst += hex[c / 16];
            dst += hex[c % 16];

        }
    }
    return dst;
}

std::string Base64Encode(const unsigned char* data, int data_byte) {
    //编码表
    const char encode_table[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghi"
                             "jklmnopqrstuvwxyz0123456789+/";
    //返回值
    std::string str_encode;
    unsigned char tmp[4] = {0};
    int line_length = 0;
    for(int i=0; i<(int)(data_byte / 3); i++) {
            tmp[1] = *data++;
            tmp[2] = *data++;
            tmp[3] = *data++;
            str_encode += encode_table[tmp[1] >> 2];
            str_encode += encode_table[((tmp[1] << 4) | (tmp[2] >> 4)) & 0x3F];
            str_encode += encode_table[((tmp[2] << 2) | (tmp[3] >> 6)) & 0x3F];
            str_encode += encode_table[tmp[3] & 0x3F];
            if(line_length+=4, line_length==76) {
                str_encode+="\r\n";line_length=0;
            }
    }
    //对剩余数据进行编码
    int Mod=data_byte % 3;
    if(Mod == 1) {
        tmp[1] = *data++;
        str_encode += encode_table[(tmp[1] & 0xFC) >> 2];
        str_encode += encode_table[((tmp[1] & 0x03) << 4)];
        str_encode += "==";
    } else if(Mod==2) {
        tmp[1] = *data++;
        tmp[2] = *data++;
        str_encode += encode_table[(tmp[1] & 0xFC) >> 2];
        str_encode += encode_table[((tmp[1] & 0x03) << 4) | ((tmp[2] & 0xF0) >> 4)];
        str_encode += encode_table[((tmp[2] & 0x0F) << 2)];
        str_encode += "=";
    }

    return str_encode;
}

}   // namespace footbook