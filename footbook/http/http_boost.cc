//
// Created by YangGuang on 2019-04-11.
//

#include "http_boost.h"

#include "glog/logging.h"

namespace footbook {
namespace http {

HttpBoost::HttpBoost(boost::asio::io_service& io_service)
    : resolver_(io_service), socket_(io_service) {
}

Status HttpBoost::Get(const std::string &url) {
    try {
        ResolveRequestHandle(url, &HttpBase::BuildGetRequest);
    } catch (std::exception& e) {
        return Status::HttpError(const_cast<char*>(e.what()));
    }
    return Status::Ok();
}

Status HttpBoost::Post(const std::string &url) {
    try {
        ResolveRequestHandle(url, &HttpBase::BuildPostRequest);
    } catch (std::exception& e) {
        return Status::HttpError(const_cast<char*>(e.what()));
    }
    return Status::Ok();
}

Status HttpBoost::Response(std::string *res) const {
    return Status();
}

void HttpBoost::ResolveRequestHandle(const std::string &url,
                                     HttpBase::BuildCallback func) {
    try {
        response_data_.clear();
        // 解析URL
        std::string server, port, path;
        ParseUrl(url, server, port, path);

        std::ostream request_stream(&request_);
        // 合成请求
        func(server, path, request_stream);

        // 解析服务地址\端口
        boost::asio::ip::tcp::resolver::query query(server, port);
        resolver_.async_resolve(query,
                                std::bind(&HttpBoost::OnResolve, this,
                                            std::placeholders::_1,
                                            std::placeholders::_2));
    }
    catch (std::exception& e) {
        socket_.close();
        throw e;
    }
}

void HttpBoost::OnResolve(const boost::system::error_code &err,
        boost::asio::ip::tcp::resolver::iterator endpoint_iterator) {
    if (err) {
        LOG(ERROR) << "Error: " << err;
        return;
    }
    boost::asio::async_connect(socket_, endpoint_iterator,
                               std::bind(&HttpBoost::OnConnect, this,
                                           std::placeholders::_1));
}

void HttpBoost::OnConnect(const boost::system::error_code &err) {
    if (err) {
        LOG(ERROR) << "Error: " << err;
        return;
    }
    // 发送request请求
    boost::asio::async_write(socket_, request_,
                             std::bind(&HttpBoost::OnSendRequest, this,
                                         std::placeholders::_1));
}

void HttpBoost::OnSendRequest(const boost::system::error_code &err) {
    if (err) {
        LOG(ERROR) << "Error: " << err;
        return;
    }

    // 异步持续读数据到response_，直到接收协议符号 \r\n 为止
    boost::asio::async_read_until(socket_, response_, "\r\n",
                                  std::bind(&HttpBoost::OnReadResponse, this,
                                              std::placeholders::_1));
}

void HttpBoost::OnReadResponse(const boost::system::error_code &err) {
    if (err) {
        LOG(ERROR) << "Error: " << err;
        return;
    }

    // 解析buff
    std::istream response_stream(&response_);
    unsigned int status_code;
    std::string http_version, status_message;
    response_stream >> http_version;
    response_stream >> status_code;
    std::getline(response_stream, status_message);

    // 核对是否是正确返回
    if (!response_stream || http_version.substr(0, 5) != "HTTP/") {
        LOG(ERROR) << "Error responce data!";
        return;
    }
    if (status_code != 200) {
        LOG(ERROR) << "Server responce status code is " << status_code;
        return;
    }

    // 读取响应头,直到接收协议符号 \r\n\r\n 为止
    boost::asio::async_read_until(socket_, response_, "\r\n\r\n",
                                  std::bind(&HttpBoost::OnReadHeaders, this,
                                          std::placeholders::_1));
}

void HttpBoost::OnReadHeaders(const boost::system::error_code &err) {
    if (err) {
        LOG(ERROR) << "Error: " << err;
        return;
    }
    // 输出响应头
    std::istream response_stream(&response_);
    std::string header;
    while (std::getline(response_stream, header) && header != "\r") {
        LOG(INFO) << header;
    }

    // 写完所有剩余的内容
    if (response_.size() > 0) {
        boost::asio::streambuf::const_buffers_type cbt = response_.data();
        response_data_ += std::string(
                boost::asio::buffers_begin(cbt), boost::asio::buffers_end(cbt));
        LOG(INFO) << &response_;
    }

    // 开始读取剩余所有内容
    boost::asio::async_read(socket_, response_,
                            boost::asio::transfer_at_least(1),
                            std::bind(&HttpBoost::OnReadContent, this,
                                    std::placeholders::_1));
}

void HttpBoost::OnReadContent(const boost::system::error_code &err) {
    if (!err) {
        // 输出读到的数据
        boost::asio::streambuf::const_buffers_type cbt = response_.data();
        response_data_ += std::string(
                boost::asio::buffers_begin(cbt), boost::asio::buffers_end(cbt));
        LOG(INFO) << &response_;
        // 继续读取剩余内容，直到读到EOF
        boost::asio::async_read(socket_, response_,
                                boost::asio::transfer_at_least(1),
                                std::bind(&HttpBoost::OnReadContent, this,
                                            std::placeholders::_1));
    }
    else if (err != boost::asio::error::eof) {
        LOG(ERROR) << "Error: " << err;
    }
    else {
        socket_.close();
        resolver_.cancel();
        LOG(INFO) << "Read responce content done!";
    }
}

Status Post(std::string url, std::string* responce) {
    boost::asio::io_service io;
    HttpBoost c(io);
    Status status = c.Post(url);
    if (!status.ok())
        return status;
    io.run();
    status = c.Response(responce);
    return status;
}

Status Get(std::string url, std::string* responce) {
    boost::asio::io_service io;
    HttpBoost c(io);
    Status status = c.Get(url);
    if (!status.ok())
        return status;
    io.run();
    status = c.Response(responce);
    return status;
}

}   // namespace http
}   // namespace footbook