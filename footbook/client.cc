//
// Created by YangGuang on 2019-03-29.
//

#include "client.h"

#include <functional>

#include "glog/logging.h"
#include "alias.h"
#include "base/threading/browser_thread.h"
#include "footbook/alibaba_sms/sms.h"

namespace footbook {

namespace {
constexpr int kUserNameMaximumSize = 64;
constexpr int kUserNameMinimumSize = 6;
constexpr int kPasswordMaximumSize = 128;
constexpr int kPasswordMinimumSize = 8;
}

std::vector<std::string> DBSelect(const std::string& str) {
    return std::vector<std::string>();
}



Status SendMsg(const std::string& str, int) {
    return Status();
}

void Client::Login(const std::string &user_name,
                    const std::string &password,
                    const Client::LoginCallback &callback) {
    DCHECK(callback);
    // 简单用户名判断
    if (user_name.size() < kUserNameMinimumSize ||
        user_name.size() > kUserNameMaximumSize) {
        // 用户名不合法, 调用对应函数
        callback(Status::InValidAccount("The Account size Crossing the line."));
        return;
    }

    if (password.size() < kPasswordMinimumSize ||
        password.size() > kPasswordMaximumSize) {
        // 密码不合法, 调用对应函数
        callback(Status::InValidPassword("The password size crossing the line."));
        return;
    }

    // 发送一个取数据到任务到DB线程
    CampusChatThread::PostTaskAndReplyWithResult<std::vector<std::string>,
            std::vector<std::string>>(CampusChatThread::DB, FROM_HERE,
            std::bind(&DBSelect, std::string()),
            std::bind(&Client::OnGetDBCompleteForLogin, user_name, password,
            callback, std::placeholders::_1));
}

void Client::Register(const std::string &user_nmae,
                       const std::string &password,
                       const Client::RegisterCallback &callback) {

}

Status Client::LoginOut() {
    return Status();
}

void Client::OnGetDBCompleteForLogin(const std::string& user_name,
                                     const std::string& password,
                                     const Client::LoginCallback& callback,
                                     const std::vector<std::string>& result) {

}

}   // namespace footbook