#include "ThreadPool.hpp"

#include <iostream>

ThreadPool::ThreadPool(int min, int max) : finished_(false), min_(min), max_(max) {
    for (int i = 0; i < min_; i++) {
        AddThread();
    }

    manageThread_ = std::thread([this]() {
        while (!finished_) {
            //统计处于等待队列的线程
            int waitingCount = 0;
            for (auto&t: threads_) {
                if (t.second->GetState() == WorkThread::STATE_WAIT) {
                    ++waitingCount;
                }
            }

            std::cout << "manageThread_ is running" << " maxNum:" << max_ << " minNum:" << min_ << " TaskLen:" <<
                    taskQue_
                    .GetSize() << " ThreadNum:" << threads_.size() << " STATE_WAIT_Num:" << waitingCount <<
                    " workingCount:" << threads_.size() - waitingCount << std::endl;
            //动态添加线程
            if ((taskQue_.GetSize() > 2 * threads_.size()) && (threads_.size() < max_)) {
                AddThread();
            }
            else {
                //符合条件就动态删除线程
                if (waitingCount > 2 * taskQue_.GetSize() && threads_.size() > min_) {
                    DelThread();
                }
            }
            //休眠100ms
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    });
}

ThreadPool::~ThreadPool() {
    if (manageThread_.joinable()) {
        manageThread_.join();
    }
}


void ThreadPool::AddTask(const TaskPtr&task) {
    printf("-- add task: [%s]\n", task->GetName().c_str());
    taskQue_.AddTask(task);
    std::unique_lock<std::mutex> lk(mutex_);
    condVar_.notify_all();
}

void ThreadPool::Finish() {
    finished_ = true; //这个时候manageThread_会自动退出
    for (auto&t: threads_) {
        t.second->Finish();
    }
    std::unique_lock<std::mutex> lk(mutex_);
    condVar_.notify_all();
}

void ThreadPool::AddThread() {
    printf("-- add thread\n");
    const auto tdPtr = std::make_shared<WorkThread>(taskQue_,condVar_, mutex_);
    threads_[tdPtr->GetId()] = tdPtr;
}

void ThreadPool::DelThread() {
    printf("-- del thread\n");
    std::thread::id tid;
    for (auto&t: threads_) {
        if (t.second->GetState() == WorkThread::STATE_WAIT) {
            t.second->Finish();
            tid = t.first;
            break;
        }
    }
    threads_.erase(tid);
    std::unique_lock<std::mutex> lk(mutex_);
    condVar_.notify_all();
}
