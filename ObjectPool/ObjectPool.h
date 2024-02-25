//
// Created by 17632 on 2023/11/21.
//

#ifndef OBJECTPOOL_H
#define OBJECTPOOL_H
#include <cstdio>
#include <memory>
#include<unordered_map>
#include<unordered_set>


template<typename T>
class ObjectPool {
public:
    explicit ObjectPool(const int min_size = 3, const int max_size = 10);

    ~ObjectPool();

    std::shared_ptr<T> getObject() {
        if (m_objects.empty()) {
            return nullptr;
        }

        T* obj = *m_objects.begin();
        m_objects.erase(m_objects.begin());
        printf("allocate a obj %p\n", obj);
        std::shared_ptr<T> temp = std::shared_ptr<T>(obj, [this](T* obj_) {
            m_objects.insert(obj_);
            //回收了一个对象
            printf("deallocate obj %p\n", obj_);
        });
        return temp;
    }

private:
    std::unordered_set<T *> m_objects;
    int m_min_size;
    int m_max_size;
};

template<typename T>
ObjectPool<T>::ObjectPool(const int min_size, const int max_size): m_min_size(min_size), m_max_size(max_size) {
    for (int i = 0; i < min_size; ++i) {
        m_objects.insert(new T);
    }
}

template<typename T>
ObjectPool<T>::~ObjectPool() {
    for (auto object: m_objects) {
        delete object;
    }
}


#endif //OBJECTPOOL_H
