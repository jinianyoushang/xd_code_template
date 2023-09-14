#include <iostream>
#include <vector>
#include <climits>

using namespace std;

class Graph {
    int vertices;
    vector<vector<int>> adjMatrix;

public:
    Graph(int vertices) {
        this->vertices = vertices;
        adjMatrix.resize(vertices, vector<int>(vertices, INT_MAX));
    }

    void addEdge(int u, int v, int w) {
        adjMatrix[u][v] = w;
        adjMatrix[v][u] = w;  // 假设是无向图
    }

    //遍历distance 找出最近的顶点
    int findMinVertex(vector<bool> &visited, vector<int> &distance) {
        int minVertex = -1;
        for (int i = 0; i < vertices; i++) {
            if (!visited[i] && (minVertex == -1 || distance[i] < distance[minVertex])) {
                minVertex = i;
            }
        }
        return minVertex;
    }

    vector<int> dijkstra(int src) {
        vector<bool> visited(vertices, false);
        vector<int> distance(vertices, INT_MAX);

        distance[src] = 0;

        for (int i = 0; i < vertices - 1; i++) {
            int minVertex = findMinVertex(visited, distance);
            visited[minVertex] = true;

            for (int j = 0; j < vertices; j++) {
                if (adjMatrix[minVertex][j] != INT_MAX && !visited[j] && distance[minVertex] != INT_MAX) {
                    int newDist = distance[minVertex] + adjMatrix[minVertex][j];
                    if (newDist < distance[j]) {
                        distance[j] = newDist;
                    }
                }
            }
        }

        // 打印最短路径
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
