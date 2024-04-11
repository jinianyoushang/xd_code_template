#pragma once
//ʹ�����Ļ��ζ���
#include <iostream>
#include <mutex>
#include <memory>

template<typename T, size_t Cap>
class CircularQueLk :private std::allocator<T> {
public:
    CircularQueLk() :_max_size(Cap + 1),_data(std::allocator<T>::allocate(_max_size)), _head(0), _tail(0) {}
    CircularQueLk(const CircularQueLk&) = delete;
    CircularQueLk& operator = (const CircularQueLk&) volatile = delete;
    CircularQueLk& operator = (const CircularQueLk&) = delete;

    ~CircularQueLk() {
        //ѭ������
        std::lock_guard<std::mutex>  lock(_mtx);
        //�����ڲ�Ԫ�ص���������
        while (_head != _tail) {
            std::allocator<T>::destroy(_data + _head);
            _head = (_head + 1) % _max_size;
        }
        //���û��ղ���
        std::allocator<T>::deallocate(_data, _max_size);
    }

    //��ʵ��һ���ɱ�����б�汾�Ĳ��뺯����Ϊ��׼����
    template <typename ...Args>
    bool emplace(Args && ... args) {
        std::lock_guard<std::mutex> lock(_mtx);
        //�ж϶����Ƿ�����
        if ((_tail + 1) % _max_size == _head) {
            std::cout << "circular que full ! " << std::endl;
            return false;
        }
        //��β��λ�ù���һ��T���͵Ķ��󣬹������Ϊargs...
        std::allocator<T>::construct(_data + _tail, std::forward<Args>(args)...);
        //����β��Ԫ��λ��
        _tail = (_tail + 1) % _max_size;
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
        std::lock_guard<std::mutex> lock(_mtx);
        //�ж�ͷ����β��ָ���Ƿ��غϣ�����غ������Ϊ��
        if (_head == _tail) {
            std::cout << "circular que empty ! " << std::endl;
            return false;
        }
        //ȡ��ͷ��ָ��ָ�������
        val = std::move(_data[_head]);
        //����ͷ��ָ��
        _head = (_head + 1) % _max_size;
        return true;
    }
private:
    size_t _max_size;
    T* _data;
    std::mutex _mtx;
    size_t _head = 0;
    size_t _tail = 0;
};
