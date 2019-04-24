//
// Created by YangGuang on 2019-04-23.
//

#ifndef FOOTBOOK_SERVER_ANOLYZE_H
#define FOOTBOOK_SERVER_ANOLYZE_H

#include <string>
#include <map>

#include "base/macor.h"
#include "footbook/status.h"

namespace base {
template <typename Type>
struct DefaultSingletonTraits;
}   // namespace base

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
 private:
    friend struct base::DefaultSingletonTraits<Anolyze>;
    Anolyze() = default;
    ~Anolyze() = default;
    DISALLOW_COPY_AND_ASSIGN(Anolyze);
};


}   // namespace footbook
#endif //FOOTBOOK_SERVER_ANOLYZE_H
