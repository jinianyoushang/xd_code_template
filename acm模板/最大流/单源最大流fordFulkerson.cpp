#include <iostream>
#include <vector>
#include <queue>
#include <climits>

//计算单源最大流，多源时可以使用一个虚拟节点链接所有点，即可
class MaxFlow {
private:
    int V;                              // 图有多少个边
    std::vector<std::vector<int>> rG;   // 图
    std::vector<int> parent;            // 保存广度优先遍历路径

    bool bfs(int s, int t) {
        std::vector<bool> visited(V, false);
        std::queue<int> q;
        q.push(s);
        visited[s] = true;
        parent[s] = -1;

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            for (int v = 0; v < V; v++) {
                if (!visited[v] && rG[u][v] > 0) {
                    q.push(v);
                    parent[v] = u;
                    visited[v] = true;
                }
            }
        }

        return visited[t];
    }

public:
    MaxFlow(int V) : V(V), rG(V, std::vector<int>(V)), parent(V) {}

    void addEdge(int u, int v, int w) {
        rG[u][v] = w;
    }

    int fordFulkerson(int s, int t) {
        int max_flow = 0;

        while (bfs(s, t)) {
            int path_flow = INT_MAX;
            for (int v = t; v != s; v = parent[v]) {
                int u = parent[v];
                path_flow = std::min(path_flow, rG[u][v]);
            }

            for (int v = t; v != s; v = parent[v]) {
                int u = parent[v];
                rG[u][v] -= path_flow;
                rG[v][u] += path_flow;
            }

            max_flow += path_flow;
        }

        return max_flow;
    }
};

int main() {
    MaxFlow g(4);
    g.addEdge(0, 1, 3);
    g.addEdge(0, 2, 2);
    g.addEdge(1, 2, 5);
    g.addEdge(1, 3, 2);
    g.addEdge(2, 3, 3);

    std::cout << "The maximum flow is " << g.fordFulkerson(0, 3) << std::endl;

    return 0;
}
