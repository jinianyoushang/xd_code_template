#pragma once

#include <thread>
#include <sstream>

inline int GetThreadId(const std::thread::id tid) {
    std::stringstream ss;
    ss << tid;
    int id;
    ss >> id;
    return id;
}

inline int GetThreadId() {
    return GetThreadId(std::this_thread::get_id());
}

inline int GetThreadId(const std::thread &t) {
    return GetThreadId(t.get_id());
}
