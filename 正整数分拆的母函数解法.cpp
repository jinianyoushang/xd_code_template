//#include "main.h"
#include "bits/stdc++.h"
#include <iostream>
#include <vector>

using namespace std;

int mod = 1e9 + 7;


//母函数，等价于正整数无序号分拆
//结果是 对n 的k分拆结果是C(n-1,k-1)
// n 有多少种组合方式 每个数大于等于0
int main() {

    int n = 3;//数的个数
    int sum = 10;//和
    vector<vector<int>> dp(n + 1, vector<int>(sum + 1, 0));
    //初始化
    for (int i = 1; i <= sum; ++i) {
        //如果有限制就在这里写限制
        dp[1][i] = 1;
    }

    for (int i = 2; i <= n; ++i) {
        for (int j = i; j <= sum; ++j) {
            for (int k = 1; k <= sum; ++k) {
                if (j - k >= 1) {
                    //如果有限制就在这里写限制
                    dp[i][j] = (dp[i][j] + dp[i - 1][j - k]) % mod;
                } else {
                    break;
                }
            }
        }
    }
    cout << dp[n][sum] << endl;
    return 0;
}



//有限制的情况
/*
 *
 //#include "main.h"
#include "bits/stdc++.h"
#include <iostream>
#include <vector>

using namespace std;

int mod = 1e9 + 7;


//数不能等于 nums[i]的情况

int main() {
    int n;
    cin >> n;
    vector<int> nums(n);
    int sum = 0;
    for (int i = 0; i < n; ++i) {
        cin >> nums[i];
        sum += nums[i];
    }
    //dp[i][j]表示前i个数组成和为j的个数。
    // j最大为sum
    //i为n
    vector<vector<int>> dp(n + 1, vector<int>(sum + 1, 0));
    //初始化
    for (int i = 1; i <= sum; ++i) {
        if (i != nums[0]) {
            dp[1][i] = 1;
        }
    }

    for (int i = 2; i <= n; ++i) {
        for (int j = i; j <= sum; ++j) {
            for (int k = 1; k <= sum; ++k) {
                if (j - k >= 1) {
                    if (k != nums[i-1]) {
                        dp[i][j] = (dp[i][j] + dp[i - 1][j - k]) % mod;
                    }
                } else {
                    break;
                }
            }
        }
    }
    cout << dp[n][sum] << endl;
    return 0;
}
 */