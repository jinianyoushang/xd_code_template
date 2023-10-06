//使用list不用自己写 双向链表
//这里可能需要实际删除节点创建节点，如果自己写双向链表，就不需要分配新的节点
//但是我推荐这种写法，不用new delete
struct Node {
    int key, val;

    Node(int key, int val) : key(key), val(val) {}
};

class LRUCache {
private:
    int capacity;
    list<Node> lruList;
    unordered_map<int, list<Node>::iterator> keyTable;//指向实际值
public:
    explicit LRUCache(int _capacity) {
        capacity = _capacity;
    }

    int get(int key) {
        auto it = keyTable.find(key);
        if (it == keyTable.end())return -1;
        int val = it->second->val;
        lruList.erase(it->second);
        lruList.emplace_front(key, val);
        keyTable[key] = lruList.begin();
        return lruList.front().val;
    }

    void put(int key, int value) {
        if (capacity == 0) return;
        //如果可以找到
        auto it = keyTable.find(key);
        if (it != keyTable.end()) {
            keyTable[key]->val = value;
            get(key);//放到头部
            return;
        }
        
        //如果找不到
        if (keyTable.size() == capacity) {//如果满了
            keyTable.erase(lruList.back().key);
            lruList.pop_back();
        }
        lruList.emplace_front(key, value);
        keyTable[key] = lruList.begin();
    }
};