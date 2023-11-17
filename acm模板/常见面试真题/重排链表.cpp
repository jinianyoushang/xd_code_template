#include <iostream>
#include <bits/stdc++.h>

using namespace std;
//例如：有n个节点的单链表：1 -> 2 -> 3 -> ... -> n-2 -> n-1 -> n -> NULL
//处理后的链表为1 -> n > 2 -> n-1 -> 3 -> n-2 ...

struct ListNode {
    int val;
    ListNode *next = nullptr;

    explicit ListNode(int val) : val(val) {}
};


ListNode *reverseList(ListNode *head) {
    if (!head) return head;
    ListNode *pre = nullptr, *next;
    while (head) {
        next = head->next;
        head->next = pre;
        pre = head;
        head = next;
    }
    return pre;
}

ListNode *solution(ListNode *head) {
    if (!head) return nullptr;
    //快慢指针找到中间节点
    ListNode *fast = head, *slow = head;
    while (fast->next && fast->next->next) {
        slow = slow->next;
        fast = fast->next;
        fast = fast->next;
    }

    ListNode *node1 = head;
    ListNode *node2 = reverseList(slow->next);
    slow->next = nullptr;
    //合并两个链表
    while (node1 && node2) {
        auto t1 = node1->next;
        auto t2 = node2->next;
        node1->next = node2;
        node2->next = t1;
        node1 = t1;
        node2 = t2;
    }
    return head;
}

int main() {
    auto *node1 = new ListNode(1);
    ListNode *temp = node1;
    for (int i = 2; i <= 7; ++i) {
        temp->next = new ListNode(i);
        temp = temp->next;
    }

    auto res = solution(node1);
    while (res) {
        cout << res->val << " ";
        res = res->next;
    }
    return 0;
}