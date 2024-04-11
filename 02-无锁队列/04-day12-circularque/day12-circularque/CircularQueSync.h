#pragma once
//���Ƽ��İ汾 �߶��Ż���
#include <iostream>
#include <memory>

template<typename T, size_t Cap>
class CircularQueSync : private std::allocator<T>
{
public:
    CircularQueSync() :_max_size(Cap + 1),
        _data(std::allocator<T>::allocate(_max_size))
        , _head(0), _tail(0), _tail_update(0) {}

    CircularQueSync(const CircularQueSync&) = delete;
    CircularQueSync& operator = (const CircularQueSync&) volatile = delete;
    CircularQueSync& operator = (const CircularQueSync&) = delete;

    ~CircularQueSync()
    {
        //�����ڲ�Ԫ�ص���������
        while (_head != _tail) {
            std::allocator<T>::destroy(_data + _head);
            _head = (++_head)%_max_size;
        }
        //���û��ղ���
        std::allocator<T>::deallocate(_data, _max_size);
    }

    //���Ӻ���

    bool pop(T& val) {

        size_t h;
        do
        {
            h = _head.load(std::memory_order_relaxed);  //1 ��
            //�ж�ͷ����β��ָ���Ƿ��غϣ�����غ������Ϊ��
            if (h == _tail.load(std::memory_order_acquire)) //2��
            {
                std::cout << "circular que empty ! " << std::endl;
                return false;
            }

            //�ж������ʱҪ��ȡ�����ݺ�tail_update�Ƿ�һ�£����һ��˵��β������δ������
            if (h == _tail_update.load(std::memory_order_acquire)) //3��
            {
                return false;
            }
            val = _data[h]; // 2��

        } while (!_head.compare_exchange_strong(h,
            (h + 1) % _max_size, std::memory_order_release, std::memory_order_relaxed)); //4 ��
        std::cout << "pop data success, data is " << val << std::endl;
        return true;
    }

    bool push(const T& val)
    {
        size_t t;
        do
        {
            t = _tail.load(std::memory_order_relaxed);  //5
            //�ж϶����Ƿ���
            if ((t + 1) % _max_size == _head.load(std::memory_order_acquire))
            {
                std::cout << "circular que full ! " << std::endl;
                return false;
            }



        } while (!_tail.compare_exchange_strong(t,
            (t + 1) % _max_size, std::memory_order_release, std::memory_order_relaxed));  //6

        _data[t] = val; 
        size_t tailup;
        do
        {
            tailup = t;

        } while (!_tail_update.compare_exchange_strong(tailup,
            (tailup + 1) % _max_size, std::memory_order_release, std::memory_order_relaxed)); //7

        std::cout << "called push data success " << val << std::endl;
        return true;
    }

private:
    size_t _max_size;
    T* _data;
    std::atomic<size_t>  _head;
    std::atomic<size_t> _tail;
    std::atomic<size_t> _tail_update;
};