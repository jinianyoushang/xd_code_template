// day18-LockFreeQue.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "singlepushpop.h"
#include <thread>
#include "lockfreeque.h"
#include <cassert>
#include "crushque.h"
#include "memoryleakque.h"


void TestSinglePopPush() {
    SinglePopPush<int>  que;
    std::thread t1([&]() {
        for (int i = 0; i < 10000; i++) {
            que.push(i);
            std::cout << "push data is " << i << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        });

    std::thread t2([&]() {
        for (int i = 0; i < 10000; i++) {
           auto p =  que.pop();
           if (p == nullptr) {
               std::this_thread::sleep_for(std::chrono::milliseconds(10));
               continue;
           }
           
           std::cout << "pop data is " << *p << std::endl;
        }
      });

    t1.join();
    t2.join();
}

#define TESTCOUNT 10
void TestLockFreeQue() {
    lock_free_queue<int>  que;
    std::thread t1([&]() {
        for (int i = 0; i < TESTCOUNT; i++) {
            que.push(i);
            std::cout << "push data is " << i << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        });

   

	std::thread t2([&]() {
		for (int i = 0; i < TESTCOUNT;) {
			auto p = que.pop();
			if (p == nullptr) {
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
				continue;
			}
			i++;
			std::cout << "pop data is " << *p << std::endl;
		}
		});

    t1.join();
	t2.join();

    assert(que.destruct_count == TESTCOUNT);
}

void TestLockFreeQueBase() {
   
     lock_free_queue<int>  que;
    std::thread t1([&]() {
        for (int i = 0; i < TESTCOUNT; i++) {
            que.pop();
            que.pop();
            que.push(i);
            std::cout << "push data is " << i << std::endl;
            auto data = que.pop();
            std::cout << "pop data is " << *data << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        });

	t1.join();
}


void TestLockFreeQueMultiPop() {
	lock_free_queue<int>  que;
	std::thread t1([&]() {
		for (int i = 0; i < TESTCOUNT*200; i++) {
			que.push(i);
			std::cout << "push data is " << i << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
		});



	std::thread t2([&]() {
		for (int i = 0; i < TESTCOUNT*100;) {
			auto p = que.pop();
			if (p == nullptr) {
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
				continue;
			}
			i++;
			std::cout << "pop data is " << *p << std::endl;
		}
		});

	std::thread t3([&]() {
		for (int i = 0; i < TESTCOUNT*100;) {
			auto p = que.pop();
			if (p == nullptr) {
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
				continue;
			}
			i++;
			std::cout << "pop data is " << *p << std::endl;
		}
		});

	t1.join();
	t2.join();
	t3.join();

	assert(que.destruct_count == TESTCOUNT*200);
}


void TestLockFreeQueMultiPushPop() {
	lock_free_queue<int>  que;
	std::thread t1([&]() {
		for (int i = 0; i < TESTCOUNT * 100; i++) {
			que.push(i);
			std::cout << "push data is " << i << std::endl;
			//std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
		});

	std::thread t4([&]() {
		for (int i = TESTCOUNT*100; i < TESTCOUNT * 200; i++) {
			que.push(i);
			std::cout << "push data is " << i << std::endl;
			//std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
		});

	std::thread t2([&]() {
		for (int i = 0; i < TESTCOUNT * 100;) {
			auto p = que.pop();
			if (p == nullptr) {
				//std::this_thread::sleep_for(std::chrono::milliseconds(10));
				continue;
			}
			i++;
			std::cout << "pop data is " << *p << std::endl;
		}
		});

	std::thread t3([&]() {
		for (int i = 0; i < TESTCOUNT * 100;) {
			auto p = que.pop();
			if (p == nullptr) {
				//std::this_thread::sleep_for(std::chrono::milliseconds(10));
				continue;
			}
			i++;
			std::cout << "pop data is " << *p << std::endl;
		}
		});

	t1.join();
	t2.join();
	t3.join();
	t4.join();
	assert(que.destruct_count == TESTCOUNT * 200);
}


void TestCrushQue() {
	crush_que<int>  que;
	std::thread t1([&]() {
		for (int i = 0; i < TESTCOUNT*10000; i++) {
			que.push(i);
			std::cout << "push data is " << i << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
		});



	std::thread t2([&]() {
		for (int i = 0; i < TESTCOUNT*10000;) {
			auto p = que.pop();
			if (p == nullptr) {
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
				continue;
			}
			i++;
			std::cout << "pop data is " << *p << std::endl;
		}
		});

	t1.join();
	t2.join();

}

void TestLeakQue() {
	memoryleak_que<int>  que;
	std::thread t1([&]() {
		for (int i = 0; i < TESTCOUNT ; i++) {
			que.push(i);
			std::cout << "push data is " << i << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
		});



	std::thread t2([&]() {
		for (int i = 0; i < TESTCOUNT ;) {
			auto p = que.pop();
			if (p == nullptr) {
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
				continue;
			}
			i++;
			std::cout << "pop data is " << *p << std::endl;
		}
		});

	t1.join();
	t2.join();

	assert(que.destruct_count == TESTCOUNT );

}


void TestLockFreeQueMultiPushPop2() {
	lock_free_queue<int>  que;

	std::thread t1([&]() {
		for (int i = 0; i < TESTCOUNT * 100; i++) {
			que.push(i);
			std::cout << "push data is " << i << std::endl;
			// std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
		});

	std::thread t2([&]() {
		for (int i = 0; i < TESTCOUNT * 100; i++) {
			que.push(i);
			std::cout << "push data is " << i << std::endl;
			// std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
		});

	std::thread t3([&]() {
		for (int i = 0; i < TESTCOUNT * 100; i++) {
			que.push(i);
			std::cout << "push data is " << i << std::endl;
			// std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
		});

	std::thread t4([&]() {
		for (int i = 0; i < TESTCOUNT * 100; i++) {
			que.push(i);
			std::cout << "push data is " << i << std::endl;
			// std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
		});

	std::thread t5([&]() {
		for (int i = 0; i < TESTCOUNT * 100;) {
			auto p = que.pop();
			if (p == nullptr) {
				 //std::this_thread::sleep_for(std::chrono::milliseconds(10));
				continue;
			}
			i++;
			std::cout << "pop data is " << *p << std::endl;
		}
		});

	std::thread t6([&]() {
		for (int i = 0; i < TESTCOUNT * 100;) {
			auto p = que.pop();
			if (p == nullptr) {
				 //std::this_thread::sleep_for(std::chrono::milliseconds(10));
				continue;
			}
			i++;
			std::cout << "pop data is " << *p << std::endl;
		}
		});
	std::thread t7([&]() {
		for (int i = 0; i < TESTCOUNT * 100;) {
			auto p = que.pop();
			if (p == nullptr) {
				 //std::this_thread::sleep_for(std::chrono::milliseconds(10));
				continue;
			}
			i++;
			std::cout << "pop data is " << *p << std::endl;
		}
		});

	std::thread t8([&]() {
		for (int i = 0; i < TESTCOUNT * 100;) {
			auto p = que.pop();
			if (p == nullptr) {
				//std::this_thread::sleep_for(std::chrono::milliseconds(10));
				continue;
			}

			i++;
			std::cout << "pop data is " << *p << std::endl;
		}
		});
	t1.join();
	t2.join();
	t3.join();
	t4.join();
	t5.join();
	t6.join();
	t7.join();
	t8.join();
	std::cout << "construct count is " << que.construct_count << std::endl;
	std::cout << "destruct count is " << que.destruct_count << std::endl;
	assert(que.destruct_count == TESTCOUNT * 100*4);
}

int main()
{
	//测试单线程pop和单线程push
	//TestSinglePopPush();

	//测试崩溃版本
	//TestCrushQue();

	//测试内存泄露版本
	//TestLeakQue();


    //TestLockFreeQue();
   // TestLockFreeQueBase();
	//TestLockFreeQueMultiPop();
	//测试多个生产和多个消费线程
	//TestLockFreeQueMultiPushPop();
	TestLockFreeQueMultiPushPop2();
	
    std::cout << "Hello World!\n";
}


