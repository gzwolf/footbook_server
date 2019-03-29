//
// Created by YangGuang on 2019-03-29.
//

#ifndef CAMPUS_CHAT_CLIENNT_H
#define CAMPUS_CHAT_CLIENNT_H

#include <string>
#include <functional>

#include "server/status.h"

namespace cchat {

class Client {
    using LoginCallback = std::function<void(const Status&)>;
    using RegisterCallback = std::function<void(const Status&)>;
 public:
    static void Login(const std::string &user_name,
                      const std::string &password,
                      const LoginCallback& callback);

    static void Register(const std::string& user_nmae,
                         const std::string& password,
                         const RegisterCallback& callback);

    static Status LoginOut();

 private:
    static void OnGetDBComplete();
};

}


#endif //CAMPUS_CHAT_CLIENNT_H
