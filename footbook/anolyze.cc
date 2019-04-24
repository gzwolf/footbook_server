//
// Created by YangGuang on 2019-04-23.
//

#include "anolyze.h"


#include "base/singleton.h"
#include "footbook/message_keys.h"
#include "glog/logging.h"

namespace footbook {


Anolyze *Anolyze::GetInstance() {
    return base::Singleton<Anolyze,
        base::LeakySingletonTraits<Anolyze>>::get();
}

Status Anolyze::AnolyzeLoginMsg(
        const std::map<std::string, std::string> &keys_map,
        std::string *user_name,
        std::string *password) {
    DCHECK(user_name);
    DCHECK(password);
    user_name->clear();
    password->clear();
    auto it = keys_map.find(keys::kUserName);
    if (it == keys_map.end())
        return Status::InvalidData("Payload format error, Username is not exist.");
    *user_name = it->second;
    it = keys_map.find(keys::kPassword);
    if (it == keys_map.end())
        return Status::InvalidData("Payload format error, Password is not exist.");
    *password = it->second;
    return Status::Ok();
}

Status Anolyze::AnolyzeRegisterMsg(
        const std::map<std::string, std::string> &keys_map,
        std::string *user_name,
        std::string *password,
        std::string *verify_code) {
    return Status();
}
}   // namesapce footbook