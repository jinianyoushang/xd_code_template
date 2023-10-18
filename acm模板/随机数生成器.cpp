#include <iostream>
#include <random>
#include <vector>

std::vector<int> generateRandomArray(int n) {
    std::random_device rd;  // 用于获取随机种子
    std::mt19937 gen(rd()); // 以随机种子初始化随机数引擎
    std::uniform_int_distribution<int> dis(0, 99); // 定义范围在 0 到 99 之间的均匀分布

    std::vector<int> arr(n);
    for (int i = 0; i < n; i++) {
        arr[i] = dis(gen); // 生成随机数并存储在数组中
    }

    return arr;
}

int main() {
    int n = 10; // 数组长度
    std::vector<int> randomArray = generateRandomArray(n);

    // 输出随机数组
    for (int i = 0; i < n; i++) {
        std::cout << randomArray[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}