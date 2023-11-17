#include <iostream>
#include <bits/stdc++.h>

using namespace std;


class MyTimer2 {
private:
    atomic<bool> m_stop;
public:
    explicit MyTimer2() : m_stop(true) {}

    //可以使用完美转发，支持各种类型任务
    void start(int interval, const std::function<void()> &task) {
        if (!m_stop) {//有任务在执行
            return;
        }
        m_stop = false;
        std::thread t([&, interval]() {
            while (!m_stop) {
                this_thread::sleep_for(chrono::milliseconds(interval));
                task();
            }
        });
        t.detach();
    }

    void stop() {
        m_stop = true;
    }
};


void printNum() {
    static int a = 0;
    a++;
    cout << "a=" << a << endl;
}

int main() {
    //自己的测试
    MyTimer2 myTimer2;
    myTimer2.start(1000, printNum);
    getchar();
    myTimer2.stop();
    getchar();
    return 0;
}