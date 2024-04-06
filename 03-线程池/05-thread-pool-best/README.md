# 线程池项目说明

## 目录结构

```
thread-pool
|-- CMakeLists.txt # cmake工程管理文件
|-- example.cpp # 测试demo
|-- threadpool # 基于继承task
    |-- CMakeLists.txt # cmake工程管理文件
    |-- Task.hpp # 线程池中允许的任务的抽象基类
    |-- TaskQueue.hpp # 任务队列的实现
    |-- ThreadPool.hpp/cpp # 线程池的实现
    |-- ThreadUtils.hpp # 线程工具函数，提取线程ID
    |-- WorkThread.hpp/cpp # 工作线程
|-- threadpool2 # 更灵活的task，支持各种可调用对象，可使用future获取范围值
    |-- task_queue.hpp # 任务队列的实现
    |-- thread_pool.hpp # 线程池的实现
    |-- work_thread.hpp # 工作线程
```

## 使用

### 基于继承的Task(threadpool)

参照example.cpp代码

```cpp
#include <cstdio>
#include "ThreadPool.hpp"

// 定义字节的任务类，继承Task基类
class MyTask : public Task {
public:
    MyTask(int i) : Task("MyTask" + std::to_string(i)) {}
    
    // 实现Task类的Run函数，最终线程池里会调用该函数
    void Run()
    {
        for (int i = 0; i < 5; i++) {
            printf("-- thread [%3d]: task_name=[%20s], out=%d\n", GetThreadId(), GetName().c_str(), i);
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
};

int main(int argc, char **argv) {
    ThreadPool pool;
    for (int i = 0; i < 50; i++) {
        pool.AddTask(std::make_shared<MyTask>(i));
    }
    int i = 100;
    while(i < 150) {
        pool.AddTask(std::make_shared<MyTask>(i));
        std::this_thread::sleep_for(std::chrono::seconds(1));
        i++;
    }
    pool.Finish(); // 记得终止线程池
    return 0;
}

```

### 基于继承的Task(threadpool3)（推荐2）

这个是对Task(threadpool)的改进，使用条件变量控制线程，而不是让其自旋；更推荐这个，而不是之前的。



### 基于可调用对象的task(threadpool2)（推荐1）

这个用途更广一些；尽量使用这个

参照example2.cpp代码

```cpp
#include <cstdio>
#include "threadpool2/thread_pool.hpp"

// 作为task的函数
float Sum(float end)
{
    float s = 0;
    float i = 0.0f;
    while (i < end) {
        s += i;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        i += 0.5;
    }
    return s;
}

// 作为task的函数对象
struct SumFunctor {
    float end;
    SumFunctor(float e) : end(e) {}

    float operator()()
    {
        float s = 0;
        float i = 0.0f;
        while (i < end) {
            s += i;
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            i += 0.5;
        }
        return s;
    }
};

int main(int argc, char **argv) {
    ThreadPool pool;
    std::vector<std::future<float>> futures; // 用于获取task执行结果
    for (int i = 0; i < 20; i++) {
        // 添加lambda表达式形式的task，并将返回结果放到future，方便后续获取执行结果
        futures.emplace_back(pool.commit([](int k) -> float {
            float s = 0;
            for (int j = 0; j < k; j++) {
                s += j;
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
            return s;
        }, i));
    }
    // 获取执行结果
    for (auto &f : futures) {
        auto ret = f.get();
        printf("[lambda] result: %f\n", ret);
    }
    futures.clear();
    float i = 100;
    while(i < 110) {
        // 添加函数形式的task
        futures.emplace_back(pool.commit(Sum, i));
        std::this_thread::sleep_for(std::chrono::seconds(1));
        i += 1.0f;
    }
    for (auto &f : futures) {
        auto ret = f.get();
        printf("[function] result: %f\n", ret);
    }

    futures.clear();
    while(i < 120) {
        // 添加函数对象形式的task
        futures.emplace_back(pool.commit(SumFunctor(i)));
        std::this_thread::sleep_for(std::chrono::seconds(1));
        i += 1.0f;
    }
    for (auto &f : futures) {
        auto ret = f.get();
        printf("[functor] result: %f\n", ret);
    }
    return 0;
}
```