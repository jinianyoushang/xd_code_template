//
// Created by xd on 2023/6/10.
//
#include <thread>
#include <vector>
#include <semaphore>

std::semaphore sem(3);

void task() {
    sem.acquire();
    // 访问资源
    sem.release();
}

int main() {
    std::vector<std::thread> threads;

    for (int i = 0; i < 10; i++) {
        threads.push_back(std::thread(task));
    }

    for (auto& th : threads) {
        th.join();
    }
}