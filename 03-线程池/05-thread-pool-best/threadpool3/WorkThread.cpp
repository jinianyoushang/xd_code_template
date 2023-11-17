#include "WorkThread.hpp"
#include <cstdio>
#include "ThreadUtils.hpp"

WorkThread::WorkThread(TaskQueue&taskQue, std::condition_variable&cond, std::mutex&mtx) : taskQue_(taskQue),
    condVar_(cond), mutex_(mtx), state_(STATE_WAIT), finished_(false) {
    thread_ = std::thread([this]() {
        while (!finished_) {
            state_ = STATE_WAIT;
            {
                std::unique_lock<std::mutex> lk(mutex_);
                if (taskQue_.IsEmpty()) {
                    condVar_.wait(lk, [&]() { return !taskQue_.IsEmpty() || finished_; });
                }
            }
            if (finished_) {
                break;
            }
            TaskPtr task = taskQue_.GetTask();
            if (task != nullptr) {
                state_ = STATE_WORK;
                printf("-- thread [%3d]: run task [%20s] begin\n", GetThreadId(), task->GetName().c_str());
                task->Run();
                printf("-- thread [%3d]: run task [%20s] end\n", GetThreadId(), task->GetName().c_str());
            }
        }
        state_ = STATE_EXIT;
    });
    printf("-- thread [%3d]: started\n", GetThreadId(thread_));
}

WorkThread::~WorkThread() {
    printf("-- thread [%3d]: finished\n", GetThreadId(thread_));
    if (thread_.joinable()) {
        thread_.join();
    }
}
