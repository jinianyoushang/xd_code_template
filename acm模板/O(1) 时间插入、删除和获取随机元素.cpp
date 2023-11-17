#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <random>

using namespace std;

//O(1) 时间插入、删除和获取随机元素
class RandomizedSet {
    unordered_map<int, int> umap;//用来保存索引 key：数据值 val：数字对应索引
    vector<int> arr;
    std::random_device rd;
    std::mt19937 gen;
public:
    RandomizedSet() : gen(rd()) {
    }

    bool insert(int val) {
        if (umap.count(val)) {
            return false;
        }

        umap[val] = arr.size();
        arr.push_back(val);
        return true;
    }

    bool remove(int val) {
        if (!umap.count(val)) return false;

        int valIndex = umap[val];
        int endVal = arr[arr.size() - 1];
        umap[endVal] = valIndex;
        arr[valIndex] = endVal;
        arr.pop_back();
        umap.erase(val);
        return true;
    }

    int getRandom() {
        std::uniform_int_distribution<> numDis(0, arr.size() - 1);
        int randomInt = numDis(gen);
        return arr[randomInt];
    }
};

// 插入、删除和获取随机元素O(1)时间且允许有重复数字的结构
// 测试链接 :
// https://leetcode.cn/problems/insert-delete-getrandom-o1-duplicates-allowed/
class RandomizedCollection {
    unordered_map<int, unordered_set<int>> umap;//用来保存索引 key：数据值 val：数字对应索引的集合
    vector<int> arr;
    std::random_device rd;
    std::mt19937 gen;
public:
    RandomizedCollection() : gen(rd()) {
    }

    bool insert(int val) {
        arr.push_back(val);
        umap[val].insert(arr.size() - 1);
        return umap[val].size() == 1;
    }

    bool remove(int val) {
        if (!umap.count(val)) {
            return false;
        }
        int valIndex = *umap[val].begin();//要删除的index
        int endVal = arr[arr.size() - 1];//最后的结果
        if (val == endVal) {
            umap[val].erase(arr.size() - 1);
        } else {
            arr[valIndex] = endVal;
            umap[endVal].insert(valIndex);
            umap[endVal].erase(arr.size() - 1);//把最后的删除
            umap[val].erase(valIndex);
        }
        arr.pop_back();
        if(umap[val].empty()){
            umap.erase(val);
        }
        return true;
    }

    int getRandom() {
        std::uniform_int_distribution<> numDis(0, arr.size() - 1);
        int randomInt = numDis(gen);
        return arr[randomInt];
    }
};


int main() {


    return 0;
}