//#include "main.h"

/*
    思路:
    首先将vector<int> list转为哈希表这样可以在O(1)的时间内完成查询
    然后开始使用循环迭代轮次，对于每轮次
        先创建新的哈希表保存下一轮次的结果，，
        由于每次更新都涉及存活的细胞数量，所以我们分为两部分处理，首先是存活的细胞，然后是存活细胞周围细胞的处理
        对于每个存活的细胞我们都判断其周围存活的细胞的数量，这样可以决定当前位置下一时刻细胞的死活
        对于存活细胞周围细胞的处理，我们对每个周围进行遍历，决定下一时刻该位置细胞的死活。
        将下一轮次结果作为下一次迭代开始的状态。
    最后打印输出结果
*/

//#include "bits/stdc++.h"
#include <iostream>
#include <vector>
#include <unordered_set>

using namespace std;

class Solution {
public:
    void startGame(vector<int> list, int n, int m) {
        unordered_set<int> aliveCells(list.begin(), list.end());
        vector<int> dir_x = {-1, -1, -1, 0, 0, 1, 1, 1};
        vector<int> dir_y = {-1, 0, 1, -1, 1, -1, 0, 1};

        while (m--) {
            unordered_set<int> nextAliveCells;
            //存活的细胞的处理
            for (int cell: aliveCells) {
                int x = (cell - 1) / n + 1;
                int y = (cell - 1) % n + 1;
                int liveNeighbors = 0;
                for (int k = 0; k < 8; ++k) {
                    int nx = x + dir_x[k];
                    int ny = y + dir_y[k];
                    if (nx >= 1 && nx <= n && ny >= 1 && ny <= n) {
                        int neighborCell = (nx - 1) * n + ny;
                        if (aliveCells.count(neighborCell)) {
                            liveNeighbors++;
                        }
                    }
                }
                if (liveNeighbors == 2 || liveNeighbors == 3) {
                    nextAliveCells.insert(cell);
                }
            }
            //存活的细胞周围的细胞的处理
            for (int cell: aliveCells) {
                int x = (cell - 1) / n + 1;
                int y = (cell - 1) % n + 1;
                for (int k = 0; k < 8; ++k) {
                    int nx = x + dir_x[k];
                    int ny = y + dir_y[k];
                    if (nx >= 1 && nx <= n && ny >= 1 && ny <= n) {
                        int neighborCell = (nx - 1) * n + ny;
                        if (!aliveCells.count(neighborCell)) {
                            int liveNeighbors = 0;
                            for (int kk = 0; kk < 8; ++kk) {
                                int nnx = nx + dir_x[kk];
                                int nny = ny + dir_y[kk];
                                if (nnx >= 1 && nnx <= n && nny >= 1 && nny <= n) {
                                    int nNeighborCell = (nnx - 1) * n + nny;
                                    if (aliveCells.count(nNeighborCell)) {
                                        liveNeighbors++;
                                    }
                                }
                            }
                            if (liveNeighbors == 3) {
                                nextAliveCells.insert(neighborCell);
                            }
                        }
                    }
                }
            }
            aliveCells = nextAliveCells;
        }

        for (int cell: aliveCells) {
            cout << cell << " ";
        }
        cout << endl;
    }
};

int main() {
    Solution solution;

    vector<int> list1 = {1, 2, 3, 4, 5};
    int n1 = 5, m1 = 1;
    solution.startGame(list1, n1, m1);  // Output: 2 3 4 7 8 9

    vector<int> list2 = {10, 14, 16, 21, 22};
    unordered_set<int>  s(list2.begin(),list2.end());
    int n2 = 6, m2 = 4;
    solution.startGame(list2, n2, m2);  // Output: 17 21 23 28 29

    return 0;
}