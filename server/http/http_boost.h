//
// Created by YangGuang on 2019-04-11.
//

#ifndef CAMPUS_CHAT_HTTP_BOOST_H
#define CAMPUS_CHAT_HTTP_BOOST_H

#include "server/status.h"
#include "server/http/http_base.h"
#include "boost/asio.hpp"

namespace footbook {
namespace http {

class HttpBoost : public HttpBase {
 public:
    explicit HttpBoost(boost::asio::io_service& io_service);

    Status Get(const std::string& url) override;
    Status Post(const std::string& url) override;
    Status Response(std::string* res) const override;

 private:
    // 建立请求
    void ResolveRequestHandle(const std::string& url, BuildCallback func);
    void OnResolve(const boost::system::error_code& err,
                   boost::asio::ip::tcp::resolver::iterator endpoint_iterator);
    // 连接后
    void OnConnect(const boost::system::error_code& err);
    // 发送请求后
    void OnSendRequest(const boost::system::error_code& err);
    // 读取响应后
    void OnReadResponse(const boost::system::error_code& err);
    // 读取响应头后
    void OnReadHeaders(const boost::system::error_code& err);
    // 读取正文数据后
    void OnReadContent(const boost::system::error_code& err);

    // 解析器
    boost::asio::ip::tcp::resolver resolver_;
    // 套接字
    boost::asio::ip::tcp::socket socket_;
    // 请求缓冲区
    boost::asio::streambuf request_;
    // 响应缓冲区
    boost::asio::streambuf response_;
    // 响应数据
    std::string response_data_;
};

Status Get(std::string url, std::string* response);
Status Post(std::string url, std::string* responce);

}   // namespace http
}   // namespace footbook


#endif //CAMPUS_CHAT_HTTP_BOOST_H
