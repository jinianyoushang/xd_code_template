#include<cstdio>
#include<queue>
#include <utility>
#include<vector>
#include<algorithm>
#include<string>
#include<map>
#include<unordered_map>
#include<iostream>
#include <sstream>
#include "main.h"

using namespace std;


//官方题解 只能计算+-
class Solution {
public:
    int calculate(string s) {
        stack<int> ops;
        ops.push(1);
        int sign = 1;

        int ret = 0;
        int n = s.length();
        int i = 0;
        while (i < n) {
            if (s[i] == ' ') {
                i++;
            } else if (s[i] == '+') {
                sign = ops.top();
                i++;
            } else if (s[i] == '-') {
                sign = -ops.top();
                i++;
            } else if (s[i] == '(') {
                ops.push(sign);
                i++;
            } else if (s[i] == ')') {
                ops.pop();
                i++;
            } else {
                long num = 0;
                while (i < n && s[i] >= '0' && s[i] <= '9') {
                    num = num * 10 + s[i] - '0';
                    i++;
                }
                ret += sign * num;
            }
        }
        return ret;
    }
};


/*每次扫描到的 数字 直接压入 a 栈
每次扫描到的符号 x 需要与 b 栈顶部的符号 y 比较优先级，如果 x > y，则 x 压入 b 栈，
 如果 x <= y，将 b 栈顶元素弹出后压入 a，继续比较 x 与 下一个 y。
最终获取的表达式为：2 5 3 - 4 * * 16 2 / -，需要将 a 中的元素弹出后再逆序。*/

class Solution2 {
public:
//    将中缀表达式转换为后缀表达式
//
//            创建两个栈 s1、s2，s1 存储临时字符，s2 存储转换过程。(s2 使用 list 代替栈)
//    遍历后缀表达式的 list
//    遍历到数字，直接添加到 s2 中。
//    遍历到符号 + - * / 时，需要与 s1 栈顶符号比较优先级。
//    如果比 s1 栈顶符号优先级高，则直接压入 s1。
//    反之，将 s1 栈顶元素弹出并添加到 s2 中，继续与 s1 的次顶元素比较，直至 s1 为空或ls中遍历到的元素优先级高于 s1 栈顶符号。
//    遍历到 (，直接压入 s1 中。
//    遍历到 )，弹出 s1 中的符号，直至遇到 (，最后将 ( 弹出，ls 继续向后遍历，这样就删除了一对括号。
//    ls 遍历完以后，将 s1 中剩余的符号依次弹出并添加到 s2 中。
//    最终将转换结果 s2 返回即可。
// 没有错误处理
//负号判断 如果-号前面不是) 或者 数字，说明-号是数的整体的部分。
    vector<string> infixConvertToPostfix(string &str) {
        stack<char> s1;
        vector<string> s2;//保存结果
        unordered_map<char, int> priority{{'*', 2},
                                          {'/', 2},
                                          {'+', 1},
                                          {'-', 1},
                                          {'(', 0}};
        bool hasMinus = false;
        for (int i = 0; i < str.size(); ++i) {
            //说明这个是负号
            if (str[i] == '-' && (i == 0 || (i > 0 && priority.count(str[i - 1])))) {
                hasMinus = true;
                i++;
            }
            if (str[i] >= '0' && str[i] <= '9') { //如果是数字
//                找到这个数字
                int j = i + 1;
                for (; j < str.size(); ++j) {
                    if (str[j] >= '0' && str[j] <= '9') {
                    } else {
                        break;
                    }
                }
                string temp = str.substr(i, j - i);
                if (hasMinus) {
                    temp = "-" + temp;
                }
                s2.push_back(temp);
                i = j - 1;
            } else if (str[i] == '(') {  // 遍历到 (，直接压入 s1
                s1.push(str[i]);
            } else if (str[i] == ')') {   // 遍历到 ) ，弹出 s1 中的符号添加到 s2 中，直至遇到 (
                while (s1.top() != '(') {
                    s2.emplace_back(1, s1.top());
                    s1.pop();
                }
                s1.pop(); // 当 ( 弹出，消除小括号
            } else { // 遍历到 + - * /
                // s1 不为空，且当遍历到的符号，小于等于栈顶符号优先级，需要弹栈操作
                // 直到当前符号优先级大于 s1 栈顶元素或 s1 弹空时，结束
                while (!s1.empty() && priority[str[i]] <= priority[s1.top()]) {
                    s2.emplace_back(1, s1.top());
                    s1.pop();
                }
                // 比较结束后，将当前字符压入 s1 中
                s1.push(str[i]);
            }
            hasMinus = false;
        }
        // 将 s1 中剩余符号添加到 s2 中
        while (!s1.empty()) {
            s2.emplace_back(1, s1.top());
            s1.pop();
        }
        return s2;
    }

    //逆波兰表达式求值
    int solvePostfix(vector<string> &tokens) {
        unordered_set<string> unorderedSet{"+", "-", "*", "/"};
        stack<int> s;
        for (const auto &item: tokens) {
            if (unorderedSet.count(item)) {
                int a = s.top();
                s.pop();
                int b = s.top();
                s.pop();
                if (item == "+") {
                    s.push(a + b);
                } else if (item == "-") {
                    s.push(b - a);
                } else if (item == "*") {
                    s.push(b * a);
                } else {
                    s.push(b / a);
                }

            } else {
                s.push(stoi(item));
            }
        }
        return s.top();
    }

    //支持负号
    int calculate(string s) {
        //判断字符串合法性
        //去掉空格
        string ts;
        for (const auto &item: s) {
            if (item != ' ') {
                ts.push_back(item);
            }
        }

        //如果最开始是负号直接在负号前添加0
        if (ts[0] == '-') {
            ts = "0" + ts;
        }

        string ts2;
        // (- 这种情况处理
        for (int i = 0; i < ts.size(); ++i) {
            if (i < ts.size() - 1 && ts[i] == '(' && ts[i + 1] == '-') {
                ts2.push_back(ts[i]);
                ts2.push_back('0');
            } else {
                ts2.push_back(ts[i]);
            }
        }
        auto temp = infixConvertToPostfix(ts2);
        return solvePostfix(temp);
    }
};

int main() {
    Solution solution;
//    string s = "2*((5-3)*4)-16/2";
//    auto temp = solution.infixConvertToPostfix(s);// 2 5 3 - 4 * * 16 2 / -
//    for (const auto &item: temp) {
//        cout << item << " ";
//    }
//    cout << endl;
//
//    auto resVal = solution.solvePostfix(temp);
//    cout << resVal << endl; //8

    string s1 = "1-(     -2)";
    cout << solution.calculate(s1) << endl;//3

    string s2 = "- (3 + (4 + 5))";
    cout << solution.calculate(s2) << endl;//-12

    string s3 = "- (3 - (- (4 + 5) ) )";
    cout << solution.calculate(s3) << endl;//-12


    return 0;
}


