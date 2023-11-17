cmake_minimum_required(VERSION 3.25)
project(ThreadPool)

set(CMAKE_CXX_STANDARD 17)
set(A 1)

add_subdirectory(threadpool_linux)
add_subdirectory(threadpool_all_platform)
add_subdirectory(Semaphore_c++11)
add_subdirectory(threadpool_BlockingConcurrentQueue)

add_executable(test
        test.cpp)





