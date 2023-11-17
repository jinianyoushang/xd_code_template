#define _GLIBCXX_DEBUG

#include <iostream>
#include <random>
#include <functional>
#include <thread>
#include <atomic>
#include "thread_pool.h"

using namespace std;

std::random_device rd; // 真实随机数产生器
std::mt19937 mt(rd()); //生成计算随机数mt
std::uniform_int_distribution<int> dist(-1000, 1000); //生成-1000到1000之间的离散均匀分布数
auto rnd = std::bind(dist, mt);

// 设置线程睡眠时间
void simulate_hard_computation() {
    std::this_thread::sleep_for(std::chrono::milliseconds(200 + rnd()));
}

// 添加两个数字的简单函数并打印结果
void multiply(const int a, const int b) {
    simulate_hard_computation();
    const int res = a * b;
    std::cout << a << " * " << b << " = " << res << std::endl;
}

// 添加并输出结果
void multiply_output(int &out, const int a, const int b) {
    simulate_hard_computation();
    out = a * b;
    std::cout << a << " * " << b << " = " << out << std::endl;
}

// 结果返回
int multiply_return(const int a, const int b) {
    simulate_hard_computation();
    const int res = a * b;
    std::cout << a << " * " << b << " = " << res << std::endl;
    return res;
}

//定义类
class Task {
    atomic_int m_int = 1;
public:
    void process() {
        this_thread::sleep_for(chrono::seconds(1));
        cout << m_int++ << endl;
    }
};

void example() {
    // 创建3个线程的线程池
    ThreadPool pool(6);

//    // 提交乘法操作，总共30个
//    for (int i = 1; i <= 3; ++i)
//        for (int j = 1; j <= 10; ++j) {
//            pool.submit(multiply, i, j);
//        }
//
//    // 使用ref传递的输出参数提交函数
//    int output_ref;
//    auto future1 = pool.submit(multiply_output, std::ref(output_ref), 5, 6);
//
//    // 等待乘法输出完成
//    future1.get();
//    std::cout << "Last operation result is equals to " << output_ref << std::endl;
//
//    // 使用return参数提交函数
//    auto future2 = pool.submit(multiply_return, 5, 3);
//
//    // 等待乘法输出完成
//    int res = future2.get();
//    std::cout << "Last operation result is equals to " << res << std::endl;
//
//
//    //自定义个方法
//    auto func3 = [](auto &a, auto &b) {
//        return a * b;
//    };
//    auto future3 = pool.submit(func3, 2, 10);
//    cout << "Last operation result is equals to " << future3.get() << endl;

    //自定义类的方法
    Task task1;
    auto future4 = pool.submit([&task1]() {
        task1.process();
    });
    future4.get();//也可以不等待
}

int main() {
    example();

    return 0;
}
