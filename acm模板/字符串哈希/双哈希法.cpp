#include <iostream>
#include <vector>

using namespace std;

#define ll long long   // 双Hash方法，不同的Base和MOD，相当于两次 单Hash，效率比较低
ll Base1 = 29;
ll Base2 = 131;
ll MOD1 = 1e9 + 7;
ll MOD2 = 1e9 + 9;
const int MAXN = 2e4 + 50;//字符串最大长度
ll h1[MAXN], h2[MAXN], p1[MAXN], p2[MAXN];


pair<ll, ll> get_hash(const string &text) {
    int n = text.size();
    h1[0] = 0, h2[0] = 0, p1[0] = 1, p2[0] = 1;
    for (int i = 0; i < n; i++) {
        h1[i + 1] = (h1[i] * Base1 + (text[i] - 'a' + 1)) % MOD1;
        h2[i + 1] = (h2[i] * Base2 + (text[i] - 'a' + 1)) % MOD2;
    }
    return {h1[n],h2[n]};
}

bool cmp(const string &s, const string &t) {
    return get_hash(s) == get_hash(t);
}


int main() {
    auto temp= get_hash("abc");
    cout << temp.first<<" "<< temp.second<< endl;
    cout<<cmp("test","tes")<<endl;
    cout<<cmp("test","test")<<endl;
    return 0;
}
