
#include "server/glog_helper.h"
#include "server/server.h"
#include "server/message.h"
#include "server/alibaba_sms/sms.h"

int main(int argc, char* argv[]) {
    // 测试消息编解码
    std::string str = "I love you!";
    cchat::Message msg(str);
    msg.set_sender(123);
    msg.set_receiver(0);
    msg.set_status(3);
    msg.set_type(0x1234);

    std::string encode_str;
    cchat::Message decode_msg;

    cchat::EncodeMessage(msg, &encode_str);
    cchat::DecodeMessage(encode_str, &decode_msg);

    std::cout << decode_msg.payload_size() << "\nsender = "
            << decode_msg.sender() << "\nreceiver = " << decode_msg.receiver()
            << "\nstatus = " << decode_msg.status() << "payload = " << decode_msg.payload() << std::endl;


    // 初始化glog日志系统
    cchat_log::GlogHelper glog_helper(argv[0]);
    // 测试短信发送
    cchat::SMS sms;
    sms.Send("13971348462", "123456");

    const char host[] = "127.0.0.1";
    std::size_t thread_count = 4;
    auto endpoint = boost::asio::ip::tcp::endpoint(
            boost::asio::ip::address::from_string(host), 9996);
    cchat::Server server(thread_count, endpoint);
    // 开始服务器程序
    server.Start();
    server.Run();

    return 0;
}