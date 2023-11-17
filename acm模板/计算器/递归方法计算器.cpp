#include <iostream>
#include <vector>

using namespace std;

//计算器
//使用递归+2个栈来实现 ，每次遇到(这样的就递归
class Solution {
private:
    int where;

    // s[i....]开始计算，遇到字符串终止 或者 遇到)停止
    // 返回 : 自己负责的这一段，计算的结果
    // 返回之间，更新全局变量where，为了上游函数知道从哪继续！
    int f(const std::string &s, int i) {
        int cur = 0;
        std::vector<int> numbers;
        std::vector<char> ops;
        while (i < s.length() && s[i] != ')') {
            if (s[i] >= '0' && s[i] <= '9') {
                cur = cur * 10 + s[i++] - '0';
            } else if (s[i] != '(') {
                push(numbers, ops, cur, s[i++]);
                cur = 0;
            } else {
                cur = f(s, i + 1);
                i = where + 1;
            }
        }
        push(numbers, ops, cur, '+');
        where = i;
        return compute(numbers, ops);
    }


    //放到栈里面
    void push(std::vector<int> &numbers, std::vector<char> &ops, int cur, char op) {
        int n = numbers.size();
        if (n == 0 || ops.back() == '+' || ops.back() == '-') {
            numbers.push_back(cur);
            ops.push_back(op);
        } else {
            if (ops.back() == '*') {
                numbers.back() = numbers.back() * cur;
            } else {
                numbers.back() = numbers.back() / cur;
            }
            ops.back() = op;
        }
    }

    //计算结果
    int compute(const std::vector<int> &numbers, const std::vector<char> &ops) {
        int n = numbers.size();
        int ans = numbers[0];
        for (int i = 1; i < n; i++) {
            ans += ops[i - 1] == '+' ? numbers[i] : -numbers[i];
        }
        return ans;
    }

public:

    int calculate(const std::string &str) {
        where = 0;
        //去掉字符串的空格
        string temp;
        for (const auto &item: str) {
            if (item != ' ') {
                temp.push_back(item);
            }
        }
        return f(temp, 0);
    }


};


int main() {
    Solution s{};
    std::string str = "2+3*4+2*(3 -1)";
    int result = s.calculate(str);
    std::cout << "Result: " << result << std::endl; //18
    return 0;
}