cmake_minimum_required(VERSION 3.25)
project(testUniquePtr)

set(CMAKE_CXX_STANDARD 17)

add_executable(testFileData main.cpp
        FileData.cpp)

add_executable(testUniquePtr main2.cpp
        FileData2.cpp)
