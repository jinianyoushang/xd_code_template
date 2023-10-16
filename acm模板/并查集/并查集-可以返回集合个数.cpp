#include <iostream>
#include <vector>

using namespace std;

class UnionFind {
private:
    std::vector<int> parent;
    int sets = 0;
public:
    int getCount() const{
        return sets;
    }

    explicit UnionFind(int n) {
        sets = n;
        parent.resize(n);
        for (int i = 0; i < n; i++) {
            parent[i] = i;
        }
    }

    int find(int x) {
        if (x != parent[x]) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }

    void unite(int x, int y) {
        int fx = find(x);
        int fy = find(y);
        if (fx != fy) {
            sets--;
            parent[fx] = fy;
        }
    }

    bool isSameSet(int x, int y) {
        return find(x) == find(y);
    }
};


int main() {

    return 0;
}