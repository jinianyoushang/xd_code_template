#include <exception>
#include <iostream>
#include<bits/stdc++.h>
#include <unordered_map>

using namespace std;


struct Node {
    int val;
    int key = 0;
    Node *next = nullptr;
    Node *prev = nullptr;

    Node(int v) : val(v) {}

    Node(int k, int v) : key(k), val(v) {}
};

class LRU {
private:
    int m_size;//最大大小
    // int cur_size;//当前大小
    Node *head;
    Node *tail;
    unordered_map<int, Node *> umap;

public:
    LRU(int size) {
        m_size = size;
        head = new Node(0);
        tail = new Node(0);
        head->next = tail;
        tail->prev = head;
    }

    void insertHead(Node *node) {
        node->next = head->next;
        head->next->prev = node;
        node->prev = head;
        head->next = node;
    }

    void deleteNode(Node *node){
        Node *prev=node->prev;
        Node *next=node->next;
        delete node;
        prev->next=next;
        next->prev=prev;
    }

    void deleteTailNode() {
        Node *temp = tail->prev;
        deleteNode(temp);
    }

    void moveToHead(Node *node) {
        Node *prev = node->prev;
        Node *next = node->next;
        prev->next = next;
        next->prev = prev;
        insertHead(node);
    }

    void add(int key, int val) {
        Node *temp = new Node(key, val);
        umap[key] = temp;
        insertHead(temp);
    }

    void deleteTail() {
        //要判断是否存在
        if (head->next != tail) {
            int key = tail->prev->key;
            // Node *todelete=umap[key];
            umap.erase(key);
            deleteTailNode();
        } else {
            throw exception();//没有找到的异常，可以定义一个
        }
    }

    bool exist(int key) {
        return umap.count(key);
    }

    int getLenth() {
        return umap.size();
    }

    int getNode(int key) {
        if (umap.count(key)) {
            Node *temp = umap[key];
            moveToHead(temp);
            return umap[key]->val;
        } else {
            throw exception();//没有找到的异常，可以定义一个
        }
    }

    void printLRU(){
        Node *node=head->next;
        while (node!=tail){
            cout<<node->val<<" ";
            node=node->next;
        }
        cout<<endl;
    }
};

int main() {
    LRU lru(10);
    lru.add(1,1);
    lru.add(2,2);
    lru.add(3,3);
    lru.add(4,4);
    lru.printLRU();
    lru.deleteTail();
    lru.printLRU();
    cout<<lru.getNode(4);

    return 0;
}