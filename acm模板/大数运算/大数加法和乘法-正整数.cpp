#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;


string multiply(string a, string b) {
    int n1 = a.size();
    int n2 = b.size();
    vector<int> result(n1 + n2, 0);
    for (int i = n1 - 1; i >= 0; --i) {
        for (int j = n2 - 1; j >= 0; --j) {
            result[i + j + 1] += (a[i] - '0') * (b[j] - '0');
            result[i + j] += result[i + j + 1] / 10;
            result[i + j + 1] = result[i + j + 1] % 10;
        }
    }

    string res;
    bool noZeroFound = false;
    for (const auto &item: result) {
        if (item != 0 || noZeroFound) {
            noZeroFound = true;
            res += (char) (item + '0');
        }
    }
    return res.empty() ? "0" : res;
}

string add(string a, string b) {
    int n1 = a.size();
    int n2 = b.size();
    vector<int> v(max(n1, n2) + 1, 0);
    //先把a放进去
    for (int i = 0; i < n1; ++i) {
        v[i] = *(a.rbegin() + i) - '0';
    }

    //把b放进去
    for (int i = 0; i < n2; ++i) {
        v[i] += *(b.rbegin() + i) - '0';
        v[i + 1] += v[i] / 10;
        v[i] = v[i] % 10;
    }
    std::reverse(v.begin(), v.end());
    string res;
    bool noZeroFound = false;
    for (const auto &item: v) {
        if (item != 0 || noZeroFound) {
            noZeroFound = true;
            res += (char) (item + '0');
        }
    }
    return res.empty() ? "0" : res;
}


int main() {
    string a, b;
    cin >> a >> b;
    cout << multiply(a, b) << endl;
    cout << add(a, b) << endl;
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
