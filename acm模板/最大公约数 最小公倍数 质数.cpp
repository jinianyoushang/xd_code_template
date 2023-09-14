
#include <iostream>
#include <cmath>

using namespace std;

//判断一个数是否是质数
bool isPrime(int num) {
    if (num <= 1) {
        return false;
    }

    int sqrtNum = std::sqrt(num);
    for (int i = 2; i <= sqrtNum; i++) {
        if (num % i == 0) {
            return false;
        }
    }

    return true;
}

//返回最大公约数
int gcd(int a, int b) {
    if (b == 0) {
        return a;
    }
    return gcd(b, a % b);
}

//返回最小公倍数
int lcm(int a, int b) {
    int gcdValue = gcd(a, b);
    return (a * b) / gcdValue;
}


int main() {
    int num;
    std::cout << "Enter a number: ";
    std::cin >> num;

    if (isPrime(num)) {
        std::cout << num << " is a prime number." << std::endl;
    } else {
        std::cout << num << " is not a prime number." << std::endl;
    }

    cout<<gcd(6,4)<<endl;
    cout<<lcm(6,4)<<endl;


    return 0;
}