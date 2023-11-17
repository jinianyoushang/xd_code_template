#pragma once

#include <thread>
#include <atomic>
#include <memory>

#include "TaskQueue.hpp"


class WorkThread {
public:
    constexpr static int STATE_WAIT = 1;
    constexpr static int STATE_WORK = 2;
    constexpr static int STATE_EXIT = 3;

    explicit WorkThread(TaskQueue &taskQue);

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
    std::atomic_int state_;
    std::atomic_bool finished_;
    std::thread thread_;
};

using ThreadPtr = std::shared_ptr<WorkThread>;


