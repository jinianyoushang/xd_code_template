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


vector<int> bubbleSort(vector<int> v) {
    int n = v.size();
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (v[j] > v[j + 1]) {
                swap(v[j], v[j + 1]);
            }
        }
    }
    printVector(v);
    return v;
}

//改进版本，如果某趟有序直接退出
vector<int> bubbleSort2(vector<int> v) {
    int n = v.size();
    for (int i = 0; i < n - 1; ++i) {
        bool flag = true;
        for (int j = 0; j < n - i - 1; ++j) {
            if (v[j] > v[j + 1]) {
                swap(v[j], v[j + 1]);
                flag = false;
            }
        }
        if (flag) break;
    }
    printVector(v);
    return v;
}


void selectionSort(vector<int> v) {
    int n = v.size();
    for (int i = 0; i < n - 1; ++i) {
        int minIndex = i;
        for (int j = i; j < n; ++j) {
            if (v[minIndex] > v[j]) {
                minIndex = j;
            }
        }
        if (i != minIndex) {
            swap(v[minIndex], v[i]);
        }

    }
    printVector(v);
}

void insertSort(vector<int> v) {
    int n = v.size();
    for (int i = 1; i < v.size(); ++i) {
        int j;
        if (v[i] < v[i - 1]) {
            int temp = v[i];
            //腾出该有的位置
            for (j = i - 1; j >= 0 && v[j] > temp; --j) {
                v[j + 1] = v[j];
            }
            v[j + 1] = temp;
        }
    }
    printVector(v);
}


void shellSort(vector<int> v) {
    int n = v.size();
    int i, j;
    int increment = n;
    do {
        increment = increment / 3 + 1;
        for (i = increment; i < n; i++) {
            if (v[i] < v[i - increment]) {
                int temp = v[i];
                for (j = i - increment; j >= 0 && temp < v[j]; j -= increment) {
                    v[j + increment] = v[j];
                }
                v[j + increment] = temp;
            }
        }
    } while (increment > 1);

    printVector(v);
}

