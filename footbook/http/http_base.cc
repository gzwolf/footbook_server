//
// Created by YangGuang on 2019-04-11.
//

#include "http_base.h"


namespace footbook {
namespace http {

namespace {
const char kHttpJsonBegin = '[';
const char kHttpJsonEnd = ']';

}
Status HttpBase::ParseUrl(const std::string& url, std::string& out_server,
                    std::string& out_port, std::string& out_path) {
    const std::string& http___ = "http://";
    const std::string& https___ = "https://";
    std::string temp_data = url;

    // 截断http协议头
    if (temp_data.find(http___) == 0) {
        temp_data = temp_data.substr(http___.length());
    } else if (temp_data.find(https___) == 0) {
        temp_data = temp_data.substr(https___.length());
    } else {
        return Status::HttpError("Not find the \'http\' or \'https\' protocol");
    }

    // 解析域名
    std::size_t idx = temp_data.find('/');
    // 解析 域名后的page
    if (std::string::npos == idx) {
        out_path = "/";
        idx = temp_data.size();
    }
    else {
        out_path = temp_data.substr(idx);
    }

    // 解析域名
    out_server = temp_data.substr(0, idx);

    // 解析端口
    idx = out_server.find(':');
    if (std::string::npos == idx) {
        out_port = "80";
    }
    else {
        out_port = out_server.substr(idx + 1);
        out_server = out_server.substr(0, idx);
    }

    return Status::Ok();

}

Status HttpBase::BuildPostRequest(const std::string& server,
                               const std::string& path,
                               std::ostream& out_request) {
    std::string temp_path(path), temp_json;
    std::size_t json_pos_begin = temp_path.find(kHttpJsonBegin) + 1;
    std::size_t json_pos_end = temp_path.find(kHttpJsonEnd);
    if (json_pos_begin != std::string::npos) {
        // 计算json的长度
        std::size_t temp_json_lenth = std::string::npos;
        if (json_pos_end != temp_json_lenth) {
            temp_json_lenth = (json_pos_end - json_pos_begin);
        }
        temp_json = temp_path.substr(json_pos_begin, temp_json_lenth);
        temp_path = temp_path.substr(0, (json_pos_begin - 1));
    }


    out_request << "POST " << temp_path.c_str() << " HTTP/1.0\r\n";
    out_request << "Host: " << server.c_str() << "\r\n";
    out_request << "Content-Length: " << temp_json.length() << "\r\n";
    out_request << "Content-Type: application/x-www-form-urlencoded\r\n";
    out_request << "Accept: */*\r\n";
    out_request << "Connection: close\r\n\r\n";
    out_request << temp_json.c_str();
    return Status::Ok();
}


Status HttpBase::BuildGetRequest(const std::string& server,
                                 const std::string& path,
                                 std::ostream& out_request) {
    out_request << "GET " << path.c_str() << " HTTP/1.0\r\n";
    out_request << "Host: " << server.c_str() << "\r\n";
    out_request << "Accept: */*\r\n";
    out_request << "Connection: close\r\n\r\n";
    return Status::Ok();
}

}   // namespace footbook
}   // namespace http