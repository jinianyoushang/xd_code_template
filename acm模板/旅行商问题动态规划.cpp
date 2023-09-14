//#include "main.h"

#include <iostream>
#include <bits/stdc++.h>
#include <iostream>
#include <cmath>
#include <cstring>
#include <vector>

using namespace std;

#define N 5
const int INF = 10e9;

static const int M = 1 << (N - 1);
//存储城市之间的距离
int g[N][N] = {{0,   3,  INF, 8,  9},
               {3,   0,  3,   10, 5},
               {INF, 3,  0,   4,  3},
               {8,   10, 4,   0,  20},
               {9,   5,  3,   20, 0}};
//保存顶点i到状态s最后回到起始点的最小距离
int dp[N][M];
//保存路径
vector<int> path;

//核心函数，求出动态规划dp数组
void TSP() {
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
bool isVisited(bool visited[]) {
    for (int i = 1; i < N; i++) {
        if (visited[i] == false) {
            return false;
        }
    }
    return true;
}

//获取最优路径，保存在path中,根据动态规划公式反向找出最短路径结点
void getPath() {
    //标记访问数组
    bool visited[N] = {false};
    //前驱节点编号
    int pioneer = 0, min = INF, S = M - 1, temp;
    //把起点结点编号加入容器
    path.push_back(0);

    while (!isVisited(visited)) {
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
void printPath() {
    cout << "最小路径为：";
    vector<int>::iterator it = path.begin();
    for (it; it != path.end(); it++) {
        cout << *it << "--->";
    }
    //单独输出起点编号
    cout << 0;
}

int main() {
    TSP();
    cout << "最小值为：" << dp[0][M - 1] << endl;
    getPath();
    printPath();
    return 0;
}