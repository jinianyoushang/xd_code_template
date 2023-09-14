//#include "main.h"

#include <iostream>
#include <bits/stdc++.h>

using namespace std;

const int INF = 1e8;

//核心函数，求出动态规划dp数组
void TSP(vector<vector<int>> &g, int N, int M, vector<vector<int>> &dp) {
    //初始化dp[i][0]
    for (int i = 0; i < N; i++) {
        dp[i][0] = g[i][0];
    }
    //求解dp[i][j],先跟新列在更新行
    for (int j = 1; j < M; j++) {
        for (int i = 0; i < N; i++) {
            dp[i][j] = INF;
            //如果集和j(或状态j)中包含结点i,则不符合条件退出
            if (((j >> (i - 1)) & 1) == 1) {
                continue;
            }
            for (int k = 1; k < N; k++) {
                if (((j >> (k - 1)) & 1) == 0) {
                    continue;
                }
                if (dp[i][j] > g[i][k] + dp[k][j ^ (1 << (k - 1))]) {
                    dp[i][j] = g[i][k] + dp[k][j ^ (1 << (k - 1))];
                }
            }
        }
    }

}

//判断结点是否都以访问,不包括0号结点
bool isVisited(vector<bool> &visited, int N) {
    for (int i = 1; i < N; i++) {
        if (visited[i] == false) {
            return false;
        }
    }
    return true;
}

//获取最优路径，保存在path中,根据动态规划公式反向找出最短路径结点
void
getPath(int N, vector<bool> &visited, vector<int> &path, vector<vector<int>> &g, vector<vector<int>> &dp) {
    const int M = 1 << (N - 1);

    //前驱节点编号
    int pioneer = 0, min = INF, S = M - 1, temp;
    //把起点结点编号加入容器
    path.push_back(0);

    while (!isVisited(visited, N)) {
        for (int i = 1; i < N; i++) {
            if (visited[i] == false && (S & (1 << (i - 1))) != 0) {
                if (min > g[i][pioneer] + dp[i][(S ^ (1 << (i - 1)))]) {
                    min = g[i][pioneer] + dp[i][(S ^ (1 << (i - 1)))];
                    temp = i;
                }
            }
        }
        pioneer = temp;
        path.push_back(pioneer);
        visited[pioneer] = true;
        S = S ^ (1 << (pioneer - 1));
        min = INF;
    }
}

//输出路径
void printPath(vector<int> &path) {
    cout << "最小路径为：";
    vector<int>::iterator it = path.begin();
    for (it; it != path.end(); it++) {
        cout << *it << "--->";
    }
    //单独输出起点编号
    cout << 0;
}


/*
 *
4
0 2 6 5
2 0 4 4
6 4 0 2
5 4 2 0
最小值为：13
最小路径为：0--->1--->2--->3--->0
进程已结束,退出代码0
 * */
int main() {
    //输入
    int N;
    cin >> N;
    vector<vector<int>> v(N, vector<int>(N, 0));
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            int temp;
            cin >> temp;
            v[i][j] = temp;
        }
    }
    static const int M = 1 << (N - 1);
    vector<vector<int>> dp(N, vector<int>(M));
    //标记访问数组
    vector<bool> visited(N, false);
    //保存路径
    vector<int> path;

    TSP(v, N, M, dp);
    cout << "最小值为：" << dp[0][M - 1] << endl;
    getPath(N, visited, path, v, dp);
    printPath(path);
    return 0;
}