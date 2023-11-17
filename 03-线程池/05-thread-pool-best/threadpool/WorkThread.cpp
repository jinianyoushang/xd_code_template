#include "WorkThread.hpp"
#include <chrono>
#include <cstdio>
#include "ThreadUtils.hpp"

WorkThread::WorkThread(TaskQueue&taskQue) : taskQue_(taskQue), state_(STATE_WAIT), finished_(false) {
    thread_ = std::thread([this]() {
        while (!finished_) {
            state_ = STATE_WAIT;
            while (!finished_ && taskQue_.IsEmpty()) {
                //TODO 这样相当于自旋的不太好，应该用条件变量或者信号量进行控制
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
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
