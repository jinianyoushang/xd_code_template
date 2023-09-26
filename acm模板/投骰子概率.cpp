#include <fstream>
#include <iostream>
#include <bits/stdc++.h>

using namespace std;

//掷骰子 t  投出大于等于n的概率
double probability(int t, int n) {
    vector<vector<double>> dp(t + 1, vector<double>(6 * t + 1, 0));
    dp[0][0] = 1;

    for (int i = 1; i <= t; i++) {
        for (int j = 1; j <= 6 * t; j++) {
            for (int k = 1; k <= 6; k++) {
                if (j - k >= 0) {
                    dp[i][j] += dp[i - 1][j - k] / 6.0;
                }
            }
        }
    }

    double totalWays = 0;
    for (int i = n; i <= 6 * t; i++) {
        totalWays += dp[t][i];
    }

    return (double) totalWays;
}

//掷骰子 t  投出大于等于n的概率
double probability2(int t, int n) {
    vector<vector<long long>> dp(t + 1, vector<long long>(6 * t + 1, 0));
    dp[0][0] = 1;

    for (int i = 1; i <= t; i++) {
        for (int j = 1; j <= 6 * t; j++) {
            for (int k = 1; k <= 6; k++) {
                if (j - k >= 0) {
                    dp[i][j] += dp[i - 1][j - k];
                }
            }
        }
    }

    long long totalWays = 0;
    for (int i = n; i <= 6 * t; i++) {
        totalWays += dp[t][i];
    }

    return (double) totalWays / pow(6, t);
}

int main() {
    int n, h;
    cin >> n >> h;
    int coins = 0;
    for (int i = 0; i < n; ++i) {
        int t, x;
        cin >> t >> x;
        if (t == 1) {
            coins += x;
        } else {
            h -= x;
        }
    }
    if (h <= 0) {
        cout << 1 << endl;
        return 0;
    }

    if (coins == 0) {
        cout << 0 << endl;
        return 0;
    }
    double res = probability(coins, h);
//    cout << res << endl;
    printf("%lf", res);
    return 0;
}
/**
2 5
 1 1
 2 1

0.5

 3 1145
 1 4
 1 9
 1 9

 0

 *
 *
*/