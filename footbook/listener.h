//
// Created by YangGuang on 2019-03-26.
//

#ifndef CAMPUS_CHAT_LISTENER_H
#define CAMPUS_CHAT_LISTENER_H

#include "footbook/status.h"

namespace footbook {

class Message;

class Listener {
 public:
    virtual Status OnMessageReceived(const Message& message) = 0;
    virtual void OnClientConnect() {}
    virtual void OnBadMessageReceived(const Message& message) {}
    virtual void OnLogin(const Status& status) {}
    virtual void OnRegister(const Status& status) {}
    virtual void OnSendSMS(const Status& sttus) {}

 //protected:
    virtual ~Listener() = default;
};

}   // namespace footbook


#endif //CAMPUS_CHAT_LISTENER_H
