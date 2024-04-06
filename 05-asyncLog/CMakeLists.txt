cmake_minimum_required(VERSION 3.28)
project(asyncLog)

set(CMAKE_CXX_STANDARD 17)


# 主线程cout
add_executable(asyncLog1 test1.cpp)
# 主线程printf
add_executable(asyncLog2 test2.cpp)

# 主线程cout 禁用cout同步
add_executable(asyncLog3 test3.cpp)
# 异步log输出
add_executable(asyncLog4 test4.cpp)