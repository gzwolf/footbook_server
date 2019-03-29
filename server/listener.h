//
// Created by YangGuang on 2019-03-26.
//

#ifndef CAMPUS_CHAT_LISTENER_H
#define CAMPUS_CHAT_LISTENER_H

#include "server/status.h"

namespace cchat {

class Message;

class Listener {
 public:
    virtual bool OnMessageReceived(const Message& message) = 0;
    virtual void OnClientConnect() {}
    virtual void OnBadMessageReceived(const Message& message) {}
    virtual void OnLogin(const Status& status) {}
    virtual void OnRegister(const Status& status) {}

 //protected:
    virtual ~Listener() = default;
};

}   // namespace cchat


#endif //CAMPUS_CHAT_LISTENER_H
