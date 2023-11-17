#include <iostream>
#include <vector>
#include <unordered_set>
#include <random>

using namespace std;

class Bitset {
    vector<int> sets;
    int reverse;
    int _size;
    int zero;
    int _one;

public:
    Bitset(int size) {
        _size = size;
        sets.resize((size + 31) / 32);
        reverse = 0;
        zero = size;
        _one = 0;
    }

    void fix(int idx) {
        int index = idx / 32;
        int bit = idx % 32;
        if (!reverse) {
            if ((sets[index] & (1 << bit)) == 0) {
                zero--;
                _one++;
                sets[index] |= (1 << bit);
            }
        }else{
            if ((sets[index] & (1 << bit)) != 0) {
                zero--;
                _one++;
                sets[index] ^= (1 << bit);
            }
        }
    }

    void unfix(int idx) {
        int index = idx / 32;
        int bit = idx % 32;
        if (!reverse) {
            if ((sets[index] & (1 << bit)) != 0) {
                _one--;
                zero++;
                sets[index] ^= (1 << bit);
            }
        } else {
            if ((sets[index] & (1 << bit)) == 0) {
                _one--;
                zero++;
                sets[index] |= (1 << bit);
            }
        }
    }

    void flip() {
        reverse = !reverse;
        swap(_one, zero);
    }

    bool all() {
        return _size == _one;
    }

    bool one() {
        return _one > 0;
    }

    int count() {
        return _one;
    }

    std::string toString() {
        string res;
        for (int i = 0; i < _size; ++i) {
            int index = i / 32;
            int bit = i % 32;
            res += (((sets[index] >> bit) & 1) ^ reverse) == 1 ? "1" : "0";
        }
        return res;
    }
};




// 对数器测试
int main() {


    return 0;
}