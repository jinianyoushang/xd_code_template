#include <iostream>
#include <set>

class MedianFinder {
    std::multiset<int> left;  // 存放小于中位数的数，最大的数在最右边
    std::multiset<int> right; // 存放大于中位数的数，最小的数在最左边
    //left的大小 等于right或者等一right.size()+1
public:
    // 添加一个数字
    void addNum(int num) {
        if (left.empty() || num <= *left.rbegin()) {
            left.insert(num);
        } else {
            right.insert(num);
        }

        // 保持两棵树的平衡
        while (left.size() > right.size() + 1) {
            right.insert(*left.rbegin());
            left.erase(left.find(*left.rbegin()));
        }
        while (right.size() > left.size()) {
            left.insert(*right.begin());
            right.erase(right.find(*right.begin()));
        }
    }

    // 删除一个数字
    void removeNum(int num) {
        if (left.find(num) != left.end()) {
            left.erase(left.find(num));
        } else {
            right.erase(right.find(num));
        }

        // 保持两棵树的平衡
        while (left.size() > right.size() + 1) {
            right.insert(*left.rbegin());
            left.erase(left.find(*left.rbegin()));
        }
        while (right.size() > left.size()) {
            left.insert(*right.begin());
            right.erase(right.find(*right.begin()));
        }
    }

    // 返回中位数
    double findMedian() {
        if (left.size() == right.size()) {
            return (*left.rbegin() + *right.begin()) * 0.5;
        }
        return *left.rbegin();
    }
};

int main() {
    MedianFinder mf;
    mf.addNum(1);
    mf.addNum(2);
    std::cout << mf.findMedian() << std::endl;  // 1.5
    mf.addNum(3);
    std::cout << mf.findMedian() << std::endl;  // 2
    mf.removeNum(2);
    std::cout << mf.findMedian() << std::endl;  // 2
    return 0;
}
