//
// Created by xd on 2023/6/11.
//
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
#include "Semaphore.h"

std::string FormatTimeNow(const char* format) {
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm* now_tm = std::localtime(&now_c);

    char buf[20];
    std::strftime(buf, sizeof(buf), format, now_tm);
    return std::string(buf);
}

Semaphore g_semaphore(2);
std::mutex g_io_mtx;

void DoWork() {
    g_semaphore.Wait();

    std::thread::id thread_id = std::this_thread::get_id();
    std::string now = FormatTimeNow("%H:%M:%S");
    {
        std::lock_guard<std::mutex> lock(g_io_mtx);
        std::cout << "Thread " << thread_id << ": wait succeeded"
                  << " (" << now << ")" << std::endl;
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));

    g_semaphore.Signal();
}

int main() {
    int threadNum = 4;
    std::vector<std::thread> v;
    v.reserve(threadNum);
    for (std::size_t i = 0; i < threadNum; ++i) {
        v.emplace_back(&DoWork);
    }
    for (std::thread& t : v) {
        t.join();
    }
    return 0;
}
