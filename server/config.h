//
// Created by YangGuang on 2019-03-28.
//

#ifndef CAMPUS_CHAT_CONFIG_H
#define CAMPUS_CHAT_CONFIG_H

#include <string>

#include <tiff.h>

namespace cchat {

// 将value放入到dst中
void PutFixed32(std::string* dst, uint32_t value);
void PutFixed64(std::string* dst, uint64_t value);

// Lower-level put，直接将value放入到缓冲区, 采用小端
// REQUIRES: dst要有足够的缓冲区
void EncodeFixed32(char *dst, uint32_t value);
void EncodeFixed64(char* dst, uint64_t value);

uint32_t DecodeFixed32(const char *ptr);
uint64_t  DecodeFixed64(const char* ptr);

std::string UrlEncode(const std::string& sz_encode);
std::string Base64Encode(const unsigned char* data,int data_byte);

}   // namespace cchat

#endif //CAMPUS_CHAT_CONFIG_H
