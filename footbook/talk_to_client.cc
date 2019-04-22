//
// Created by YangGuang on 2019-03-24.
//

#include "footbook/talk_to_client.h"

#include <functional>

#include "base/threading/browser_thread.h"
#include "footbook/alias.h"
#include "footbook/client.h"
#include "footbook/alibaba_sms/sms.h"
#include "footbook/port.h"
#include "footbook/limit.h"
#include "talk_to_client.h"


namespace footbook {

TalkToClient::TalkToClient(boost::asio::io_service &io_service,
                           Server& footbook)
    : io_service_(io_service),
      sock_(io_service_),
      listener_(new Context(footbook)){
    memset(buf_, 0, kTextBufferSize);
}

void TalkToClient::Start() {
    listener_->OnClientConnect();
    boost::asio::ip::tcp::no_delay no_delay(true);
    sock_.set_option(no_delay);
    Read();
}

void TalkToClient::Stop() {

}

void TalkToClient::Read() {
    memset(buf_, 0, kTextBufferSize);
    sock_.async_read_some(boost::asio::buffer(buf_, kTextBufferSize),
            std::bind(&TalkToClient::OnRead, shared_from_this(),
            std::placeholders::_1, std::placeholders::_2));
}

void TalkToClient::ProcessMessage(std::string str) {
    Message msg;
    bool result = DecodeMessage(str, &msg);
    if (result) {
        if (!listener_->OnMessageReceived(msg))
            listener_->OnBadMessageReceived(msg);
    } else {
        listener_->OnBadMessageReceived(msg);
    }

}

TalkToClient::TalkToClientPtr TalkToClient::New(
        boost::asio::io_service &io_service, Server& footbook) {
    //return std::make_shared<TalkToClient>(io_service);
    return std::shared_ptr<TalkToClient>(new TalkToClient(io_service, footbook));
}

void TalkToClient::OnRead(const ErrorCode &error_code, size_t byte) {
    if (error_code) {       // 错误处理

    } else {
        // 成功读取
        CampusChatThread::PostTask(CampusChatThread::ID::MSG,
                base::Location(),
                base::BindOnceClosure(&TalkToClient::ProcessMessage,
                this, std::string(buf_, byte)));
    }
    // 循环读取
    Read();
}

bool TalkToClient::Context::OnMessageReceived(const Message &message) {
    switch (static_cast<Message::MsgType>(message.type())) {
        case Message::kGeneralChat:
            break;
        case Message::kGroupChat:
            break;
        case Message::kSignIn: {
            std::map<std::string, std::string> res;
            DecodePayload(message.payload(), &res);
            res.find("username");
            std::string user_name;
            std::string password;
            Client::Login(user_name, password, std::bind(
                    &Listener::OnLogin, this, std::placeholders::_1));
            break;
        }
        case Message::kRegister:
            break;
        case Message::kSendVerificationCode: {
            std::string phone_number;
            auto code = port::Random(Limit<int>(10000, 999999));
            CampusChatThread::PostTaskAndReplyWithResult<Status, Status>(
                    CampusChatThread::HTTP,
                    FROM_HERE,
                    std::bind(&SMS::Send, SMS::GetInstance(),
                    phone_number, std::to_string(code)),
                    std::bind(&Listener::OnSendSMS, this, std::placeholders::_1));
            break;
        }
        default:
            return false;
    }
    return true;
}


void TalkToClient::Context::OnClientConnect() {
}

void TalkToClient::Context::OnBadMessageReceived(const Message &message) {
    Listener::OnBadMessageReceived(message);
}

TalkToClient::Context::~Context() {

}

TalkToClient::Context::Context(Server &server)
    : server_(server){
}

void TalkToClient::Context::OnLogin(const Status &status) {
    if (status.ok()) {
        // 登陆成功, 发送信息
    } else {
        // 登陆失败
    }
}

void TalkToClient::Context::OnRegister(const Status &status) {
    Listener::OnRegister(status);
}

void TalkToClient::Context::OnSendSMS(const Status &status) {
    Listener::OnSendSMS(status);
}

}   // namesapce footbook
