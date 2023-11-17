#include <iostream>


//把i上面的一堆从from移动到to上
void f(int i, const std::string &from, const std::string &to, const std::string &other) {
    if (i == 1) {
        std::cout << "移动圆盘 1 从 " << from << " 到 " << to << std::endl;
    } else {
        f(i - 1, from, other, to);
        std::cout << "移动圆盘 " << i << " 从 " << from << " 到 " << to << std::endl;
        f(i - 1, other, to, from);
    }
}

void hanoi(int n) {
    if (n > 0) {
        f(n, "左", "右", "中");
    }
}


//n重的最优是2^n-1次
int main() {
    int n = 4;
    hanoi(n);
    return 0;
}