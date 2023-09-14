#include <iostream>
#include <vector>
#include <algorithm>


using namespace std;

void printVector(vector<int> v) {
    for (const auto &item: v) {
        cout << item << " ";
    }
    cout << endl;
}

void bubbleSort(vector<int> v) {
    for (int i = 0; i < v.size(); ++i) {
        for (int j = 0; j < v.size() - i - 1; ++j) {
            if (v[j] > v[j + 1]) {
                swap(v[j], v[j + 1]);
            }
        }
    }
    printVector(v);
}

void selectionSort(vector<int> v) {
    for (int i = 0; i < v.size() - 1; ++i) {
        int minIndex = i;
        for (int j = i; j < v.size(); ++j) {
            if (v[j] < v[minIndex]) {
                minIndex = j;
            }
        }
        swap(v[minIndex], v[i]);
    }
    printVector(v);
}


int help(vector<int> &v, int l, int r) {
    int num = v[l];
    while (l < r) {
        while (l < r && v[r] >= num) {
            r--;
        }
        swap(v[l], v[r]);
        while (l < r && v[l] <= num) {
            l++;
        }
        swap(v[l], v[r]);
    }
    return l;
}

void Qsort(vector<int> &v, int l, int r) {
    if (l >= r) {
        return;
    }
    int index = help(v, l, r);
    Qsort(v, l, index - 1);
    Qsort(v, index + 1, r);

}

void quickSort(vector<int> v) {
    Qsort(v, 0, v.size() - 1);
    printVector(v);
}

void insertSort(vector<int> v) {
    for (int i = 1; i < v.size(); ++i) {
        if (v[i] < v[i - 1]) {
            int num = v[i];
            int j = i - 1;
            //腾出位置
            for (; j >= 0 && num < v[j]; j--) {
                v[j + 1] = v[j];
            }
            v[j + 1] = num;
        }
    }
    printVector(v);
}

void Merge(vector<int> &v, int l, int mid, int h) {
    //low为第1有序区的第1个元素，i指向第1个元素, mid为第1有序区的最后1个元素
    int i = l, j = mid + 1, k = 0;////mid+1为第2有序区第1个元素，j指向第1个元素
    vector<int> temp(h - l + 1, 0);////temp数组暂存合并的有序序列
    //合并两个有序区间到temp;
    while (i <= mid && j <= h) {
        if (v[i] < v[j]) {
            temp[k++] = v[i++];
        } else {
            temp[k++] = v[j++];
        }
    }
    while (i <= mid) {
        temp[k++] = v[i++];
    }
    while (j <= h) {
        temp[k++] = v[j++];
    }
    //temp还原到v
    i = l;
    for (int num: temp) {
        v[i++] = num;
    }
}

void Msort(vector<int> &v, int l, int h) {
    if (l >= h) {
        return;
    }
    int mid = l + (h - l) / 2;
    Msort(v, l, mid);
    Msort(v, mid + 1, h);
    Merge(v, l, mid, h);
}

void mergeSort(vector<int> v) {
    Msort(v, 0, v.size() - 1);
    printVector(v);
}


//对于提前知道数的范围的比较好用
void countingSort(vector<int> v) {
    //如果不知道就不好弄
    int maxVal = *max_element(v.begin(), v.end());
    vector<int> temp(maxVal + 1, 0);
    for (const auto &item: v) {
        temp[item]++;
    }
    int index = 0;
    for (int i = 0; i < temp.size(); ++i) {
        while (temp[i]--) {
            v[index++] = i;
        }
    }
    printVector(v);
}

void heapAdjust(vector<int> &v, int s, int m) {
    int temp, j;
    temp = v[s];
    for (j = 2 * s; j <= m; j*=2) {
        if (j < m && v[j] < v[j + 1]) {
            j++;
        }
        if (temp >= v[j]) {
            break;
        }
        v[s] = v[j];
        s = j;
    }
    v[s] = temp;
}

void heapSort(vector<int> v) {
    int n = v.size();
    int i;
    for (i = n / 2; i >= 1; --i) {
        heapAdjust(v, i, n - 1);
    }
    for (int j = n - 1; j > 0; --j) {
        swap(v[0], v[j]);
        heapAdjust(v, 0, j - 1);
    }

    printVector(v);
}


int main(int argc, char **argv) {
    vector<int> v{9, 8, 1, 3, 2, 4, 5, 7, 6};

    bubbleSort(v);
    selectionSort(v);
    insertSort(v);
//    shellSort(v);
    heapSort(v);
    mergeSort(v);//还有非递归实现
    quickSort(v);//后面还有改进
    countingSort(v);

    return 0;
}







