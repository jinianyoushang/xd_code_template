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


int main() {
    Node *head = new Node(0);
    head->next = new Node(1);
    head->next->pre = head;
    auto res = insert(head, 2, 3);

    while (res){
        cout<<res->val<<" ";
        res=res->next;
    }

    return 0;
}