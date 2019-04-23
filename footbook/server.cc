//
// Created by Sunshine on 2019-03-23.
//
#include <iomanip>
#include <chrono>

#include "footbook/server.h"

#include <glog/logging.h>

#include "footbook/talk_to_client.h"

namespace footbook {

namespace {
Server* g_instance = nullptr;
}   // namespace

Server::Server(std::size_t thread_count,
               const boost::asio::ip::tcp::endpoint &endpoint)
    : thread_count_(thread_count),
      io_service_pool_(thread_count),
      acceptor_(io_service_pool_.get_io_service()),
      major_thread_(FootbookThread::MSG),
      db_thread_(FootbookThread::DB),
      http_thread_(FootbookThread::HTTP) {
    if (g_instance == nullptr)
        g_instance = this;
    acceptor_.open(endpoint.protocol());
    acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    acceptor_.bind(endpoint);
    acceptor_.listen();
}

Server *Server::Currnet() {
    return g_instance;
}

void Server::Start() {
    major_thread_.Start();
    db_thread_.Start();
    http_thread_.Start();
    major_thread_.RegisterAsBrowserThread();
    db_thread_.RegisterAsBrowserThread();
    http_thread_.RegisterAsBrowserThread();
    Accept();
}

void Server::Run() {
    io_service_pool_.Run();
}

void Server::Stop() {
    major_thread_.Stop();
    db_thread_.Stop();
    http_thread_.Stop();
    io_service_pool_.Stop();
}

void Server::Accept() {
   TalkToClient::TalkToClientPtr new_client =
       TalkToClient::New(io_service_pool_.get_io_service(), *this);

   acceptor_.async_accept(new_client->sock(),
       std::bind(&Server::OnConnect, this, new_client, std::placeholders::_1));
}

void Server::OnConnect(std::shared_ptr<TalkToClient> client,
                       const boost::system::error_code &error_code) {
    if (error_code) {
        // 错误处理.
        LOG(ERROR) << "connect error!";
    } else {
        auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        LOG(INFO) << "There are new customer connections， IP is " <<
                  client->sock().remote_endpoint().address().to_string() <<
                  "\t" << std::put_time(std::localtime(&t), "%F %T");
        // 开启会话.
        client->Start();
    }

    TalkToClient::TalkToClientPtr new_client =
        TalkToClient::New(io_service_pool_.get_io_service(),*this);

    acceptor_.async_accept(new_client->sock(),
        std::bind(&Server::OnConnect, this, new_client, std::placeholders::_1));

}

Server::~Server() {
    DCHECK(g_instance);
    delete g_instance;
}



}   // namespace footbook