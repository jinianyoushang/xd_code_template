//
// Created by 17632 on 24-4-8.
// 线程安全的查找表
//

#ifndef THREADSAFEHASHTABLE_H
#define THREADSAFEHASHTABLE_H
#include <unordered_map>
#include <shared_mutex>

template<typename Key, typename Value, typename Hash=std::hash<Key>>
class ThreadSafeHashTable {
public:
    ThreadSafeHashTable() = default;

    ThreadSafeHashTable(const ThreadSafeHashTable&) = delete;

    ThreadSafeHashTable& operator=(const ThreadSafeHashTable&) = delete;

    ThreadSafeHashTable(ThreadSafeHashTable&&) = delete;

    ThreadSafeHashTable& operator=(ThreadSafeHashTable&&) = delete;

    ~ThreadSafeHashTable() = default;


    void insert(Key&&key, Value&&value);

    void insert(std::pair<Key, Value>&&pair);

    void erase(Key const&key);

    size_t size() const;

    auto find(Key const&key) const;

    typename std::unordered_map<Key, Value, Hash>::iterator begin();

    typename std::unordered_map<Key, Value, Hash>::iterator end();

    Value& operator [](const Key&key);

private:
    std::unordered_map<Key, Value, Hash> m_umap; //保存数据的容器
    mutable std::shared_mutex m_mutex;
};


template<typename Key, typename Value, typename Hash>
void ThreadSafeHashTable<Key, Value, Hash>::insert(Key&&key, Value&&value) {
    std::unique_lock lock(m_mutex);
    m_umap.insert(std::make_pair(std::forward<Key>(key), std::forward<Value>(value)));
}

template<typename Key, typename Value, typename Hash>
void ThreadSafeHashTable<Key, Value, Hash>::insert(std::pair<Key, Value>&&pair) {
    std::unique_lock lock(m_mutex);
    m_umap.insert(std::forward<std::pair<Key, Value>>(pair));
}

template<typename Key, typename Value, typename Hash>
void ThreadSafeHashTable<Key, Value, Hash>::erase(Key const&key) {
    std::unique_lock lock(m_mutex);
    m_umap.erase(key);
}

template<typename Key, typename Value, typename Hash>
auto ThreadSafeHashTable<Key, Value,
    Hash>::find(Key const&key) const {
    std::shared_lock lock(m_mutex);
    return m_umap.find(key);
}

template<typename Key, typename Value, typename Hash>
typename std::unordered_map<Key, Value, Hash>::iterator ThreadSafeHashTable<Key, Value, Hash>::begin() {
    std::shared_lock lock(m_mutex);
    return m_umap.begin();
}

template<typename Key, typename Value, typename Hash>
typename std::unordered_map<Key, Value, Hash>::iterator ThreadSafeHashTable<Key, Value, Hash>::end() {
    std::shared_lock lock(m_mutex);
    return m_umap.end();
}

template<typename Key, typename Value, typename Hash>
Value& ThreadSafeHashTable<Key, Value, Hash>::operator[](const Key&key) {
    std::unique_lock lock(m_mutex);
    return m_umap[key];
}

template<typename Key, typename Value, typename Hash>
size_t ThreadSafeHashTable<Key, Value, Hash>::size() const {
    std::shared_lock lock(m_mutex);
    return m_umap.size();
}


#endif //THREADSAFEHASHTABLE_H
