#pragma once
#include <queue>
#include <mutex>
#include <functional>

using Task = std::function<void()>;

class TaskQueue {
public:    
    bool Empty()
    {
        std::lock_guard<std::mutex> lock(queMutex_);
        return tasks_.empty();
    }
    
    size_t GetSize()
    {
        std::lock_guard<std::mutex> lock(queMutex_);
        return tasks_.size();
    }
    
    void AddTask(const Task &task)
    {
        std::lock_guard<std::mutex> lock(queMutex_);
        tasks_.push(task);
    }
    
    Task GetTask()
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
    std::queue<Task> tasks_;
    std::mutex queMutex_;
};

