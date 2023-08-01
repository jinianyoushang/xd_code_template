#include <iostream>
#include <thread>
#include "lightweightsemaphore.h"

using namespace moodycamel::details;

using namespace std;

Semaphore semaphore(0);

int main() {


    thread t1([](){
        while (1){
            semaphore.wait();
            cout<<"semaphore once"<<endl;
        }
    });

    thread t2([](){
        while(true){
            semaphore.signal();
            this_thread::sleep_for(chrono::seconds(1));
        }
    });

    t1.join();
    t2.join();
    return 0;
}
