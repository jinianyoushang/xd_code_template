#pragma once
#include <thread>
#include <atomic>
#include <memory>
#include <condition_variable>
#include <sstream>
#include "task_queue.hpp"

inline unsigned long long GetThreadId(std::thread::id tid)
{
    std::stringstream ss;
    ss << tid;
    unsigned long long id;
    ss >> id;
    return id;
}

inline unsigned long long GetThreadId()
{
    return GetThreadId(std::this_thread::get_id());
}

inline unsigned long long GetThreadId(const std::thread &t)
{
    return GetThreadId(t.get_id());
}

class WorkThread {
public:
    constexpr static int STATE_WAIT = 1;
    constexpr static int STATE_WORK = 2;
    constexpr static int STATE_EXIT = 3;
    
    WorkThread(TaskQueue &taskQue, std::condition_variable &cond, std::mutex &mtx) : taskQue_(taskQue), condVar_(cond), mutex_(mtx)
    {
        thread_ = std::thread([this](){
            while (!finished_) {
                state_ = STATE_WAIT;
                {
                    std::unique_lock<std::mutex> lk(mutex_);
                    if (taskQue_.Empty()) {
                        condVar_.wait(lk, [&]() { return !taskQue_.Empty() || finished_; });
                    }
                }
                if (finished_) {
                    break;
                }
                Task task = taskQue_.GetTask();
                if (task != nullptr) {
                    state_ = STATE_WORK;
                    task();
                }
            }
            state_ = STATE_EXIT;
        });
        printf("-- thread [%llu]: started\n", GetThreadId(thread_));
    }
    
    ~WorkThread()
    {
        printf("-- thread [%llu]: finished\n", GetThreadId(thread_));
        if (thread_.joinable()) {
            thread_.join();
        }
    }
    
    int GetState()
    {
        return state_;
    }
    
    void Finish()
    {
        finished_ = true;
    }
    
    std::thread::id GetId() const {
        return thread_.get_id();
    }
    
    std::thread& GetThread()
    {
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


