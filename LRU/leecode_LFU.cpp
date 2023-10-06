//使用list不用自己写 双向链表
//这里可能需要实际删除节点创建节点，如果自己写双向链表，就不需要分配新的节点
//但是我推荐这种写法，不用new delete
struct Node {
    int key, val, freq;
    Node(int key, int val, int freq) : key(key), val(val), freq(freq) {}
};

class LFUCache {
private:
    int minFreq, capacity;
    unordered_map<int, list<Node>::iterator> keyTable;//指向实际值
    unordered_map<int, list<Node>> freqTable;
public:
    explicit LFUCache(int _capacity) {
        capacity = _capacity;
        minFreq = 0;
        keyTable.clear();
        freqTable.clear();
    }

    int get(int key) {
        auto it = keyTable.find(key);
        if (it == keyTable.end()) {
            return -1;
        }
        auto node = it->second;
        int freq = node->freq;
        int val = node->val;
        //删除节点
        freqTable[freq].erase(node);
        if (freqTable[freq].empty()) {
            freqTable.erase(freq);
            if(minFreq==freq){
                minFreq++;
            }
        }
        //放在头部
        freqTable[freq + 1].emplace_front(key, val, freq + 1);
        keyTable[key] = freqTable[freq + 1].begin();
        return val;
    }

    void put(int key, int value) {
        if (capacity == 0) return;
        auto it = keyTable.find(key);
        if (it == keyTable.end()) {//如果不存在
            //如果满了，需要清空位置
            if (keyTable.size() == capacity) {
                auto node=freqTable[minFreq].back();
                keyTable.erase(node.key);
                freqTable[minFreq].pop_back();
                if(freqTable[minFreq].empty()){
                    freqTable.erase(minFreq);
                }
            }
            minFreq = 1;
            freqTable[1].emplace_front(key, value, 1);
            keyTable[key] = freqTable[1].begin();
        } else {
            it->second->val=value;
            get(key);//移动位置，更新信息
        }
    }
};
