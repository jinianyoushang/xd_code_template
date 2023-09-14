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
        for (int i = 0; i < vertices; i++) {
            adjMatrix[i][i] = 0; // distance from vertex to itself is always 0
        }
    }

    void addEdge(int u, int v, int w) {
        adjMatrix[u][v] = w;
    }

    vector<vector<int>> floydWarshall() {
        vector<vector<int>> dist = adjMatrix;

        for (int k = 0; k < vertices; k++) {
            for (int i = 0; i < vertices; i++) {
                for (int j = 0; j < vertices; j++) {
                    if (dist[i][k] == INT_MAX || dist[k][j] == INT_MAX) continue;
                    if (dist[i][k] + dist[k][j] < dist[i][j]) {
                        dist[i][j] = dist[i][k] + dist[k][j];
                    }
                }
            }
        }

        // Print the shortest distance matrix
        for (int i = 0; i < vertices; i++) {
            for (int j = 0; j < vertices; j++) {
                if (dist[i][j] == INT_MAX) {
                    cout << "INF ";
                } else {
                    cout << dist[i][j] << " ";
                }
            }
            cout << endl;
        }
        return dist;
    }
};

int main() {
    Graph g(4);
    g.addEdge(0, 1, 3);
    g.addEdge(0, 3, 7);
    g.addEdge(1, 0, 3);
    g.addEdge(1, 2, 8);
    g.addEdge(2, 1, 8);
    g.addEdge(2, 3, 2);
    g.addEdge(3, 0, 7);
    g.addEdge(3, 2, 2);

    cout << "Shortest distance matrix using Floyd-Warshall:\n";
    g.floydWarshall();

    return 0;
}
