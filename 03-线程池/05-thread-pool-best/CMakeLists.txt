cmake_minimum_required(VERSION 3.5)

project(thread-pool)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(threadpool)

add_executable(thread-pool example.cpp)

target_link_libraries(thread-pool threadpool)

add_executable(thread-pool2 example2.cpp)

#第三个测试用例
add_subdirectory(threadpool3)
add_executable(thread-pool3 example3.cpp)
target_link_libraries(thread-pool3 threadpool3)

target_link_libraries(thread-pool2
        pthread
)