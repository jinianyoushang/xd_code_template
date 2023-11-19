cmake_minimum_required(VERSION 3.27)
project(testCMySQL)

set(CMAKE_CXX_STANDARD 17)

set(MYSQL_PATH "C:\\Program Files\\MySQL\\MySQL Server 8.0")
include_directories("${MYSQL_PATH}\\include")

link_directories("${MYSQL_PATH}\\lib")

add_executable(testCMySQL main.cpp)
target_link_libraries(testCMySQL libmysql)