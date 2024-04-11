#pragma once

#include <exception>
#include <mutex>
#include <stack>
#include <condition_variable>

struct empty_stack : std::exception
{
    const char* what() const throw();
};

//这个可能不是异常安全的
template<typename T>
class threadsafe_stack
{
private:
    std::stack<T> data;
    mutable std::mutex m;
public:
    threadsafe_stack() {}

    threadsafe_stack(const threadsafe_stack& other)
    {
        std::lock_guard<std::mutex> lock(other.m);
        data = other.data;
    }

    threadsafe_stack& operator=(const threadsafe_stack&) = delete;

    void push(T new_value)
    {
        std::lock_guard<std::mutex> lock(m);
        data.push(std::move(new_value));    // ⇽-- - ①
    }

    std::shared_ptr<T> pop()
    {
        std::lock_guard<std::mutex> lock(m);
        if (data.empty()) throw empty_stack();  //  ⇽-- - ②
            std::shared_ptr<T> const res(
                std::make_shared<T>(std::move(data.top())));   // ⇽-- - ③
            data.pop();   // ⇽-- - ④
            return res;
    }

    void pop(T& value)
    {
        std::lock_guard<std::mutex> lock(m);
        if (data.empty()) throw empty_stack();
        value = std::move(data.top());   // ⇽-- - ⑤
            data.pop();   // ⇽-- - ⑥
    }

    bool empty() const
    {
        std::lock_guard<std::mutex> lock(m);
        return data.empty();
    }
};


//可以等待的版本
//同样可能会有异常安全问题 pop的时候
template<typename  T>
class threadsafe_stack_waitable
{
private:
    std::stack<T> data;
    mutable std::mutex m;
    std::condition_variable cv;
public:
    threadsafe_stack_waitable() {}

    threadsafe_stack_waitable(const threadsafe_stack_waitable& other)
    {
        std::lock_guard<std::mutex> lock(other.m);
        data = other.data;
    }

    threadsafe_stack_waitable& operator=(const threadsafe_stack_waitable&) = delete;

    void push(T new_value)
    {
        std::lock_guard<std::mutex> lock(m);
        data.push(std::move(new_value));    // ⇽-- - ①
        cv.notify_one();
    }

    std::shared_ptr<T> wait_and_pop()
    {
        std::unique_lock<std::mutex> lock(m);
        cv.wait(lock, [this]()   //  ⇽-- - ②
            {
                if(data.empty())
                {
                    return false;
                }
                return true;
            });


        std::shared_ptr<T> const res(
            std::make_shared<T>(std::move(data.top())));   // ⇽-- - ③
        data.pop();   // ⇽-- - ④
        return res;
    }

    void wait_and_pop(T& value)
    {
        std::unique_lock<std::mutex> lock(m);
        cv.wait(lock, [this]()
            {
                if (data.empty())
                {
                    return false;
                }
                return true;
            });

        value = std::move(data.top());   // ⇽-- - ⑤
        data.pop();   // ⇽-- - ⑥
    }

    bool empty() const
    {
        std::lock_guard<std::mutex> lock(m);
        return data.empty();
    }

    bool try_pop(T& value)
    {
        std::lock_guard<std::mutex> lock(m);
        if(data.empty())
        {
            return false;
        }

        value = std::move(data.top());
        data.pop();
        return true;
    }

    std::shared_ptr<T> try_pop()
    {
        std::lock_guard<std::mutex> lock(m);
        if(data.empty())
        {
            return std::shared_ptr<T>();
        }

        std::shared_ptr<T> res(std::make_shared<T>(std::move(data.top())));
        data.pop();
        return res;
    }

};