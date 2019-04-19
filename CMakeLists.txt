# 支持的最低版本
cmake_minimum_required(VERSION 2.6)
cmake_policy(SET CMP0012 NEW)


#项目名称
project(campus_chat)

#支持C++11
add_definitions(-std=c++14)

set(INC_MYSQL /usr/local/Cellar/mysql/5.7.20/include/mysql/)
set(LINK_MYSQL /usr/local/Cellar/mysql/5.7.20/lib/)
set(INC_DIR /usr/local/include)
set(INC_GTEST ./footbook/gtest/include)
set(LINK_DIR /usr/local/lib)

include_directories(./)
include_directories(./footbook)
include_directories(${INC_GTEST})
include_directories(${INC_DIR})
include_directories(${INC_MYSQL})
link_directories(${LINK_DIR})
link_directories(${LINK_MYSQL})

option(LEVELDB_BUILD_TESTS "Build LevelDB's unit tests" ON)

set(SERVER_SRC
        footbook/server.h
        footbook/server.cc
        footbook/io_service_pool.cc
        footbook/io_service_pool.h
        footbook/talk_to_client.cc
        footbook/talk_to_client.h
        footbook/glog_helper.h
        footbook/glog_helper.cc
        footbook/message.cc
        footbook/message.h
        footbook/alias.h
        footbook/listener.cc
        footbook/listener.h
        footbook/config.h
        footbook/config.cc
        footbook/client.cc
        footbook/client.h
        footbook/status.cc
        footbook/status.h
        footbook/limit.h)

if (LEVELDB_BUILD_TESTS)
    set(MAIN_SRC footbooktest/test_main.cc)
    set(TEST_SRC footbooktest/message_unitest.cc)
else()
    set(MAIN_SRC campus_chat_main.cc)
    set(TEST_SRC)
endif()

set(ALIBABA_SMS_SRC
        footbook/alibaba_sms/sms.h
        footbook/alibaba_sms/sms.cc)

set(HTTP_SRC
        footbook/http/http_base.h
        footbook/http/http_base.cc
        footbook/http/http_boost.cc
        footbook/http/http_boost.h)

set(DB_SRC
        footbook/db/mysql_interface.h
        footbook/db/mysql_interface.cc
        footbook/db/db.cc
        footbook/db/db.h
        footbook/db/table.cc
        footbook/db/table.h
        footbook/db/table_struct.h
        footbook/db/table_struct.cc)

set(ALL_SRC ${SERVER_SRC} ${ALIBABA_SMS_SRC} ${MAIN_SRC} ${HTTP_SRC} ${DB_SRC} ${TEST_SRC} footbook/port.h)

add_subdirectory(footbook/base)
add_subdirectory(footbook/gtest)

#利用SRC生成一个可执行文件
add_executable(${PROJECT_NAME} ${ALL_SRC})

target_link_libraries(${PROJECT_NAME} footbook_gtest glog base_lib boost_thread-mt
        boost_system boost_filesystem ssl crypto mysqlclient)