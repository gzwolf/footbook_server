//
// Created by YangGuang on 2019-03-29.
//

#include "client.h"

namespace cchat {

void Client::Login(const std::string &user_name,
                    const std::string &password,
                    const Client::LoginCallback &callback) {

}

void Client::Register(const std::string &user_nmae,
                       const std::string &password,
                       const Client::RegisterCallback &callback) {

}

Status Client::LoginOut() {
    return Status();
}

void Client::OnGetDBComplete() {

}
}   // namespace cchat