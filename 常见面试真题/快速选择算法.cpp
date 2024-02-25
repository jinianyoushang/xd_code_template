#include <iostream>
#include <unordered_map>
#include <vector>
#include <bits/stdc++.h>

using namespace std;


int partition(vector<int> &v, int l, int r) {
    int temp = v[l];
    while (l < r) {
        while (l < r && v[r] >= temp) {
            r--;
        }
        swap(v[l], v[r]);
        while (l < r && v[l] <= temp) {
            l++;
        }
        swap(v[l], v[r]);
    }
    return l;
}

//快速选择算法  返回第index大值的索引 从0开始
int quickSelect(vector<int> &v, int k, int l, int r) {
    if (l == r) {
        return v[l];
    }
    int div = partition(v, l, r);
    if (div == k) {
        return v[div]; //如果找到
    } else if (div > k) {
        return quickSelect(v, k, l, div - 1);
    }
    return quickSelect(v, k, div + 1, r);
}

int main() {
    vector<int> nums{10, 9, 2, 5, 3, 7, 101, 18};

    for (int i = 0; i < nums.size(); ++i) {
        cout << quickSelect(nums, i, 0, nums.size() - 1) << endl;
    }
    return 0;
}
/**


*/

