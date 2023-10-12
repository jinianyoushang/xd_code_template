#include <iostream>
#include <vector>
#include <string>

using namespace std;

// 大数取余
int largeMod(const string& dividend, int divisor) {
    int mod = 0;
    for (char ch : dividend) {
        mod = (mod * 10 + (ch - '0')) % divisor;
    }
    return mod;
}

// 大数除法
string largeDivide(const string& dividend, int divisor) {
    string quotient;
    int idx = 0;
    int temp = dividend[idx] - '0';
    while (temp < divisor) {
        temp = temp * 10 + (dividend[++idx] - '0');
    }
    while (dividend.size() > idx) {
        quotient += std::to_string((temp / divisor) + '0');
        temp = (temp % divisor) * 10 + (dividend[++idx] - '0');
    }
    if (quotient.empty()) {
        quotient = "0";
    }
    return quotient;
}

int main() {
    string dividend;
    int divisor;

    // 输入被除数和除数
    cout << "Enter dividend: ";
    cin >> dividend;
    cout << "Enter divisor: ";
    cin >> divisor;

    // 大数取余
    int mod = largeMod(dividend, divisor);
    cout << "Modulus: " << mod << endl;

    // 大数除法
    string quotient = largeDivide(dividend, divisor);
    cout << "Quotient: " << quotient << endl;

    return 0;
}