#include <iostream>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

//方法1 没有预处理  单哈希法
const int M = 1e9 + 7;//最大值，用来取模，也可以使用自然溢出方法
const int B = 233;//base值

typedef long long ll;
ll get_hash(const string &s) {
    ll res = 0;
    for (char i : s) {
        res = ((ll) res * B + (ll)i) % M;
    }
    return res;
}

bool cmp(const string &s, const string &t) {
    return get_hash(s) == get_hash(t);
}


int main() {
    cout<<get_hash("abc")<<endl;
    return 0;
}
