cmake_minimum_required(VERSION 3.10)
project(LockFreeQueue)

set(CMAKE_CXX_STANDARD 17)

add_executable(LockFreeQueue
        LockFreeQueue.h
        LockFreeQueueTest.cpp
)

target_link_libraries(LockFreeQueue pthread rt)

