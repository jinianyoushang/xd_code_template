#pragma once
#include<atomic>
#include<memory>
//单线程消费，单线程生产的队列
template<typename T>
class SinglePopPush
{
private:
    struct node
    {
        std::shared_ptr<T> data;
        node* next;
        node() :
            next(nullptr)
        {}
    };
    std::atomic<node*> head;
    std::atomic<node*> tail;
    node* pop_head()
    {
        node* const old_head = head.load();
        // ⇽-- - 1
        if (old_head == tail.load())   
        {
            return nullptr;
        }
        head.store(old_head->next);
        return old_head;
    }
public:
    SinglePopPush() :
        head(new node), tail(head.load())
    {}
    SinglePopPush(const SinglePopPush& other) = delete;
    SinglePopPush& operator=(const SinglePopPush& other) = delete;
    ~SinglePopPush()
    {
        while (node* const old_head = head.load())
        {
            head.store(old_head->next);
            delete old_head;
        }
    }
    std::shared_ptr<T> pop()
    {
        node* old_head = pop_head();
        if (!old_head)
        {
            return std::shared_ptr<T>();
        }
        // ⇽-- -2
        std::shared_ptr<T> const res(old_head->data);  
            delete old_head;
        return res;
    }
    void push(T new_value)
    {
        std::shared_ptr<T> new_data(std::make_shared<T>(new_value));
        // ⇽-- - 3
        node* p = new node;    
        //⇽-- - 4
        node* const old_tail = tail.load(); 
        //⇽-- - 5
        old_tail->data.swap(new_data);   
        //⇽-- - 6
        old_tail->next = p; 
        //⇽-- - 7
        tail.store(p);    
    }
};