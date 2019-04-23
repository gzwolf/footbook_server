#include <iostream>
#include <sstream>
#include <iomanip>
#include <chrono>

#include "base/at_exit.h"
#include "footbook/glog_helper.h"
#include "footbook/server.h"
#include "footbook/message.h"
#include "footbook/talk_to_client.h"

int main(int argc, char* argv[]) {

    // 初始化glog日志系统
    footbook::GlogHelper glog_helper(argv[0]);

    // 在这个对象离开作用域会将帮定到这个对象到所有删除函数都调用起来
    base::AtExitManager at_exit;

    const char host[] = "127.0.0.1";
    std::size_t thread_count = 3;
    auto endpoint = boost::asio::ip::tcp::endpoint(
            boost::asio::ip::address::from_string(host), 9996);
    footbook::Server server(thread_count, endpoint);
    boost::asio::io_service test_io;
    footbook::TalkToClient::TalkToClientPtr client = footbook::TalkToClient::New(test_io, server);
    // 开始服务器程序
    server.Start();
    server.Run();

    return 0;
}