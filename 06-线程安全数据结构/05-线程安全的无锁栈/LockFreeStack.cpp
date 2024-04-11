// LockFreeStack.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "LockFreeStack.h"
#include <thread>
#include <set>
#include <mutex>
#include <cassert>
#include "HazardPointerStack.h"
#include "RefCountStack.h"
#include "SingleRefStack.h"


hazard_pointer hazard_pointers[max_hazard_pointers];

void TestLockFreeStack() {

    lock_free_stack<int> lk_free_stack;
    std::set<int>  rmv_set;
    std::mutex set_mtx;

    std::thread t1([&]() {
        for (int i = 0; i < 20000; i++) {
            lk_free_stack.push(i);
            std::cout << "push data " << i << " success!" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
            }
        });

    std::thread t2([&]() {
		for (int i = 0; i < 10000;) {
            auto head = lk_free_stack.pop();
            if (!head) {
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
				continue;
            }
			std::lock_guard<std::mutex> lock(set_mtx);
			rmv_set.insert(*head);
            std::cout << "pop data " << *head << " success!" << std::endl;
            i++;
		}
      });

	std::thread t3([&]() {
		for (int i = 0; i < 10000;) {
			auto head = lk_free_stack.pop();
            if (!head) {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                continue;
            }
            std::lock_guard<std::mutex> lock(set_mtx);
            rmv_set.insert(*head);
            std::cout << "pop data " << *head << " success!" << std::endl;
            i++;
		}
		});

    t1.join();
    t2.join();
    t3.join();

    assert(rmv_set.size() == 20000);
}


void TestHazardPointer() {
    hazard_pointer_stack<int> hazard_stack;
    std::set<int>  rmv_set;
    std::mutex set_mtx;

    std::thread t1([&]() {
        for (int i = 0; i < 20000; i++) {
            hazard_stack.push(i);
            std::cout << "push data " << i << " success!" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
        });

    std::thread t2([&]() {
        for (int i = 0; i < 10000;) {
            auto head = hazard_stack.pop();
            if (!head) {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                continue;
            }
            std::lock_guard<std::mutex> lock(set_mtx);
            rmv_set.insert(*head);
            std::cout << "pop data " << *head << " success!" << std::endl;
            i++;
        }
        });

    std::thread t3([&]() {
        for (int i = 0; i < 10000;) {
            auto head = hazard_stack.pop();
            if (!head) {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                continue;
            }
            std::lock_guard<std::mutex> lock(set_mtx);
            rmv_set.insert(*head);
            std::cout << "pop data " << *head << " success!" << std::endl;
            i++;
        }
        });

    t1.join();
    t2.join();
    t3.join();

    assert(rmv_set.size() == 20000);
}

void TestRefCountStack() {
    ref_count_stack<int>  ref_count_stack;
	std::set<int>  rmv_set;
	std::mutex set_mtx;

	std::thread t1([&]() {
		for (int i = 0; i < 20000; i++) {
            ref_count_stack.push(i);
			std::cout << "push data " << i << " success!" << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
		}
		});

	std::thread t2([&]() {
		for (int i = 0; i < 10000;) {
			auto head = ref_count_stack.pop();
			if (!head) {
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
				continue;
			}
			std::lock_guard<std::mutex> lock(set_mtx);
			rmv_set.insert(*head);
			std::cout << "pop data " << *head << " success!" << std::endl;
			i++;
		}
		});

	std::thread t3([&]() {
		for (int i = 0; i < 10000;) {
			auto head = ref_count_stack.pop();
			if (!head) {
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
				continue;
			}
			std::lock_guard<std::mutex> lock(set_mtx);
			rmv_set.insert(*head);
			std::cout << "pop data " << *head << " success!" << std::endl;
			i++;
		}
		});

	t1.join();
	t2.join();
	t3.join();

	assert(rmv_set.size() == 20000);
}

void TestSingleRefStack() {
	single_ref_stack<int>  single_ref_stack;
	std::set<int>  rmv_set;
	std::mutex set_mtx;

	std::thread t1([&]() {
		for (int i = 0; i < 20000; i++) {
			single_ref_stack.push(i);
			std::cout << "push data " << i << " success!" << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
		}
		});

	std::thread t2([&]() {
		for (int i = 0; i < 10000;) {
			auto head = single_ref_stack.pop();
			if (!head) {
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
				continue;
			}
			std::lock_guard<std::mutex> lock(set_mtx);
			rmv_set.insert(*head);
			std::cout << "pop data " << *head << " success!" << std::endl;
			i++;
		}
		});

	std::thread t3([&]() {
		for (int i = 0; i < 10000;) {
			auto head = single_ref_stack.pop();
			if (!head) {
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
				continue;
			}
			std::lock_guard<std::mutex> lock(set_mtx);
			rmv_set.insert(*head);
			std::cout << "pop data " << *head << " success!" << std::endl;
			i++;
		}
		});

	t1.join();
	t2.join();
	t3.join();

	assert(rmv_set.size() == 20000);
}

void TestReleaseSeq() {
	int data = 0;
	std::atomic<int> flag = 0;
	std::thread t1([&]() {
		data = 42;  //1
		flag.store(1, std::memory_order_release); //2
		});

	std::thread  t2([&]() {
		//3
		while (!flag.load(std::memory_order_acquire));
		//4 
		assert(data == 42);
	});

	t1.join();
	t2.join();
}

std::vector<int> queue_data;
std::atomic<int> count;
std::atomic<bool> store_finish = false;
void populate_queue()
{
	unsigned const number_of_items = 20;
	queue_data.clear();
	for (unsigned i = 0; i < number_of_items; ++i)
	{
		queue_data.push_back(i);
	}
	// 1 最初的存储操作
	count.store(number_of_items, std::memory_order_release);   
	store_finish.store(true, std::memory_order_relaxed);
}

void consume_queue_items()
{
	while (true)
	{
		//2等待存储完成
		while (!store_finish.load(std::memory_order_relaxed));

		int item_index;
		//3 读—改—写”操作
		if ((item_index = count.fetch_sub(1, std::memory_order_acquire)) <= 0)   
		{
			return;
		}
		//4 从内部容器queue_data 读取数据项是安全行为
		std::cout << "queue_data is  " << queue_data[item_index-1] << std::endl;
	}
}

void TestReleaseSeq2() {
	std::thread a(populate_queue);
	std::thread b(consume_queue_items);
	std::thread c(consume_queue_items);
	a.join();
	b.join();
	c.join();
}


int main()
{
    //TestLockFreeStack();
    //TestHazardPointer();
   TestRefCountStack();
	//TestSingleRefStack();
	//TestReleaseSeq();
	//TestReleaseSeq2();
    std::cout << "Hello World!\n";
}


