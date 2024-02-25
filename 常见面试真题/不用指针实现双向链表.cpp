#include <iostream>
#include <unordered_map>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

//不使用指针实现链表
struct Node {
    int val;
    int next = -1;
    int prev = -1;
};

class ArrayList {
private:
    vector<Node> v;
    const int maxLen = 100;
    int len = 0;
    int headIndex = INT_MAX - 1;
    int tailIndex = INT_MAX;
public:
    ArrayList() {
        v.resize(maxLen);//后面可以考虑扩容
    }

    Node &getHead() {
        if (len > 0) {
            return v[0];
        } else {
            throw out_of_range("no element");
        }
    }


    void insertIndex(int val, int index) {
        if (index < 0 || index > len || index > maxLen) {
            throw out_of_range("out_of_range: " + to_string(index));
        }

        if (len == 0) {
            headIndex = 0;
            v[headIndex].val = val;
            v[headIndex].next = tailIndex;
            v[headIndex].prev = headIndex;
            len++;
            return;
        }
        //找一个空闲空间
        int freeIndex = 0;
        for (int i = 0; i < maxLen; ++i) {
            if (v[i].next == -1) {
                freeIndex = i;
                break;
            }
        }
        //初始化值
        v[freeIndex].val = val;
        //当插入头部时
        if (index == 0) {
            v[freeIndex].next = headIndex;
            v[headIndex].prev = freeIndex;
            headIndex = freeIndex;
            len++;
            return;
        }

        //当len不为0时，当插入位置为0时
        int curIndex = headIndex;
        for (int i = 0; i < index - 1; ++i) {
            curIndex = v[curIndex].next;
        }
        int next = v[curIndex].next;
        v[freeIndex].next = next;
        v[freeIndex].prev = curIndex;
        v[curIndex].next = freeIndex;
        if (index != len) {
            v[next].prev = curIndex;
        }
        len++;
    }

    void deleteNode(int index) {
        if (index >= len || index < 0) {
            throw out_of_range("out_of_range: " + to_string(index));
        }
        int curIndex = headIndex;
        int deleteIndex = headIndex;
        //单独处理删除头部
        if (index == 0) {
            curIndex = v[curIndex].next;
            headIndex = curIndex;
            v[deleteIndex].next = -1;
            v[curIndex].prev = headIndex;
            len--;
            return;
        }

        for (int i = 0; i < index; ++i) {
            curIndex = v[curIndex].next;
        }

        int prev = v[curIndex].prev;
        v[prev].next = v[curIndex].next;
        if(index!=len-1){
            v[v[curIndex].next].prev = prev;
        }
        v[curIndex].next = -1;
        len--;
    }

    void printList() {
        int cur = headIndex;
        for (int i = 0; i < len; ++i) {
            cout << v[cur].val << " ";
            cur = v[cur].next;
        }
        cout << endl;
    }
};


int main() {
    ArrayList arrayList;
    arrayList.insertIndex(0, 0);
    arrayList.insertIndex(1, 0);
    arrayList.insertIndex(2, 0);
    arrayList.insertIndex(3, 0);
    arrayList.printList();
    arrayList.deleteNode(3);
//    arrayList.deleteNode(0);
    arrayList.printList();

//    arrayList.dele

    return 0;
}


