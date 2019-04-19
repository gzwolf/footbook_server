//
// Created by Sunshine on 2019-03-23.
//

#ifndef CAMPUS_CHAT_SERVER_H
#define CAMPUS_CHAT_SERVER_H

#include <list>
#include <boost/asio.hpp>

#include "base/macor.h"
#include "footbook/io_service_pool.h"
#include "base/threading/browser_process_sub_thread.h"
#include "footbook/alias.h"

namespace footbook {

class TalkToClient;

class Server {
 public:
    Server(std::size_t thread_count,
           const boost::asio::ip::tcp::endpoint &endpoint);
    ~Server();

    // 获得当前的Server对象，在一个程序中理应只出现一个Server.
    static Server* Currnet();

    // 开始这个服务，在这个函数中会接受连接
    void Start();

    // 等待运行完成
    void Run();

    void Stop();

 private:

    void Accept();
    void OnConnect(std::shared_ptr<TalkToClient> client,
                   const boost::system::error_code& error_code);
    // 线程数量
    const std::size_t thread_count_;
    // io_service的简单封装的线程池
    IOServicePool io_service_pool_;
    std::list<std::shared_ptr<TalkToClient>> clients_;
    boost::asio::ip::tcp::acceptor acceptor_;
    CampusChatSubThread major_thread_;
    CampusChatSubThread db_thread_;
    CampusChatSubThread http_thread_;

    DISALLOW_COPY_AND_ASSIGN(Server);
};

}


#endif //CAMPUS_CHAT_SERVER_H
