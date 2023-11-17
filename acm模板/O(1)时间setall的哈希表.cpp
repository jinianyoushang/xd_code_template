#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <random>

using namespace std;


class HashSet {
    unordered_map<long long, pair<long long, int>> umap;
    int count = 0;
    int setAllTime = -1;
    long long setAllVal;

public:
    HashSet() {}

    void setAll(long long val) {
        setAllVal = val;
        setAllTime = count;
        count++;
    }

    void set(long long key, long long val) {
        umap[key] = {val, count};
        count++;
    }

    long long get(long long key) {
        if (umap.count(key)) {
            if (umap[key].second > setAllTime) {
                return umap[key].first;
            } else {
                return setAllVal;
            }
        }
        return -1;
    }
};


int main() {



    return 0;
}