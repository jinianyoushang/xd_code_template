#pragma once
//ʹ��CASָ��ʵ���������ζ���  2��ԭ�ӱ��� �Ƽ��İ汾
#include <iostream>
#include <mutex>
#include <memory>

template<typename T, size_t Cap>
class CircularQueLight: private std::allocator<T>
{
public:
    CircularQueLight():_max_size(Cap + 1),
	_data(std::allocator<T>::allocate(_max_size))
	, _head(0), _tail(0) , _tail_update(0){}

    CircularQueLight(const CircularQueLight&) = delete;
    CircularQueLight& operator = (const CircularQueLight&) volatile = delete;
    CircularQueLight& operator = (const CircularQueLight&) = delete;
    ~CircularQueLight()
    {
        //�����ڲ�Ԫ�ص���������
        while (_head != _tail) {
            std::allocator<T>::destroy(_data + _head);
            _head = (_head + 1) % _max_size;
        }
        //���û��ղ���
        std::allocator<T>::deallocate(_data, _max_size);
    }
    //���Ӻ���

	bool pop(T& val) {

        size_t h;
        do
        {
            h = _head.load();  //1 ��
            //�ж�ͷ����β��ָ���Ƿ��غϣ�����غ������Ϊ��
            if(h == _tail.load())
            {
                std::cout << "circular que empty ! " << std::endl;
                return false;
            }

            //�ж������ʱҪ��ȡ�����ݺ�tail_update�Ƿ�һ�£����һ��˵��β������δ������
            if(h == _tail_update.load())
            {
                return false;
            }
            val = _data[h]; // 2��

        } while (!_head.compare_exchange_strong(h, 
            (h+1)% _max_size)); //3 ��
        std::cout << "pop data success, data is " << val << std::endl;
		return true;
    }

    bool push(const T& val)
	{
        size_t t;
        do
        {
            t = _tail.load();  //1
            //�ж϶����Ƿ���
            if( (t+1)%_max_size == _head.load())
            {
                std::cout << "circular que full ! " << std::endl;
                return false;
            }

          

        } while (!_tail.compare_exchange_strong(t,
            (t + 1) % _max_size));  //3

		_data[t] = val; //2
        size_t tailup;
		do
        {
            tailup = t;

        } while (!_tail_update.compare_exchange_strong(tailup, 
            (tailup + 1) % _max_size));

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