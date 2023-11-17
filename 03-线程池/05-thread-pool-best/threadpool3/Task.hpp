#pragma once

#include <string>
#include <memory>
#include <utility>

class Task {
public:
    explicit Task(std::string name = "") : name_(std::move(name)) {}
    
    std::string GetName()
    {
        return name_;
    }
    
    virtual void Run() = 0;

private:
    std::string name_;
};

using TaskPtr = std::shared_ptr<Task>;
