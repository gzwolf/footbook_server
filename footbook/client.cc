//
// Created by YangGuang on 2019-03-29.
//

#include "client.h"

#include <functional>

#include "base/singleton.h"
#include "base/threading/browser_thread.h"
#include "glog/logging.h"
#include "footbook/alias.h"
#include "footbook/alibaba_sms/sms.h"


namespace footbook {

namespace {
constexpr int kUserNameMaximumSize = 64;
constexpr int kUserNameMinimumSize = 6;
constexpr int kPasswordMaximumSize = 128;
constexpr int kPasswordMinimumSize = 8;
const char kDatabaseName[] = "footbook";
const char kLeveldbName[] = "password";
}

Client *footbook::Client::GetInstance() {
    return base::Singleton<Client,
        base::LeakySingletonTraits<Client>>::get();
}

void Client::Login(const std::string &user_name,
                    const std::string &password,
                    const Client::LoginCallback &callback) {
    //DCHECK(callback);
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


    //leveldb_->Get(leveldb::ReadOptions(), user_name, )
    /*
    std::shared_ptr<std::string> result;

    // 发送一个取数据到任务到DB线程
    FootbookThread::PostTaskAndReplyWithResult<leveldb::Status,
            leveldb::Status>(FootbookThread::DB, FROM_HERE,
            std::bind(&leveldb::DB::Get, leveldb_, leveldb::ReadOptions(), user_name, result.get()),
            std::bind(&Client::OnGetDBCompleteForLogin, this, user_name, password, result,
            callback, std::placeholders::_1));
            */
    std::string res;
    auto status = leveldb_->Get(leveldb::ReadOptions(), user_name, &res);
    if (!status.ok()) {
        callback(Status::InValidAccount(status.ToString()));
        return;
    }

    if (res != password) {
        callback(Status::InValidPassword("password is error."));
        return;
    }

    callback(Status::Ok());
}

void Client::Register(const std::string &user_nmae,
                      const std::string &password,
                      const std::string& verify_code,
                      const Client::RegisterCallback &callback) {

}

Status Client::LoginOut() {
    return Status();
}

void Client::OnGetDBCompleteForLogin(const std::string& user_name,
                             const std::string& password,
                             const std::shared_ptr<std::string> str,
                             const LoginCallback& callback,
                             const leveldb::Status& status) {
    LOG(INFO) << status.ToString();
    LOG(INFO) << str;
}

footbook::Client::Client() {
    db::DB::Open(kDatabaseName, db::DBType::kMysql, &db_);
    leveldb::Options options;
    options.create_if_missing = true;
    auto status = leveldb::DB::Open(options, kLeveldbName, &leveldb_);
    DCHECK(status.ok());
}

}   // namespace footbook