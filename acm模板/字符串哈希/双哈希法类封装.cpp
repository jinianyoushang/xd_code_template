#include <iostream>
#include <vector>
#include <bits/stdc++.h>

using namespace std;



#define ll long long   // 双Hash方法，不同的Base和MOD，相当于两次 单Hash，
//字符串哈希，双哈希法 预处理缓存加速
class StringHash {
private:
    ll Base1 = 29;
    ll Base2 = 131;
    ll MOD1 = 1e9 + 7;
    ll MOD2 = 1e9 + 9;
    vector<ll> h1, h2, p1, p2;//p是系数的n次方的预处理，h是哈希
    ll len = 0;
public:
    explicit StringHash(const string &text) {
        int n = text.size();
        len = n;
        h1.resize(n + 1);
        h2.resize(n + 1);
        p1.resize(n + 1);
        p2.resize(n + 1);
        h1[0] = 0, h2[0] = 0, p1[0] = 1, p2[0] = 1;
        for (int i = 0; i < n; i++) {
            h1[i + 1] = (h1[i] * Base1 + (text[i] - 'a' + 1)) % MOD1;
            h2[i + 1] = (h2[i] * Base2 + (text[i] - 'a' + 1)) % MOD2;
        }

        for (int i = 1; i < n; i++) {
            p1[i] = (p1[i - 1] * Base1) % MOD1;
            p2[i] = (p2[i - 1] * Base2) % MOD2;
        }
    }

    pair<ll, ll> get_all_hash() {
        return {h1[len], h2[len]};
    }

    //左闭右闭
    pair<ll, ll> get_hash(int left, int right) {
        ll res1 = ((h1[right + 1] - h1[left] * p1[right - left + 1]) % MOD1 + MOD1) % MOD1;
        ll res2 = ((h2[right + 1] - h2[left] * p2[right - left + 1]) % MOD2 + MOD2) % MOD2;
        return {res1, res2};
    }

    bool operator==(StringHash &b) {
        return this->get_all_hash() == b.get_all_hash();
    }

    bool operator==(pair<ll, ll> &b) {
        return this->get_all_hash() == b;
    }
};

bool cmp(StringHash &s, StringHash &t) {
    return s.get_all_hash() == t.get_all_hash();
}


int main() {
    StringHash stringHash("abcabc");
    StringHash stringHash2("abc");
    auto hash2 = stringHash.get_all_hash();
    cout << hash2.first << " " << hash2.second << endl;    //21999780 175250841
    cout << cmp(stringHash, stringHash2) << endl;   //0
    cout << (stringHash == stringHash2) << endl;            //0
    auto hash3 = stringHash.get_hash(0, 2);
    cout << (stringHash2 == hash3) << endl;                 //1 这里验证成功
    return 0;
}
