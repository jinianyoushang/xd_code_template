#pragma once
//ʹ��CASָ��ʵ���������ζ���  ����ԭ�ӱ���
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

        //�����ڲ�Ԫ�ص���������
        while (_head != _tail) {
            std::allocator<T>::destroy(_data + _head);
            _head = (_head+1)%_max_size;
        }
        //���û��ղ���
        std::allocator<T>::deallocate(_data, _max_size);
    }

    //��ʵ��һ���ɱ�����б�汾�Ĳ��뺯����Ϊ��׼����
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

        //�ж϶����Ƿ�����
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
        //��β��λ�ù���һ��T���͵Ķ��󣬹������Ϊargs...
        std::allocator<T>::construct(_data + _tail, std::forward<Args>(args)...);
        //����β��Ԫ��λ��
        _tail = (_tail + 1) % _max_size;

        do
        {
            use_expected = true;
            use_desired = false;
        }
        while (!_atomic_using.compare_exchange_strong(use_expected, use_desired));

        return true;
    }

    //push ʵ�������汾��һ��������ֵ���ã�һ��������ֵ����

    //������ֵ���ð汾
    bool push(const T& val) {
        std::cout << "called push const T& version" << std::endl;
        return emplace(val);
    }

    //������ֵ���ð汾����ȻҲ���Խ�����ֵ���ã�T&&Ϊ��������
    // ������Ϊ����ʵ����const T&
    bool push(T&& val) {
        std::cout << "called push T&& version" << std::endl;
        return emplace(std::move(val));
    }

    //���Ӻ���
    bool pop(T& val) {

        bool use_expected = false;
        bool use_desired = true;
        do
        {
            use_desired = true;
            use_expected = false;
        } while (!_atomic_using.compare_exchange_strong(use_expected, use_desired));
        //�ж�ͷ����β��ָ���Ƿ��غϣ�����غ������Ϊ��
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
        //ȡ��ͷ��ָ��ָ�������
        val = std::move(_data[_head]);
        //����ͷ��ָ��
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
