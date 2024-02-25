/*
 * 测试hashmap占用的内存情况
 */
#include <iostream>
#include <sys/time.h>
#include <unordered_map>

using namespace std;
#define MAP_ITEM 10000000
int main()
{
    cout << "Hello World!" << endl;

    unordered_map<string, bool> unordermp;
    timeval st, et;

    unordermp.insert(make_pair("https://blog.csdn.net/muyuyuzhong/article/details/82712147", 1));
    cout << "1 " << sizeof(unordermp) << endl;
    unordermp.insert(make_pair("https://www.processon.com/mindmap/5eb5309c0791290fe04ebac2", 1));
    cout << "2 " << sizeof(unordermp) << endl;
    unordermp.insert(make_pair("https://www.yuque.com/linuxer/xngi03/ir7gry/edit", 1));
    cout << "3 " << sizeof(unordermp) << endl;
    auto iter = unordermp.begin();//auto自动识别为迭代器类型unordered_map<int,string>::iterator
    while (iter!= unordermp.end())
    {
        cout << iter->first << "," << iter->second << endl;
        ++iter;
    }


    string main_key = "https://blog.csdn.net/muyuyuzhong/article/details/";
    gettimeofday(&st, NULL);
    for(int i = 0; i < MAP_ITEM; i++) {
        string sub_key = to_string(i);
        unordermp.insert(make_pair(main_key + sub_key, 1));
    }
    gettimeofday(&et, NULL);
    cout << "insert time:" << (et.tv_sec-st.tv_sec)*1000 + (et.tv_usec-st.tv_usec)/1000 << "ms" << endl;
    cout << "key_size: " << main_key.size() + to_string(MAP_ITEM).size()/2 << endl;
    gettimeofday(&st, NULL);
    auto item = unordermp.find(main_key + "100");
    if ( item == unordermp.end() )
        std::cout << "not found";
    else
        std::cout << item->first << " is " << item->second << endl;
    gettimeofday(&et, NULL);
    cout << "find one time:" <<  (et.tv_usec-st.tv_usec)  << "us" << endl;
    // 100000   53字节    16.7M       111ms
    // 1000000  53字节    158.0M      1165ms
    // 10000000 54字节    1599.5M     13318ms  非常耗内存

    getchar();

    return 0;
}
