#pragma once

#include <queue>
#include <mutex>

#include "Task.hpp"

class TaskQueue {
public:    
    bool IsEmpty()
    {
        std::lock_guard<std::mutex> lock(queMutex_);
        return tasks_.empty();
    }
    
    size_t GetSize()
    {
        std::lock_guard<std::mutex> lock(queMutex_);
        return tasks_.size();
    }
    
    void AddTask(TaskPtr task)
    {
        std::lock_guard<std::mutex> lock(queMutex_);
        tasks_.push(task);
    }
    
    TaskPtr GetTask()
    {
        std::lock_guard<std::mutex> lock(queMutex_);
        if (tasks_.empty()) {
            return nullptr;
        }
        auto task = tasks_.front();
        tasks_.pop();
        return task;
    }

private:
    std::queue<TaskPtr> tasks_;
    std::mutex queMutex_;
};
