
#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

std::string subtract(std::string num1, std::string num2);

std::string add(const std::string &num1, const std::string &num2);

// Helper function to check if string a is greater or equal to b
bool isGreaterOrEqual(const std::string &a, const std::string &b) {
    if (a.length() != b.length())
        return a.length() > b.length();
    for (int i = 0; i < a.length(); i++) {
        if (a[i] != b[i]) return a[i] > b[i];
    }
    return true;
}

std::string subtract(std::string num1, std::string num2) {
    if (num2[0] == '-')
        return add(num1, num2.substr(1));
    if (num1[0] == '-')
        return "-" + add(num1.substr(1), num2);
    if (!isGreaterOrEqual(num1, num2))
        return "-" + subtract(num2, num1);

    int borrow = 0, i = num1.size() - 1, j = num2.size() - 1;
    std::string result;

    while (i >= 0) {
        int diff = (num1[i] - '0') - (j >= 0 ? num2[j] - '0' : 0) - borrow;
        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        result.push_back(diff + '0');
        i--;
        j--;
    }
    while (result.size() > 1 && result.back() == '0') result.pop_back();
    std::reverse(result.begin(), result.end());
    return result;
}

std::string add(const std::string &num1, const std::string &num2) {
    if (num1[0] == '-' && num2[0] == '-')
        return "-" + add(num1.substr(1), num2.substr(1));
    if (num1[0] == '-')
        return subtract(num2, num1.substr(1));
    if (num2[0] == '-')
        return subtract(num1, num2.substr(1));

    int carry = 0, i = num1.size() - 1, j = num2.size() - 1;
    std::string result;

    while (i >= 0 || j >= 0 || carry) {
        int sum = carry;
        if (i >= 0) sum += num1[i--] - '0';
        if (j >= 0) sum += num2[j--] - '0';
        carry = sum / 10;
        result.push_back(sum % 10 + '0');
    }
    std::reverse(result.begin(), result.end());
    return result;
}


std::string multiply(const std::string &num1, const std::string &num2) {
    bool negative = (num1[0] == '-') ^ (num2[0] == '-');
    std::string n1 = (num1[0] == '-') ? num1.substr(1) : num1;
    std::string n2 = (num2[0] == '-') ? num2.substr(1) : num2;

    int n = n1.size(), m = n2.size();
    std::string result(n + m, '0');

    for (int i = n - 1; i >= 0; i--) {
        int carry = 0;
        for (int j = m - 1; j >= 0; j--) {
            int temp = (result[i + j + 1] - '0') + (n1[i] - '0') * (n2[j] - '0') + carry;
            result[i + j + 1] = temp % 10 + '0';
            carry = temp / 10;
        }
        result[i] += carry;
    }

    while (result.size() > 1 && result[0] == '0') result.erase(result.begin());
    if (negative && result != "0") result.insert(result.begin(), '-');
    return result;
}

int main() {
    std::string num1, num2;

    std::cin >> num1 >> num2;

    std::cout << "Addition: " << add(num1, num2) << std::endl;
    std::cout << "Subtraction: " << subtract(num1, num2) << std::endl;
    cout << multiply(num1, num2) << endl;
    return 0;
}


/*
 *
15869523698 854789632596
13565104331286935260008
870659156294
 *
 *
 */
