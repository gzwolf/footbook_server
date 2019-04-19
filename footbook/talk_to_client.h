//
// Created by YangGuang on 2019-03-24.
//

#ifndef CAMPUS_CHAT_TALK_TO_CLIENT_H
#define CAMPUS_CHAT_TALK_TO_CLIENT_H

#include <memory>
#include <string>
#include <boost/asio.hpp>

#include "base/macor.h"
#include "base/at_exit.h"
#include "footbook/message.h"
#include "footbook/listener.h"
#include "footbook/server.h"
#include "footbook/status.h"


namespace footbook {

namespace {
const int kTextBufferSize = 4096;
}

class TalkToClient : public std::enable_shared_from_this<TalkToClient> {
 public:
    using ErrorCode = boost::system::error_code;
    using TalkToClientPtr = std::shared_ptr<TalkToClient>;
    using Socket = boost::asio::ip::tcp::socket;

    // 新建一个TalkToClient, 返回一个std::shared_ptr智能指针，想要获得TalkToCLient
    // 必须调用这个函数，TalkToClient不支持赋值和copy
    static TalkToClientPtr New(boost::asio::io_service& io_service,
                               Server& context);

    // 开始talk to client，里面的读取和写入都是异步操作
    void Start();
    // 停止会话
    void Stop();

    bool is_started() const { return is_started_; }
    Socket& sock() { return sock_; }
    std::string user_name() { return user_name_; }

    void SendMessage();



 protected:
    explicit TalkToClient(boost::asio::io_service& io_service,
                          Server& server);

    class Context : public Listener {
     public:
        explicit Context(Server& server);
        bool OnMessageReceived(const Message& message) override;
        void OnClientConnect() override;
        void OnBadMessageReceived(const Message& message) override;
        void OnLogin(const Status& status) override;
        void OnRegister(const Status& status) override;
        void OnSendSMS(const Status& status) override;
     protected:
        ~Context() override;

     private:
        Server& server_;
    };

    void Read();
    void ProcessMessage(std::string str);
    void OnRead(const ErrorCode& error_code, size_t byte);

 private:
    boost::asio::io_service& io_service_;
    boost::asio::ip::tcp::socket sock_;
    std::string user_name_;
    bool is_started_;
    char buf_[kTextBufferSize];

    std::unique_ptr<Listener> listener_;

    DISALLOW_COPY_AND_ASSIGN(TalkToClient);
};

}


#endif //CAMPUS_CHAT_TALK_TO_CLIENT_H
