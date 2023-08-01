#include <iostream>
#include <thread>
#include <mutex>
#include <shared_mutex>

std::shared_mutex mtx;
int data = 0;

void reader(int id)
{
    while (true) {
        std::shared_lock<std::shared_mutex> lock(mtx);
        std::cout << "Reader " << id << " read data: " << data << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void writer(int id)
{
    while (true) {
        std::unique_lock<std::shared_mutex>lock(mtx);
        data++;
        std::cout << "Writer " << id << " write data: " << data << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

int main()
{
    std::thread t1(reader, 1);
    std::thread t2(reader, 2);
    std::thread t3(writer, 1);
    t1.join();
    t2.join();
    t3.join();


    return 0;
}
