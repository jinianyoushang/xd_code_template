#pragma once

#include <unordered_map>
#include "TaskQueue.hpp"
#include "WorkThread.hpp"
#include "ThreadUtils.hpp"

class ThreadPool {
public:
    explicit ThreadPool(int min = 5, int max = 20);

    ~ThreadPool();

    void AddTask(const TaskPtr&task);

    void Finish();

private:
    void AddThread();

    void DelThread();

    std::atomic_bool finished_;
    std::condition_variable condVar_;
    std::mutex mutex_;
    std::unordered_map<std::thread::id, ThreadPtr> threads_;
    std::thread manageThread_;
    TaskQueue taskQue_;

    int min_;
    int max_;
};
