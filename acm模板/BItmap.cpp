#include <iostream>
#include <vector>
#include <unordered_set>
#include <random>

class Bitset {
private:
    std::vector<int> set;

public:
    Bitset(int n) {
        set.resize((n + 31) / 32);
    }

    void add(int num) {
        set[num / 32] |= 1 << (num % 32);
    }

    void remove(int num) {
        set[num / 32] &= ~(1 << (num % 32));
    }

    void reverse(int num) {
        set[num / 32] ^= 1 << (num % 32);
    }

    bool contains(int num) {
        return ((set[num / 32] >> (num % 32)) & 1) == 1;
    }
};

// 对数器测试
int main() {
    int n = 1000;
    int testTimes = 10000;
    std::cout << "测试开始" << std::endl;

    // 实现的位图结构
    Bitset bitSet(n);

    // 直接用unordered_set做对比测试
    std::unordered_set<int> hashSet;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    std::uniform_int_distribution<> numDis(0, n - 1);

    std::cout << "调用阶段开始" << std::endl;
    for (int i = 0; i < testTimes; i++) {
        double decide = dis(gen);
        int number = numDis(gen);
        if (decide < 0.333) {
            bitSet.add(number);
            hashSet.insert(number);
        } else if (decide < 0.666) {
            bitSet.remove(number);
            hashSet.erase(number);
        } else {
            bitSet.reverse(number);
            if (hashSet.count(number)) {
                hashSet.erase(number);
            } else {
                hashSet.insert(number);
            }
        }
    }
    std::cout << "调用阶段结束" << std::endl;

    std::cout << "验证阶段开始" << std::endl;
    for (int i = 0; i < n; i++) {
        if (bitSet.contains(i) != (hashSet.count(i) != 0)) {
            std::cout << "出错了！" << std::endl;
        }
    }
    std::cout << "验证阶段结束" << std::endl;

    std::cout << "测试结束" << std::endl;

    return 0;
}