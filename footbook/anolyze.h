//
// Created by YangGuang on 2019-04-23.
//

#ifndef FOOTBOOK_SERVER_ANOLYZE_H
#define FOOTBOOK_SERVER_ANOLYZE_H

#include <string>
#include <map>

#include "footbook/status.h"

namespace footbook {

// 这个类的主要工作是对keys进行解析操作
class Anolyze final {
 public:
    static Anolyze* GetInstance();
    Status AnolyzeLoginMsg(const std::map<std::string, std::string> &keys_map,
                           std::string* user_name, std::string* password);
    Status AnolyzeRegisterMsg(const std::map<std::string, std::string>& keys_map,
                              std::string* user_name, std::string* password,
                              std::string* verify_code);
    ~Anolyze() = default;
 private:
};


}   // namespace footbook
#endif //FOOTBOOK_SERVER_ANOLYZE_H
