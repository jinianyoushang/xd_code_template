#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <random>

using namespace std;

//// 最大频率栈
// 测试链接 : https://leetcode.cn/problems/maximum-frequency-stack/
/*实现 FreqStack 类:
FreqStack() 构造一个空的堆栈。
void push(int val) 将一个整数 val 压入栈顶。O(1)
int pop() 删除并返回堆栈中出现频率最高的元素。 O(1)
如果出现频率最高的元素不只一个，则移除并返回最接近栈顶的元素。
 */
class FreqStack {
    int maxTimes = 0;//最大词频数
    vector<vector<int>> countValues;//实际数据保存的位置，对应每一层
    //词频表
    unordered_map<int, int> countMap;//key：词，val：词频

public:
    FreqStack() {
    }

    void push(int val) {
        countMap[val]++;
        maxTimes = max(maxTimes, countMap[val]);
        while (countValues.size() <= countMap[val]) {
            countValues.emplace_back();
        }
        countValues[countMap[val]].push_back(val);
    }

    int pop() {
        int maxValues = countValues[maxTimes].back();
        countValues[maxTimes].pop_back();//删除最大值
        countMap[maxValues]--; //最大值对应数减一
        //清除词频表
        if (countMap[maxValues] == 0) {
            countMap.erase(maxValues);
        }
        if (countValues[maxTimes].empty()) {
            maxTimes--;
            countValues.pop_back();//删除最顶层空的
        }

        return maxValues;
    }
};

/**
 * Your FreqStack object will be instantiated and called as such:
 * FreqStack* obj = new FreqStack();
 * obj->push(val);
 * int param_2 = obj->pop();
 */


int main() {
    FreqStack freqStack;
    freqStack.push(5);//堆栈为 [5]
    freqStack.push(7);//堆栈是 [5,7]
    freqStack.push(5);//堆栈是 [5,7,5]
    freqStack.push(7);//堆栈是 [5,7,5,7]
    freqStack.push(4);//堆栈是 [5,7,5,7,4]
    freqStack.push(5);//堆栈是 [5,7,5,7,4,5]
    cout << freqStack.pop() << endl;//返回 5 ，因为 5 出现频率最高。堆栈变成 [5,7,5,7,4]。
    cout << freqStack.pop() << endl;//返回 7 ，因为 5 和 7 出现频率最高，但7最接近顶部。堆栈变成 [5,7,5,4]。
    cout << freqStack.pop() << endl;//返回 5 ，因为 5 出现频率最高。堆栈变成 [5,7,4]。
    cout << freqStack.pop() << endl;//返回 4 ，因为 4, 5 和 7 出现频率最高，但 4 是最接近顶部的。堆栈变成 [5,7]。

    return 0;
}