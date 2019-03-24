
#include "server/server.h"

int main(int argc, char* argv[]) {
    const char host[] = "127.0.0.1";
    std::size_t thread_count = 4;
    auto endpoint = boost::asio::ip::tcp::endpoint(
            boost::asio::ip::address::from_string(host), 8801);
    cchat::Server server(thread_count, endpoint);
    server.Start();
    server.Run();

    return 0;
}