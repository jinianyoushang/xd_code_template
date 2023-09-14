#include <iostream>
#include <vector>
#include <climits>
#include <stack>

using namespace std;

class Graph {
    int vertices;
    vector<vector<int>> adjMatrix;
    vector<vector<int>> predecessor;

public:
    Graph(int vertices) {
        this->vertices = vertices;
        adjMatrix.resize(vertices, vector<int>(vertices, INT_MAX));
        predecessor.resize(vertices, vector<int>(vertices, -1));

        for (int i = 0; i < vertices; i++) {
            adjMatrix[i][i] = 0;
        }
    }

    void addEdge(int u, int v, int w) {
        adjMatrix[u][v] = w;
    }

    void floydWarshall() {
        vector<vector<int>> dist = adjMatrix;

        for (int i = 0; i < vertices; i++) {
            for (int j = 0; j < vertices; j++) {
                if (adjMatrix[i][j] != INT_MAX) {
                    predecessor[i][j] = i;
                }
            }
        }

        for (int k = 0; k < vertices; k++) {
            for (int i = 0; i < vertices; i++) {
                for (int j = 0; j < vertices; j++) {
                    if (dist[i][k] == INT_MAX || dist[k][j] == INT_MAX) continue;
                    if (dist[i][k] + dist[k][j] < dist[i][j]) {
                        dist[i][j] = dist[i][k] + dist[k][j];
                        predecessor[i][j] = predecessor[k][j];
                    }
                }
            }
        }
    }

    void printPath(int start, int end) {
        if (start == end) {
            cout << start << " ";
            return;
        } else if (predecessor[start][end] == -1) {
            cout << "No path exists between " << start << " and " << end << endl;
            return;
        }
        stack<int> path;
        int u = end;
        while (u != start) {
            path.push(u);
            u = predecessor[start][u];
        }
        path.push(start);
        while (!path.empty()) {
            cout << path.top() << " ";
            path.pop();
        }
        cout << endl;
    }
};

int main() {
    Graph g(4);
    g.addEdge(0, 1, 3);
    g.addEdge(0, 3, 7);
    g.addEdge(1, 2, 8);
    g.addEdge(2, 3, 2);

    g.floydWarshall();

    cout << "Shortest path from 0 to 3: ";
    g.printPath(0, 2);

    return 0;
}
