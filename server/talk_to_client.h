//
// Created by YangGuang on 2019-03-24.
//

#ifndef CAMPUS_CHAT_TALK_TO_CLIENT_H
#define CAMPUS_CHAT_TALK_TO_CLIENT_H

#include <memory>
#include <string>
#include <boost/asio.hpp>

#include "base/macor.h"

namespace cchat {

class TalkToClient : public std::enable_shared_from_this<TalkToClient> {
 public:
    using ErrorCode = boost::system::error_code;
    using TalkToClientPtr = std::shared_ptr<TalkToClient>;
    using Socket = boost::asio::ip::tcp::socket;

    // 新建一个TalkToClient, 返回一个std::shared_ptr智能指针，想要获得TalkToCLient
    // 必须调用这个函数，TalkToClient不支持赋值和copy
    static TalkToClientPtr New(boost::asio::io_service& io_service);

    // 开始talk to client，里面的读取和写入都是异步操作
    void Start();
    // 停止会话
    void Stop();

    bool is_started() const { return is_started_; }
    Socket& sock() { return sock_; }
    std::string user_name() { return user_name_; }

 protected:
    explicit TalkToClient(boost::asio::io_service& io_service);
 private:
    boost::asio::io_service& io_service_;
    boost::asio::ip::tcp::socket sock_;
    std::string user_name_;
    bool is_started_;

    DISALLOW_COPY_AND_ASSIGN(TalkToClient);
};

}


#endif //CAMPUS_CHAT_TALK_TO_CLIENT_H
