#include <iostream>
#include <bits/stdc++.h>

using namespace std;

struct Node {
    int val;
    Node *next = nullptr;
    Node *pre = nullptr;

    Node(int val) : val(val) {}

    Node(int val, Node *next, Node *pre) : val(val), next(next), pre(pre) {}
};

Node *insert(Node *head, int val, int index) {
    //获取链表长度
    int len = 0;
    Node *node = head;
    while (node) {
        len++;
        node = node->next;
    }
    if (index > len || index < 0) {
        throw out_of_range("超出索引");
    }
    //找到要插入的位置
    node = head;
    if (index == 0) {
        Node *newHead = new Node(val);
        if (!node) return newHead;//
        newHead->next = node;
        node->pre = newHead;
        return newHead;
    }

    //其他情况
    for (int i = 0; i < index - 1; ++i) {
        node = node->next;
    }
    Node *pre = node;
    node = node->next;
    Node *newNode = new Node(val);
    newNode->next = node;
    if (node) {
        node->pre = newNode;
    }

    pre->next = newNode;
    newNode->pre = pre;
    return head;
}

//有前后哨兵
class DoubleList {
    Node *head;
    Node *tail;
    int m_len;
public:
    DoubleList() {
        head = new Node(-1);
        tail = new Node(-1);
        head->next = tail;
        tail->pre = head;
        m_len = 0;
    }

    void insertTail(int val) {
        insertIndex(val, m_len);
    }

    void insertIndex(int val, int index) {
        if (index < 0 || index > m_len) {
            throw out_of_range("out_of_range");
        }
        auto temp = head;
        for (int i = 0; i < index; ++i) {
            temp = temp->next;
        }
        auto next = temp->next;
        auto node = new Node(val);
        temp->next = node;
        node->next = next;
        next->pre = node;
        node->pre = temp;
        m_len++;
    }

    void printList() {
        auto temp = head->next;
        while (temp != tail) {
            cout << temp->val << " ";
            temp = temp->next;
        }
        cout << endl;
    }
};

int main() {
    {
        Node *head = new Node(0);
        head->next = new Node(1);
        head->next->pre = head;
        auto res = insert(head, 2, 2);
//        while (res) {
//            cout << res->val << " ";
//            res = res->next;
//        }
    }

    //测试2
    {
        DoubleList doubleList;
        doubleList.insertTail(0);
        doubleList.insertTail(1);
        doubleList.insertIndex(2, 2);
        doubleList.printList();
    }

    return 0;
}