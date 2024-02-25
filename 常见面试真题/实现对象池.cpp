#include <iostream>
#include <bits/stdc++.h>

using namespace std;

template<typename Object>
class ObjectPool {
public:
    ObjectPool(size_t unSize) : m_unSize(unSize) {
        for (size_t unIdx = 0; unIdx < m_unSize; ++unIdx) {
            m_oPool.push_back(new Object());
        }
    }

    ~ObjectPool() {
        for (const auto &item: m_oPool) {
            delete item;
        }
        m_unSize = 0;
    }

    Object *GetObject() {
        Object *pObj = NULL;
        if (0 == m_unSize) {
            pObj = new Object();
        } else {
            pObj = m_oPool.front();
            m_oPool.pop_front();
            --m_unSize;
        }
        return pObj;
    }

    void returnObject(Object *pObj) {
        m_oPool.push_back(pObj);
        ++m_unSize;
    }

private:
    size_t m_unSize;
    std::list<Object *> m_oPool;
};

int main() {

    return 0;
}