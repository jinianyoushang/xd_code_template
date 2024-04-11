#include <mutex>
#include <condition_variable>
#include <future>
#include <queue>
#include <memory>
#include <iostream>
#include <set>

#include "ThreadSafeHashTable.h"
#include "threadsafetable.h"
#include "myclass.h"

using namespace std;


void insertTest(ThreadSafeHashTable<int, int>&hashTable) {
    for (int i = 0; i < 10; ++i) {
        hashTable.insert({i, i});
    }
}

void testThreadSafeHashTable() {
    ThreadSafeHashTable<int, int> hashTable;
    hashTable.insert(1, 1);
    hashTable.insert({2, 2});
    hashTable.insert({3, 3});

    cout << hashTable.size() << endl;
    cout << hashTable[2] << endl;

    cout << hashTable[0] << endl;
    cout << hashTable.size() << endl;

    cout << hashTable.find(1)->second << endl;

    cout << (hashTable.find(200) == hashTable.end()) << endl;

    thread t1(insertTest, ref(hashTable));
    thread t2(insertTest, ref(hashTable));
    t1.join();
    t2.join();
    cout << hashTable.size() << endl;
}


void TestThreadSafeHash() {
    set<int> removeSet;
    threadsafe_lookup_table<int, std::shared_ptr<MyClass>> table;
    std::thread t1([&]() {
        for(int i = 0; i < 100; i++)
        {
           auto class_ptr =  std::make_shared<MyClass>(i);
            table.add_or_update_mapping(i, class_ptr);
        }
    });

    std::thread t2([&]() {
        for (int i = 0; i < 100; )
        {
            auto find_res = table.value_for(i, nullptr);
            if(find_res)
            {
                table.remove_mapping(i);
                removeSet.insert(i);
                i++;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    });

    std::thread t3([&]() {
        for (int i = 100; i < 200; i++)
        {
            auto class_ptr = std::make_shared<MyClass>(i);
            table.add_or_update_mapping(i, class_ptr);
        }
        });


    t1.join();
    t2.join();
    t3.join();

    for(auto & i : removeSet)
    {
        std::cout << "remove data is " << i << std::endl;
    }

    auto copy_map =  table.get_map();
    for(auto & i : copy_map)
    {
        std::cout << "copy data is " << *(i.second) << std::endl;
    }
}

int main() {
    // 测试我的代码
    // testThreadSafeHashTable();

    //测试别人的代码
    TestThreadSafeHash();
}
