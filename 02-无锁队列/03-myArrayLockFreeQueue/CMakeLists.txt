cmake_minimum_required(VERSION 3.26)
project(queue)

set(CMAKE_CXX_STANDARD 17)

add_executable(queue main.cpp
        ArrayLockFreeQueue.h
        atom_opt.h)
