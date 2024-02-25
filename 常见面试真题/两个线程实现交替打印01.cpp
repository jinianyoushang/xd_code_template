#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <list>
#include <bits/stdc++.h>

using namespace std;

int i = 1;
condition_variable condi;
mutex m;

void fun(int flag) {
    while (i <= 100) {
        unique_lock<mutex> guard(m);
        condi.wait(guard, [&]() {
            return i % 2 == flag;
        });
        if (i <= 100)
            cout << i << endl;
        i++;
        condi.notify_one();
    }
}

int main() {

    thread t1(fun, 1);
    thread t2(fun, 0);
    t1.join();
    t2.join();

    return 0;
}
