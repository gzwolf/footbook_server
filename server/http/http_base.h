//
// Created by YangGuang on 2019-04-11.
//

#ifndef CAMPUS_CHAT_HTTP_BASE_H
#define CAMPUS_CHAT_HTTP_BASE_H

#include <iostream>
#include <functional>

#include "server/status.h"

namespace footbook {
namespace http {

class HttpBase {
 public:
    virtual Status Get(const std::string& url) = 0;
    virtual Status Post(const std::string& url) = 0;
    virtual Status Response(std::string*) const = 0;

    virtual ~HttpBase() = default;

 protected:
    using BuildCallback = std::function<Status(
            const std::string&, const std::string& ,std::ostream&)>;
    // 解析URL函数， url表示解析的URL地址，后面三个参数分别是输出，out_server表示输出
    // 的服务器名，out_port表示输出的端口名，out_path表示输出的服务器子页
    static Status ParseUrl(const std::string& url, std::string& out_server,
                        std::string& out_port, std::string& out_path);

    static Status BuildPostRequest(const std::string& server,
                                const std::string& path,
                                std::ostream& out_request);

    static Status BuildGetRequest(const std::string& server, const std::string& path,
                               std::ostream& out_request);
};

}   // namespace http
}   // namespace footbook


#endif //CAMPUS_CHAT_HTTP_BASE_H
