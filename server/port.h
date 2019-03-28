//
// Created by YangGuang on 2019-03-28.
//

#ifndef CAMPUS_CHAT_PORT_H
#define CAMPUS_CHAT_PORT_H

namespace port {

inline bool IsLittleEndian() {
    unsigned int i = 0x12345678;
    char *p = reinterpret_cast<char *>(&i);
    if (*p == 0x78)
        return true;  //little
    else
        return false;
}

}   // namespace port

#endif //CAMPUS_CHAT_PORT_H
