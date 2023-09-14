#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <list>

using namespace std;

class Graph {
    int vertices;
    vector<list<pair<int, int>>> adjList;  // 临接列表

public:
    Graph(int vertices) {
        this->vertices = vertices;
        adjList.resize(vertices);
    }

    void addEdge(int u, int v, int w) {
        adjList[u].emplace_back(v, w);
        adjList[v].emplace_back(u, w);  // 假设是无向图
    }

    vector<int> dijkstra(int src) {
        vector<int> distance(vertices, INT_MAX);
        vector<bool> processed(vertices, false);  // to check if vertex's shortest path is already found

        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq; // min heap

        distance[src] = 0;
        pq.emplace(0, src);

        while (!pq.empty()) {
            int u = pq.top().second;
            pq.pop();

            if (processed[u]) continue; // 如果已经访问过了
            processed[u] = true;

            for (auto &neighbour: adjList[u]) {
                int v = neighbour.first;
                int weight = neighbour.second;

                if (distance[u] + weight < distance[v]) {
                    distance[v] = distance[u] + weight;
                    pq.emplace(distance[v], v);
                }
            }
        }

        for (int i = 0; i < vertices; i++) {
            cout << i << " - " << distance[i] << endl;
        }

        return distance;
    }
};

int main() {
    int vertices = 5;
    Graph g(vertices);

    g.addEdge(0, 1, 4);
    g.addEdge(0, 2, 2);
    g.addEdge(1, 2, 1);
    g.addEdge(1, 3, 5);
    g.addEdge(2, 3, 8);
    g.addEdge(2, 4, 10);
    g.addEdge(3, 4, 2);

    auto distance = g.dijkstra(0);

    return 0;
}
