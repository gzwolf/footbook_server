//
// Created by YangGuang on 2019-03-24.
//

#include "talk_to_client.h"

namespace cchat {

TalkToClient::TalkToClient(boost::asio::io_service &io_service)
    : io_service_(io_service),
      sock_(io_service_){

}

void TalkToClient::Start() {

}

void TalkToClient::Stop() {

}

TalkToClient::TalkToClientPtr TalkToClient::New(
        boost::asio::io_service &io_service) {
    //return std::make_shared<TalkToClient>(io_service);
    return std::shared_ptr<TalkToClient>(new TalkToClient(io_service));
}


}   // namesapce cchat