# 支持的最低版本
cmake_minimum_required(VERSION 2.6)
cmake_policy(SET CMP0012 NEW)


#项目名称
project(campus_chat)

#支持C++11
add_definitions(-std=c++14)

set(INC_DIR /usr/local/include)
set(LINK_DIR /usr/local/lib)

include_directories(./)
include_directories(${INC_DIR})
link_directories(${LINK_DIR})

set(SERVER_SRC
        server/server.h
        server/server.cc
        server/io_service_pool.cc
        server/io_service_pool.h
        server/talk_to_client.cc
        server/talk_to_client.h
        server/glog_helper.h
        server/glog_helper.cc
        server/message.cc
        server/message.h
        server/alias.h
        server/listener.cc
        server/listener.h
        server/config.h
        server/config.cc
        server/client.cc
        server/client.h
        server/status.cc
        server/status.h)

set(MAIN_SRC campus_chat_main.cc)

set(ALIBABA_SMS_SRC
        server/alibaba_sms/sms.h
        server/alibaba_sms/sms.cc)

set(HTTP_SRC server/http/http_base.h
        server/http/http_base.cc server/http/http_boost.cc server/http/http_boost.h)

set(ALL_SRC ${SERVER_SRC} ${ALIBABA_SMS_SRC} ${MAIN_SRC} ${HTTP_SRC} server/port.h)

add_subdirectory(base)

#利用SRC生成一个可执行文件
add_executable(${PROJECT_NAME} ${ALL_SRC})

target_link_libraries(${PROJECT_NAME} glog base_lib boost_thread-mt boost_system boost_filesystem ssl crypto)