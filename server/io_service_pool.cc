//
// Created by YangGuang on 2019-03-24.
//

#include "io_service_pool.h"

#include <thread>

namespace footbook {

IOServicePool::IOServicePool(std::size_t count)
    : next_io_service_(0) {
    if (!count)
        throw std::runtime_error("io_service pool size is 0");
    for (std::size_t i = 0; i < count; ++i) {
        IoServicePtr io_service(new boost::asio::io_service);
        WorkPtr work(new boost::asio::io_service::work(*io_service));
        io_services_.push_back(io_service);
        work_.push_back(work);
    }
}

void IOServicePool::Run() {
    // Create a pool of threads to run all of the io_services.
    std::vector<std::shared_ptr<std::thread> > threads;
    for (std::size_t i = 0; i < io_services_.size(); ++i) {
        std::shared_ptr<std::thread> thread(new std::thread(
                [i, this]() {
                    io_services_[i]->run();
                }));
        threads.push_back(thread);
    }

    // Wait for all threads in the pool to exit.
    for (std::size_t i = 0; i < threads.size(); ++i)
        threads[i]->join();
}

void IOServicePool::Stop() {
    // Explicitly stop all io_services.
    for (std::size_t i = 0; i < work_.size(); ++i)
        work_[i].reset();
}

boost::asio::io_service &IOServicePool::get_io_service() const {
    // Use a round-robin scheme to choose the next io_service to use.
    boost::asio::io_service& io_service = *io_services_[next_io_service_];
    ++next_io_service_;
    if (next_io_service_ == io_services_.size())
        next_io_service_ = 0;
    return io_service;
}

boost::asio::io_service& IOServicePool::get_io_service(
        std::size_t index) const {
    index = index % io_services_.size();
    return *io_services_[index];
}

}   // namespace footbook.

#include "io_service_pool.h"
