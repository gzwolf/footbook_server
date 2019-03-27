//
// Created by YangGuang on 2019-03-26.
//

#ifndef CAMPUS_CHAT_LISTENER_H
#define CAMPUS_CHAT_LISTENER_H

namespace cchat {

class Message;

class Listener {
 public:
    virtual bool OnMessageReceived(const Message& message) = 0;
    virtual void OnClientConnect() {}
    virtual void OnBadMessageReceived(const Message& message) {}

 //protected:
    virtual ~Listener() = default;
};

}   // namespace cchat


#endif //CAMPUS_CHAT_LISTENER_H
