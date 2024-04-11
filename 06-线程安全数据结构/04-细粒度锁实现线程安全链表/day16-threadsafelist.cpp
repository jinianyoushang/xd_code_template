// day16-threadsafelist.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "threadsafelist.h"
#include "tailpushlist.h"
#include "MyClass.h"
#include <set>
#include "tailpushlist.h"

std::set<int> removeSet;
void TestThreadSafeList()
{
	
	threadsafe_list<MyClass> thread_safe_list;
	std::thread t1([&]()
	{
		for(unsigned int i = 0; i < 100; i++)
		{
			MyClass mc(i);
			thread_safe_list.push_front(mc);
		}
		
	});


	std::thread t2([&]()
	{
		for (unsigned int i = 0; i < 100; )
		{
			
			auto find_res = thread_safe_list.find_first_if([&]( auto & mc)
			{
					return mc.GetData() == i;
			});

			if(find_res == nullptr)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
				continue;
			}

			removeSet.insert(i);
			i++;
		}
	});

	t1.join();
	t2.join();

}


void TestTailPush()
{
	double_push_list<MyClass> thread_safe_list;
	for(int i = 0; i < 10; i++)
	{

		MyClass mc(i);
		thread_safe_list.push_front(mc);
	}

	thread_safe_list.for_each([](const MyClass & mc)
	{
			std::cout << "for each print " << mc << std::endl;
	});


	for(int i = 10; i < 20; i++)
	{
		MyClass mc(i);
		thread_safe_list.push_back(mc);
	}

	thread_safe_list.for_each([](const MyClass& mc)
		{
			std::cout << "for each print " << mc << std::endl;
		});

	auto find_data = thread_safe_list.find_first_if([](const MyClass& mc)
		{
			return (mc.GetData() == 19);
		});

	if(find_data)
	{
		std::cout << "find_data is " << find_data->GetData() << std::endl;
	}


	thread_safe_list.insert_if([](const MyClass& mc)
		{
			return (mc.GetData() == 19);
		},20);


	thread_safe_list.for_each([](const MyClass& mc)
		{
			std::cout << "for each print " << mc << std::endl;
		});
}

void MultiThreadPush()
{
	double_push_list<MyClass> thread_safe_list;

	std::thread t1([&]()
	{
			for (int i = 0; i < 20000; i++)
			{
				MyClass mc(i);
				thread_safe_list.push_front(mc);
				std::cout << "push front " << i << " success" << std::endl;
			}
	});

	std::thread t2([&]()
	{
			for (int i = 20000; i < 40000; i++)
			{
				MyClass mc(i);
				thread_safe_list.push_back(mc);
				std::cout << "push back " << i << " success" << std::endl;
			}
	});

	std::thread t3([&]()
	{
			for(int i = 0; i < 40000; )
			{
				bool rmv_res = thread_safe_list.remove_first([&](const MyClass& mc)
					{
						return mc.GetData() == i;
					});

				if(!rmv_res)
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(10));
					continue;
				}

				i++;
			}
	});

	t1.join();
	t2.join();
	t3.join();

	std::cout << "begin for each print...." << std::endl;
	thread_safe_list.for_each([](const MyClass& mc)
		{
			std::cout << "for each print " << mc << std::endl;
		});
	std::cout << "end for each print...." << std::endl;
}

int main()
{
	//TestThreadSafeList();
	//for(auto & v: removeSet)
	//{
	//	std::cout << "remove data is " << v << std::endl;
	//}

	//TestTailPush();
	MultiThreadPush();

    std::cout << "Hello World!\n";
}


