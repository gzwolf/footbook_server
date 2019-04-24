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

 private:
    friend struct base::DefaultSingletonTraits<Client>;
    void OnGetDBCompleteForLogin(const std::string& user_name,
                                        const std::string& password,
                                        const LoginCallback& callback,
                                        const std::vector<std::string>& result);
    std::unique_ptr<db::DB> db_;
    Client() = default;
    DISALLOW_COPY_AND_ASSIGN(Client);
};

}


#endif //CAMPUS_CHAT_CLIENNT_H
