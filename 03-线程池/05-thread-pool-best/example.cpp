#include <cstdio>
#include "threadpool/ThreadPool.hpp"

class MyTask : public Task {
public:
    MyTask(const int i, const int start) : Task("MyTask" + std::to_string(i)), start_(start) {
    }

    void Run() override {
        for (int i = 0; i < 1; i++) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            printf("-- thread [%3d]: task_name=[%20s], out=%d\n", GetThreadId(), GetName().c_str(), start_ + i);
        }
    }

private:
    int start_;
};

int main(int argc, char** argv) {
    ThreadPool pool;
    for (int i = 0; i < 50; i++) {
        pool.AddTask(std::make_shared<MyTask>(i, i));
    }
    int i = 100;
    while (i < 150) {
        pool.AddTask(std::make_shared<MyTask>(i, i));
        std::this_thread::sleep_for(std::chrono::seconds(1));
        i++;
    }
    pool.Finish();
    return 0;
}
