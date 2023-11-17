#pragma once

#include <string>
#include <memory>

class Task {
public:
    explicit Task(const std::string &name = "") : name_(name) {}
    
    std::string GetName()
    {
        return name_;
    }
    
    virtual void Run() = 0;

private:
    std::string name_;
};

using TaskPtr = std::shared_ptr<Task>;
