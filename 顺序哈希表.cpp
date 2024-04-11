#include <iostream>
#include <list>
#include <unordered_map>
#include <utility>

template<typename KeyType, typename ValueType>
class OrderedHashMap {
public:
    // 插入一个键值对
    void insert(const KeyType&key, const ValueType&value) {
        // 检查键是否已存在
        if (map.find(key) == map.end()) {
            // 如果键不存在，插入到链表和哈希表中
            order.push_back(key);
            map[key] = std::make_pair(value, --order.end());
        }
        else {
            // 如果键已存在，更新哈希表中的值
            map[key].first = value;
        }
    }

    // 根据键查找值，如果找到返回值的指针，否则返回nullptr
    ValueType get(const KeyType&key) const {
        auto it = map.find(key);
        if (it != map.end()) {
            return it->second.first;
        }
        throw std::out_of_range("不存在的键");
    }

    bool exist(const KeyType&key) {
        auto it = map.find(key);
        if (it != map.end()) {
            return true;
        }
        return false;
    }

    // 遍历有序哈希表
    void print() const {
        for (const auto&key: order) {
            std::cout << key << ": " << map.at(key).first << std::endl;
        }
    }

private:
    std::list<KeyType> order; // 维护插入顺序
    std::unordered_map<KeyType, std::pair<ValueType, typename std::list<KeyType>::iterator>> map; // 哈希表存储值和对应在list中的位置
};

int main() {
    OrderedHashMap<int, std::string> map;
    map.insert(1, "one");
    map.insert(2, "two");
    map.insert(3, "three");
    map.insert(2, "TWO"); // 更新已有键的值

    if (map.exist(2)) {
        const std::string value = map.get(2);
        std::cout << "Found: " << value << std::endl;
    }
    else {
        std::cout << "Not found." << std::endl;
    }

    map.print(); // 应该按照 1:one, 2:TWO, 3:three 的顺序打印

    return 0;
}
