//
// Created by YangGuang on 2019-03-28.
//

#ifndef CAMPUS_CHAT_PORT_H
#define CAMPUS_CHAT_PORT_H

#include <random>
#include <iostream>

#include "server/limit.h"

namespace port {

inline bool IsLittleEndian() {
    unsigned int i = 0x12345678;
    char *p = reinterpret_cast<char *>(&i);
    if (*p == 0x78)
        return true;  //little
    else
        return false;
}

template <typename T>
inline auto Random(const footbook::Limit<T>& limit) -> decltype(limit.min()) {
    std::random_device rd;
    std::mt19937 mt(rd());

    typename footbook::Limit<T>::size_type code;
    bool is_real = std::is_same<typename std::decay<T>::type, double>::value ||
            std::is_same<typename std::decay<T>::type, float>::value;

    if (is_real) {
        std::uniform_real_distribution<double> dist(limit.min(), limit.max());
        code = dist(mt);
    } else {
        std::uniform_int_distribution<int> dist(limit.min(), limit.max());
        code = dist(mt);
    }

    return code;
}

}   // namespace port

#endif //CAMPUS_CHAT_PORT_H
