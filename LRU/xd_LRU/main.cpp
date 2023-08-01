#include <iostream>
#include "xd_lru.h"


using namespace std;

int main() {
    LRUCache<string, string> lruCache(2);
    lruCache.put("1", "1");
    lruCache.put("2", "2");

    try {
        cout << lruCache.get("1") << endl;
    } catch (const char *msg) {
        cerr << "ERROR:" << msg << endl;
    }

    lruCache.put("3", "3");
    lruCache.put("4", "4");
    try {
        cout << lruCache.get("1") << endl; //这里产生异常，因为空间为最大为2
    } catch (const char *msg) {
        cerr << "ERROR:" << msg << endl;
    }

    return 0;
}
