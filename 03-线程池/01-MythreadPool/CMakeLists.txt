cmake_minimum_required(VERSION 3.26)
project(MythreadPool)

set(CMAKE_CXX_STANDARD 17)

add_executable(MythreadPool mythread1.cpp
        SafeQueue.h)
add_executable(MythreadPool2 mythread2.cpp)
