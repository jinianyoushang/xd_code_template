#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <atomic>
#include "ArrayLockFreeQueue.h"

using namespace std;

atomic_bool toBreadk = false;
ArrayLockFreeQueue<int> arrayLockFreeQueue;


void insert() {
    for (int i = 0; i < 10000; ++i) {
        std::this_thread::sleep_for(chrono::milliseconds(10));
        arrayLockFreeQueue.enqueue(i);
    }
    toBreadk = true;
}


void output() {
    while (!toBreadk) {
        int temp;
        if(arrayLockFreeQueue.dequeue(temp)){
            cout << temp << endl;
        }else{
            sched_yield();
        }
    }
}

void test01() {
    ArrayLockFreeQueue<int> arrayLockFreeQueue1;
    vector<int> temp{1, 2, 3, 4, 5, 6};
    for (const auto &item: temp) {
        arrayLockFreeQueue1.enqueue(item);
    }

    int data;
    while (arrayLockFreeQueue1.try_dequeue(data)) {
        cout << data << endl;
    }
}


int main() {
    thread t1(insert);
    thread t2(output);
    t1.join();
    t2.join();

    return 0;
}
