#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <random>

using namespace std;

// 全O(1)的数据结构，返回计数最小和最大的字符串。
// 测试链接 : https://leetcode.cn/problems/all-oone-data-structure/
//请你设计一个用于存储字符串计数的数据结构，并能够返回计数最小和最大的字符串。
/*
实现 AllOne 类：

AllOne() 初始化数据结构的对象。
inc(String key) 字符串 key 的计数增加 1 。如果数据结构中尚不存在 key ，那么插入计数为 1 的 key 。
dec(String key) 字符串 key 的计数减少 1 。如果 key 的计数在减少后为 0 ，那么需要将这个 key 从数据结构中删除。测试用例保证：在减少计数前，key 存在于数据结构中。
getMaxKey() 返回任意一个计数最大的字符串。如果没有元素存在，返回一个空字符串 "" 。
getMinKey() 返回任意一个计数最小的字符串。如果没有元素存在，返回一个空字符串 "" 。
注意：每个函数都应当满足 O(1) 平均时间复杂度。
 //假设操作一直合法
 */
class AllOne {
private:
    //内部桶
    struct Bucket {
        unordered_set<string> set;
        int count = 0;//词频
        Bucket *prev = nullptr;
        Bucket *next = nullptr;

        Bucket(const string &s, int count) : count(count) {
            set.insert(s);
        }

        explicit Bucket(int count) : count(count) {}
    };

    Bucket *head;
    Bucket *tail;
    unordered_map<string, Bucket *> umap;//用来O(1)时间定位桶的位置
private:

    //插入到当前桶的右边
    void insertRight(Bucket *bucket, Bucket *newBucket) {
        bucket->next->prev = newBucket;
        newBucket->next = bucket->next;
        bucket->next = newBucket;
        newBucket->prev = bucket;
    }

    //删除一个桶
    void remove(Bucket *bucket) {
        auto pre = bucket->prev;
        auto next = bucket->next;
        pre->next = next;
        next->prev = pre;
        delete bucket;
    }

public:


    AllOne() {
        head = new Bucket("", 0);
        tail = new Bucket("", INT32_MAX);
        head->next = tail;
        tail->prev = head;
    }

    void inc(string key) {
        //如果存在这个桶
        if (umap.count(key)) {
            int curCount = umap[key]->count;
            auto curBucket = umap[key];
            //判断下一个桶是否存在
            if (umap[key]->next->count == curCount + 1) {
            } else {
                insertRight(curBucket, new Bucket(curCount + 1));
            }
            curBucket->next->set.insert(key);
            curBucket->set.erase(key);
            umap[key] = curBucket->next;
            //如果当前为空，应当清除
            if (curBucket->set.empty()) {
                remove(curBucket);
            }
        } else {//如果不存在
            //判断1的桶是否存在
            if (head->next->count == 1) {
            } else {
                insertRight(head, new Bucket(1));
            }
            head->next->set.insert(key);
            umap[key] = head->next;
        }
    }

    void dec(string key) {
        //如果存在这个桶
        //默认一定存在
        if (!umap.count(key)) {
            throw out_of_range("out_of_range");
        }
        int curCount = umap[key]->count;
        auto curBucket = umap[key];
        //判断当前是否位1的桶
        if (curBucket->count == 1) {
            curBucket->set.erase(key);
            umap.erase(key);
        }else{
            //判断上一个桶是否存在，不存在就创建
            if (curBucket->prev->count == curCount - 1){
            }else{
                insertRight(curBucket->prev, new Bucket(curCount - 1));
            }
            curBucket->prev->set.insert(key);
            curBucket->set.erase(key);
            umap[key]=curBucket->prev;
        }
        //如果当前为空，应当清除
        if (curBucket->set.empty()) {
            remove(curBucket);
        }
    }

    string getMaxKey() {
        if (head->next == tail) {
            return "";
        }
        return *tail->prev->set.begin();
    }

    string getMinKey() {
        if (head->next == tail) {
            return "";
        }
        return *head->next->set.begin();
    }

    ~AllOne() {
        auto node = head;
        //清理内存
        while (node) {
            auto temp = node;
            node = node->next;
            delete temp;
        }
    }
};


int main() {
    AllOne allOne;
    allOne.inc("hello");
    allOne.inc("hello");
    cout << allOne.getMaxKey() << endl; // 返回 "hello"
    cout << allOne.getMinKey() << endl; // 返回 "hello"
    allOne.inc("leet");
    cout << allOne.getMaxKey() << endl; // 返回 "hello"
    cout << allOne.getMinKey() << endl; // 返回 "leet"
    return 0;
}