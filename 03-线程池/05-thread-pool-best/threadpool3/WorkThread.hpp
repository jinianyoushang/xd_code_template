#pragma once

#include <thread>
#include <atomic>
#include <condition_variable>
#include <memory>

#include "TaskQueue.hpp"

//我觉得这样封装挺好的RAII
class WorkThread {
public:
    constexpr static int STATE_WAIT = 1;
    constexpr static int STATE_WORK = 2;
    constexpr static int STATE_EXIT = 3;

    explicit WorkThread(TaskQueue &taskQue, std::condition_variable &cond, std::mutex &mtx);

    ~WorkThread();

    int GetState() {
        return state_;
    }

    void Finish() {
        finished_ = true;
    }

    std::thread::id GetId() const {
        return thread_.get_id();
    }

    std::thread& GetThread() {
        return thread_;
    }

private:
    TaskQueue &taskQue_;
    std::condition_variable &condVar_;
    std::mutex &mutex_;
    std::atomic_int state_;
    std::atomic_bool finished_;
    std::thread thread_;
};

using ThreadPtr = std::shared_ptr<WorkThread>;


