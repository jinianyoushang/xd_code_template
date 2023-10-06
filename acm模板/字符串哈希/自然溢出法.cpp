#include <iostream>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

const int B = 233;//base值

typedef long long ll;
ll get_hash(const string &s) {
    ll res = 0;
    for (char i : s) {
        res = ((ll) res * B + (ll)i);
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
