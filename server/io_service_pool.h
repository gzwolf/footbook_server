//
// Created by YangGuang on 2019-03-24.
//

#ifndef CAMPUS_CHAT_IO_SERVICE_POOL_H
#define CAMPUS_CHAT_IO_SERVICE_POOL_H

#include <iostream>
#include <vector>

#include <boost/asio.hpp>

namespace cchat {

class IOServicePool {
 public:
    explicit IOServicePool(std::size_t count);

    // 在线程池中运行所有的io_service对象, 调用Stop停止工作
    void Run();

    // 停止所有在线程池中运行的io_service对象
    void Stop();

    // 获得一个io_service对象
    boost::asio::io_service& get_io_service() const;
    // 获取指定index上的io_service
    boost::asio::io_service& get_io_service(std::size_t index) const;

 private:
    using IoServicePtr = std::shared_ptr<boost::asio::io_service>;
    using WorkPtr = std::shared_ptr<boost::asio::io_service::work>;

    // The pool of io_services.
    std::vector<IoServicePtr> io_services_;

    // The work that keeps the io_services running.
    std::vector<WorkPtr> work_;

    // The next io_service to use for a connection.
    mutable std::size_t next_io_service_;
};

}   // namespace cchat


#endif //CAMPUS_CHAT_IO_SERVICE_POOL_H
