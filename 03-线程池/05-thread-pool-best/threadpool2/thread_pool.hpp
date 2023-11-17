#pragma once
#include <atomic>
#include <future>
#include <condition_variable>
#include <unordered_map>
#include "work_thread.hpp"


class ThreadPool {
public:
    explicit ThreadPool(const int min = 1, const int max = std::thread::hardware_concurrency()) : finished_(false),
        min_(min), max_(max) {
        for (int i = 0; i < min_; i++) {
            AddThread();
        }

        manageThread_ = std::thread([this]() {
            while (!finished_) {
                if ((taskQue_.GetSize() > 2 * threads_.size()) && (threads_.size() < max_)) {
                    AddThread();
                }
                else {
                    int cnt = 0;
                    for (auto&t: threads_) {
                        if (t.second->GetState() == WorkThread::STATE_WAIT) {
                            ++cnt;
                        }
                    }
                    if ((cnt > 2 * taskQue_.GetSize()) && (threads_.size() > min_)) {
                        DelThread();
                    }
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        });
    }

    ~ThreadPool() {
        Finish();
        if (manageThread_.joinable()) {
            manageThread_.join();
        }
    }

    void Finish() {
        finished_ = true;
        for (auto&t: threads_) {
            t.second->Finish();
        }
        std::unique_lock<std::mutex> lk(mutex_);
        condVar_.notify_all();
    }

    template<class F, class... Args>
    auto commit(F&&f, Args&&... args) -> std::future<decltype(f(args...))> {
        using RetType = decltype(f(args...));
        auto task = std::make_shared<std::packaged_task<RetType()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...));
        std::future<RetType> future = task->get_future();
        std::unique_lock<std::mutex> lk(mutex_);
        taskQue_.AddTask([task]() { (*task)(); });
        condVar_.notify_all();
        return future;
    }

    uint64_t Count() const { return threads_.size(); }

private:
    void AddThread() {
        auto tdPtr = std::make_shared<WorkThread>(taskQue_, condVar_, mutex_);
        threads_[tdPtr->GetId()] = tdPtr;
        printf("-- add thread: %llu\n", GetThreadId(tdPtr->GetId()));
    }

    void DelThread() {
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
        printf("-- del thread: %llu\n", GetThreadId(tid));
    }

    TaskQueue taskQue_;
    std::condition_variable condVar_;
    std::mutex mutex_;
    std::thread manageThread_;
    std::atomic_bool finished_;
    std::unordered_map<std::thread::id, ThreadPtr> threads_;
    int min_;
    int max_;
};
