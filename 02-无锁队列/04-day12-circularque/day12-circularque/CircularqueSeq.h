#pragma once
//使用CAS指令实现无锁环形队列  单个原子变量
#include <iostream>
#include <mutex>
#include <memory>

template<typename T, size_t Cap>
class CircularQueSeq :private std::allocator<T> {
public:
    CircularQueSeq() :_max_size(Cap + 1), _data(std::allocator<T>::allocate(_max_size)), _atomic_using(false),_head(0), _tail(0) {}
    CircularQueSeq(const CircularQueSeq&) = delete;
    CircularQueSeq& operator = (const CircularQueSeq&) volatile = delete;
    CircularQueSeq& operator = (const CircularQueSeq&) = delete;

    ~CircularQueSeq() {

        //调用内部元素的析构函数
        while (_head != _tail) {
            std::allocator<T>::destroy(_data + _head);
            _head = (_head+1)%_max_size;
        }
        //调用回收操作
        std::allocator<T>::deallocate(_data, _max_size);
    }

    //先实现一个可变参数列表版本的插入函数最为基准函数
    template <typename ...Args>
    bool emplace(Args && ... args) {

    	bool use_expected = false;
        bool use_desired = true;
        do
        {
            use_expected = false;
            use_desired = true;
        }
        while (!_atomic_using.compare_exchange_strong(use_expected, use_desired));

        //判断队列是否满了
        if ((_tail + 1) % _max_size == _head) {
            std::cout << "circular que full ! " << std::endl;
            do
            {
                use_expected = true;
                use_desired = false;
            }
            while (!_atomic_using.compare_exchange_strong(use_expected, use_desired));
            return false;
        }
        //在尾部位置构造一个T类型的对象，构造参数为args...
        std::allocator<T>::construct(_data + _tail, std::forward<Args>(args)...);
        //更新尾部元素位置
        _tail = (_tail + 1) % _max_size;

        do
        {
            use_expected = true;
            use_desired = false;
        }
        while (!_atomic_using.compare_exchange_strong(use_expected, use_desired));

        return true;
    }

    //push 实现两个版本，一个接受左值引用，一个接受右值引用

    //接受左值引用版本
    bool push(const T& val) {
        std::cout << "called push const T& version" << std::endl;
        return emplace(val);
    }

    //接受右值引用版本，当然也可以接受左值引用，T&&为万能引用
    // 但是因为我们实现了const T&
    bool push(T&& val) {
        std::cout << "called push T&& version" << std::endl;
        return emplace(std::move(val));
    }

    //出队函数
    bool pop(T& val) {

        bool use_expected = false;
        bool use_desired = true;
        do
        {
            use_desired = true;
            use_expected = false;
        } while (!_atomic_using.compare_exchange_strong(use_expected, use_desired));
        //判断头部和尾部指针是否重合，如果重合则队列为空
        if (_head == _tail) {
            std::cout << "circular que empty ! " << std::endl;
            do
            {
                use_expected = true;
                use_desired = false;
            }
        	while (!_atomic_using.compare_exchange_strong(use_expected, use_desired));
            return false;
        }
        //取出头部指针指向的数据
        val = std::move(_data[_head]);
        //更新头部指针
        _head = (_head + 1) % _max_size;

        do
        {
            use_expected = true;
            use_desired = false;
        }while (!_atomic_using.compare_exchange_strong(use_expected, use_desired));
        return true;
    }
private:
    size_t _max_size;
    T* _data;
    std::atomic<bool> _atomic_using;
    size_t _head = 0;
    size_t _tail = 0;
};
