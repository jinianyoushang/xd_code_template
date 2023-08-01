//计算二项式系数c（n，k） n里面挑k个
size_t binomialCoeff(int n, int k) {
    // Create a 2D array to store the results of subproblems
    size_t C[n + 1][k + 1];

    // Fill the array in a bottom-up manner
    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= min(i, k); j++) {
            // Base cases
            if (j == 0 || j == i) {
                C[i][j] = 1;
            }
                // Recurrence relation
            else {
                C[i][j] = C[i - 1][j - 1] + C[i - 1][j];
            }
        }
    }

    // Return the desired result
    return C[n][k];
}


// 计算组合数
int combination(int n, int k) {
    // 处理边界情况
    if (k == 0 || k == n) {
        return 1;
    }

    // 计算阶乘
    int numerator = 1;
    int denominator = 1;
    for (int i = 0; i < k; i++) {
        numerator *= (n - i);
        denominator *= (i + 1);
    }

    // 计算组合数
    int result = numerator / denominator;
    return result;
}

//方法一：直接根据公式
int C(int n, int m) {
  int a = 1, b = 1, c = 1;
  for (int i = n; i >= 1; --i) a *= i; //计算n!
  for (int i = m; i >= 1; --i) b *= i; //计算m!
  for (int i = n-m; i >= 1; --i) c *= i; //计算(n-m)!
  return a/(b*c); //返回结果
}

//方法二：化简公式
int C(int n, int m) {
  int a = 1, b = 1;
  for (int i = n; i > n - m; --i) a *= i; //计算n(n-1)(n-2)...(n-m+1)
  for (int i = m; i >= 1; i--) b *= i; //计算m!
  return a/b; //返回结果
}

//方法三：递推公式
int C(int n, int m) {
  if (n == m || m == 0) return 1; //递归出口
  return C(n-1, m) + C(n-1, m-1); //递归调用
}

//方法四：动态规划
int C(int n, int m) {
  vector<vector<int>> dp(n+1, vector<int>(m+1)); //创建二维数组存储中间结果
  for (int i = 0; i <= n; ++i) {
    for (int j = 0; j <= min(i, m); ++j) {
      if (j == 0 || j == i) dp[i][j] = 1; //边界条件
      else dp[i][j] = dp[i-1][j] + dp[i-1][j-1]; //状态转移方程
    }
  }
  return dp[n][m]; //返回结果
}