//本函数调整v[S]的关键字，使v[s..m]成为一个大顶堆
void heapAdjust(vector<int> &v, int s, int m) {
    int temp, j;
    temp = v[s];
    for (int j = 2 * s; j <= m; j *= 2) {
        if (j < m && v[j] < v[j + 1]) {
            ++j;
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
    for (i = n / 2; i >= 0; i--) { //构建一个大顶堆
        heapAdjust(v, i, n-1);
    }

    for (i = n - 1; i > 0; --i) {
        swap(v[0], v[i]);
        heapAdjust(v, 0, i - 1);  //重新调整
    }
    printVector(v);
}


void Merge(vector<int> &SR, vector<int> &TR, int i, int m, int n) {
    int j, k, l;
    for (j = m + 1, k = i; i <= m && j <= n; k++) {
        if (SR[i] < SR[j]) {
            TR[k] = SR[i++];
        } else {
            TR[k] = SR[j++];
        }
    }
    if (i <= m) {
        for (l = 0; l <= m - i; ++l) {
            TR[k + l] = SR[i + l];
        }
    }

    if (j <= n) {
        for (l = 0; l <= n - j; ++l) {
            TR[k + l] = SR[j + l];
        }
    }
}

void MSort(vector<int> &SR, vector<int> &TR1, int s, int t) {
    int m;
    vector<int> TR2(SR.size(), 0);
    if (s == t) {
        TR1[s] = SR[s];
    } else {
        m = (s + t) / 2;
        MSort(SR, TR2, s, m);
        MSort(SR, TR2, m + 1, t);
        Merge(TR2, TR1, s, m, t);
    }
}


void mergeSort(vector<int> v) {
    MSort(v, v, 0, v.size() - 1);
    printVector(v);
}


int Qpartition(vector<int> &v, int low, int high) {
    int pivotkey;
    pivotkey = v[low];
    while (low < high) {
        while (low < high && v[high] >= pivotkey)
            high--;
        swap(v[low], v[high]);
        while (low < high && v[low] <= pivotkey)
            low++;
        swap(v[low], v[high]);
    }
    return low;
}


void Qsort(vector<int> &v, int low, int high) {
    int pivot;
    if (low < high) {
        pivot = Qpartition(v, low, high);
        Qsort(v, low, pivot - 1);
        Qsort(v, pivot + 1, high);
    }
}

void quickSort(vector<int> v) {
    Qsort(v, 0, v.size() - 1);
    printVector(v);
}


int Qpartition2(vector<int> &v, int low, int high) {

    int mid = low + (high - low) / 2;//三数取中优化
    if (v[low] > v[high]) {
        swap(v[low], v[high]);
    }
    if (v[mid] > v[high]) {
        swap(v[mid], v[high]);
    }
    if (v[low] > v[mid]) {
        swap(v[mid], v[low]);
    }

    int pivotkey;
    pivotkey = v[low];
    while (low < high) {
        while (low < high && v[high] >= pivotkey)
            high--;
        swap(v[low], v[high]);
        while (low < high && v[low] <= pivotkey)
            low++;
        swap(v[low], v[high]);
    }
    return low;
}

void Qsort2(vector<int> &v, int low, int high) {
    int pivot;
    if (low < high) {
        pivot = Qpartition2(v, low, high);
        Qsort(v, low, pivot - 1);
        Qsort(v, pivot + 1, high);
    }
}


//三数取中优化
void quickSort2(vector<int> v) {
    Qsort2(v, 0, v.size() - 1);
    printVector(v);
}


int Qpartition3(vector<int> &v, int low, int high) {

    int mid = low + (high - low) / 2;//三数取中优化
    if (v[low] > v[high]) {
        swap(v[low], v[high]);
    }
    if (v[mid] > v[high]) {
        swap(v[mid], v[high]);
    }
    if (v[low] > v[mid]) {
        swap(v[mid], v[low]);
    }

    int pivotkey;
    pivotkey = v[low];
    ////优化不必要的交换
    int temp = pivotkey;
    while (low < high) {
        while (low < high && v[high] >= pivotkey)
            high--;
        v[low] = v[high];
        while (low < high && v[low] <= pivotkey)
            low++;
        v[high] = v[low];
    }
    v[low] = temp;
    return low;
}

void Qsort3(vector<int> &v, int low, int high) {
    int pivot;
    if (low < high) {
        pivot = Qpartition3(v, low, high);
        Qsort(v, low, pivot - 1);
        Qsort(v, pivot + 1, high);
    }
}


void quickSort3(vector<int> v) {
    Qsort3(v, 0, v.size() - 1);
    printVector(v);
}


int Qpartition4(vector<int> &v, int low, int high) {

    int mid = low + (high - low) / 2;//三数取中优化
    if (v[low] > v[high]) {
        swap(v[low], v[high]);
    }
    if (v[mid] > v[high]) {
        swap(v[mid], v[high]);
    }
    if (v[low] > v[mid]) {
        swap(v[mid], v[low]);
    }

    int pivotkey;
    pivotkey = v[low];
    ////优化不必要的交换
    int temp = pivotkey;
    while (low < high) {
        while (low < high && v[high] >= pivotkey)
            high--;
        v[low] = v[high];
        while (low < high && v[low] <= pivotkey)
            low++;
        v[high] = v[low];
    }
    v[low] = temp;
    return low;
}

void Qsort4(vector<int> &v, int low, int high) {
    int pivot;
    //优化小数组时的排序方案
    if (low < high) {
        //大于某个数时候用快排
        if ((high - low) > 7) {//这个值可以替换
            pivot = Qpartition4(v, low, high);
            Qsort(v, low, pivot - 1);
            Qsort(v, pivot + 1, high);
        }else{
            insertSort(v);// TODO这里应该用到low high,需要修改
        }
    }
}


void quickSort4(vector<int> v) {
    Qsort4(v, 0, v.size() - 1);
    printVector(v);
}



int Qpartition5(vector<int> &v, int low, int high) {

    int mid = low + (high - low) / 2;//三数取中优化
    if (v[low] > v[high]) {
        swap(v[low], v[high]);
    }
    if (v[mid] > v[high]) {
        swap(v[mid], v[high]);
    }
    if (v[low] > v[mid]) {
        swap(v[mid], v[low]);
    }

    int pivotkey;
    pivotkey = v[low];
    ////优化不必要的交换
    int temp = pivotkey;
    while (low < high) {
        while (low < high && v[high] >= pivotkey)
            high--;
        v[low] = v[high];
        while (low < high && v[low] <= pivotkey)
            low++;
        v[high] = v[low];
    }
    v[low] = temp;
    return low;
}

void Qsort5(vector<int> &v, int low, int high) {
    int pivot;
    //优化小数组时的排序方案
    if (low < high) {
        //大于某个数时候用快排
        if ((high - low) > 7) {//这个值可以替换
            while (low<high){
                pivot=Qpartition5(v, low, high);
                Qsort(v, low, pivot - 1);
                low=pivot+1;
            }
        }else{
            insertSort(v);// TODO这里应该用到low high,需要修改
        }
    }
}


void quickSort5(vector<int> v) {
    Qsort5(v, 0, v.size() - 1);
    printVector(v);
}


int main(int argc, char **argv) {
    vector<int> v{9, 8, 1, 3, 2, 4, 5, 7, 6};

    bubbleSort(v);
    bubbleSort2(v);
    selectionSort(v);
    insertSort(v);
    shellSort(v);
    heapSort(v);
    mergeSort(v);//还有非递归实现

    quickSort(v);//后面还有改进
    quickSort2(v);//三数取中优化
    quickSort3(v);//优化不必要的交换
    quickSort4(v);//优化小数组时的排序方案
    quickSort5(v);//优化递归操作

    return 0;
}




