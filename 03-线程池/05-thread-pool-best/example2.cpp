#include <cstdio>
#include <iostream>

#include "threadpool2/thread_pool.hpp"

using namespace std;

float Sum(float end) {
    float s = 0;
    float i = 0.0f;
    while (i < end) {
        s += i;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        i += 0.5;
    }
    return s;
}

struct SumFunctor {
    float end;

    explicit SumFunctor(float e) : end(e) {
    }

    float operator()() const {
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

int main(int argc, char** argv) {
    ThreadPool pool;
    std::vector<std::future<float>> futures;
    //xd test
    auto res = pool.commit([]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        return 1 + 1;
    });
    cout << res.get() << endl;


    futures.reserve(20);
    for (int i = 0; i < 20; i++) {
        futures.emplace_back(pool.commit([](int k) -> float {
            float s = 0;
            for (int j = 0; j < k; j++) {
                s += j;
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
            return s;
        }, i));
    }
    for (auto&f: futures) {
        auto ret = f.get();
        printf("[lambda 1] result: %f\n", ret);
    }
    futures.clear();
    float i = 100;
    while (i < 110) {
        futures.emplace_back(pool.commit(Sum, i));
        std::this_thread::sleep_for(std::chrono::seconds(1));
        i += 1.0f;
    }
    for (auto&f: futures) {
        auto ret = f.get();
        printf("[function 2] result: %f\n", ret);
    }

    futures.clear();
    while (i < 120) {
        futures.emplace_back(pool.commit(SumFunctor(i)));
        std::this_thread::sleep_for(std::chrono::seconds(1));
        i += 1.0f;
    }
    for (auto&f: futures) {
        auto ret = f.get();
        printf("[functor 3] result: %f\n", ret);
    }


    return 0;
}
