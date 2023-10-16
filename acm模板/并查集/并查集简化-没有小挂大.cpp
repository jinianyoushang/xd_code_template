#include <iostream>
#include <vector>

using namespace std;

class UnionFind {
private:
    std::vector<int> parent;
public:
    UnionFind(int n) {
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
        parent[find(x)] = find(y);
    }

    bool isSameSet(int x, int y) {
        return find(x) == find(y);
    }
};

int main() {

    int n = 10;
    UnionFind uf(n);

    uf.unite(1, 2);
    uf.unite(2, 3);
    uf.unite(4, 5);
    uf.unite(6, 7);
    uf.unite(8, 9);

    std::cout << "Are 1 and 3 connected? " << (uf.isSameSet(1, 3) ? "Yes" : "No") << std::endl;//yes
    std::cout << "Are 4 and 7 connected? " << (uf.isSameSet(4, 7) ? "Yes" : "No") << std::endl;//no
    std::cout << "Are 5 and 9 connected? " << (uf.isSameSet(5, 9) ? "Yes" : "No") << std::endl;
    //no

    return 0;
}