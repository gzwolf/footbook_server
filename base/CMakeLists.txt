# 支持的最低版本
cmake_minimum_required(VERSION 2.6)
cmake_policy(SET CMP0012 NEW)

#支持C++11
add_definitions(-std=c++14)

include_directories(../)
include_directories(./)

set(BASE_SRC
        at_exit.h at_exit.cc
        base_export.h
        bind_util.h
        callback.h
        command_line.h
        command_line.cc
        default_destroy.h
        hash.h hash.cc
        lazy_instance.h
        lazy_instance_helpers.h lazy_instance_helpers.cc
        location.h location.cc
        logging.h logging.cc
        lru_cache.h
        macor.h
        pending_task.h pending_task.cc
        post_task_and_reply_with_result_internal.h
        ptr_util.h
        random.h
        run_loop.h run_loop.cc
        sequenced_task_runner.h sequenced_task_runner.cc
        sequenced_task_runner_handle.h sequenced_task_runner_handle.cc
        sequenced_task_runner_helpers.h
        single_thread_task_runner.h
        singleton.h
        skiplist.h
        string_util.h string_util.cc
        superfasthash.cc
        task_runner.h task_runner.cc
        task_runner_util.h
        work_stealing_queue.h
        function_wrapper.h
        thread_safe_queue.h)

set(MESSAGE_LOOP_SRC
        message_loop/incoming_task_queue.h
        message_loop/incoming_task_queue.cc
        message_loop/message_loop.h
        message_loop/message_loop.cc
        message_loop/message_loop_task_runner.h
        message_loop/message_loop_task_runner.cc
        message_loop/message_pump.h
        message_loop/message_pump.cc
        message_loop/message_pump_default.h
        message_loop/message_pump_default.cc)
set(PORT_SRC
        port/atomic_pointer.h
        port/port.h
        port/port_stdxx.h
        port/thread_annotations.h)
set(THREADING_SRC
        threading/browser_process_sub_thread.h
        threading/browser_process_sub_thread.cc
        threading/browser_thread.h
        threading/browser_thread_impl.cc
        threading/browser_thread_impl.h
        threading/platform_thread.h
        threading/platform_thread.cc
        threading/post_task_and_reply_impl.h
        threading/post_task_and_reply_impl.cc
        threading/thread.h
        threading/thread.cc
        threading/thread_pool.h
        threading/thread_pool.cc
        threading/thread_task_runner_handle.h
        threading/thread_task_runner_handle.cc)
set(TIMER_SRC
        timer/timer.h
        timer/timer.cc)

set(ALL_BASE_SRC ${BASE_SRC} ${MESSAGE_LOOP_SRC} ${PORT_SRC}
        ${THREADING_SRC} ${TIMER_SRC})


add_library(base_lib ${ALL_BASE_SRC})