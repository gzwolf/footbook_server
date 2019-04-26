//
// Created by YangGuang on 2019-03-29.
//

#ifndef CAMPUS_CHAT_CLIENNT_H
#define CAMPUS_CHAT_CLIENNT_H

#include <string>
#include <functional>

#include "base/macor.h"
#include "footbook/status.h"
#include "footbook/db/db.h"
#include "leveldb/db.h"

namespace base {
template <typename Type>
struct DefaultSingletonTraits;
}   // namespace base

namespace footbook {

class Client {
    using LoginCallback = std::function<void(const Status&)>;
    using RegisterCallback = std::function<void(const Status&)>;
 public:
    static Client* GetInstance();
    void Login(const std::string& user_name,
                      const std::string& password,
                      const LoginCallback& callback);

    void Register(const std::string& user_nmae,
                         const std::string& password,
                         const std::string& verify_code,
                         const RegisterCallback& callback);

    Status LoginOut();

    leveldb::DB* leveldb_;
 private:
    friend struct base::DefaultSingletonTraits<Client>;
    void OnGetDBCompleteForLogin(const std::string& user_name,
                                 const std::string& password,
                                 const std::shared_ptr<std::string> str,
                                 const LoginCallback& callback,
                                 const leveldb::Status& status);
    std::shared_ptr<db::DB> db_;

    Client();
    DISALLOW_COPY_AND_ASSIGN(Client);
};

}


#endif //CAMPUS_CHAT_CLIENNT_H
