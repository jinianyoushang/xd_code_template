#include <iostream>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

//拓扑排序1
bool canFinish(vector<vector<int>> &prerequisites) {
    if (prerequisites.empty()) return false;
    unordered_map<int, int> inDegree; // 记录每个节点的入度
    unordered_map<int, vector<int>> adjacencyList; // 邻接表表示的有向图

    // 构建邻接表和入度数组
    for (const auto &edge: prerequisites) {
        int course = edge[1];
        int prerequisite = edge[0];
        adjacencyList[prerequisite].push_back(course);
        inDegree[course]++;
    }
    for (const auto &edge: prerequisites) {
        if (!inDegree.count(edge[0])) {
            inDegree[edge[0]] = 0;
        }
        if (!inDegree.count(edge[1])) {
            inDegree[edge[1]] = 0;
        }
    }

    queue<int> q;

    // 将入度为0的节点入队
    for (const auto &item: inDegree) {
        if (item.second == 0) {
            q.push(item.first);
        }
    }
    int count = 0; // 记录已经学习的课程数量

    // 拓扑排序
    while (!q.empty()) {
        int course = q.front();
        q.pop();
        count++;

        // 遍历当前课程的后继课程
        for (int successor: adjacencyList[course]) {
            inDegree[successor]--;
            if (inDegree[successor] == 0) {
                q.push(successor);
            }
        }
    }

    return count == inDegree.size(); // 如果已经学习的课程数量等于总课程数量，则可以完成拓扑排序
}

//广度优先  拓扑排序2
class Solution {
public:
    vector<int> outdegree;//代表依赖当前节点的个数
    vector<vector<int>> edge;

    bool canFinish(int numCourses, vector<vector<int>> &prerequisites) {
        outdegree.resize(numCourses, 0);
        edge.resize(numCourses);
        for (const auto &item: prerequisites) {
            outdegree[item[0]]++;
            edge[item[1]].push_back(item[0]);
        }

        //开始遍历
        //使用队列
        queue<int> q;
        //先找到没有依赖的节点
        for (int i = 0; i < numCourses; ++i) {
            if (outdegree[i] == 0) {
                q.push(i);
            }
        }
        while (!q.empty()) {
            auto index = q.front();
            q.pop();

            //对于每个依赖的节点
            for (const auto &item: edge[index]) {
                outdegree[item]--;
                if (outdegree[item] == 0) {
                    q.push(item);
                }
            }
        }

        //遍历完应该每个出度都是0
        for (const auto &item: outdegree) {
            if (item > 0) {
                return false;
            }
        }
        return true;
    }
};

int main() {
    vector<vector<int>> v = {{0, 1},
                             {1, 2}};
    //拓扑排序
    if (canFinish(v)) {
        cout << "Yes" << endl;
    } else {
        cout << "No" << endl;
    }
    return 0;
}

// {{0,1},{1,0},{2,3},{2,0},{5,6}}
// {{0,1},{1,2}}