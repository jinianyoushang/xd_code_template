#include <mutex>
#include <condition_variable>
#include <queue>
#include <memory>
#include <iostream>

template<typename T>
class threadsafe_queue {
private:
    mutable std::mutex mut;
    std::queue<T> data_queue;
    std::condition_variable data_cond; // 用于通知队列非空
    std::condition_variable space_cond; // 用于通知队列未满
    size_t max_size; // 队列的最大容量

public:
    explicit threadsafe_queue(size_t max_size) : max_size(max_size) {
    }

    void push(T new_value) {
        std::unique_lock<std::mutex> lk(mut);
        // 等待队列非满
        space_cond.wait(lk, [this] { return data_queue.size() < max_size; });
        data_queue.push(std::move(new_value));
        data_cond.notify_one();
    }

    void wait_and_pop(T&value) {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk, [this] { return !data_queue.empty(); });
        value = std::move(data_queue.front());
        data_queue.pop();
        // 通知可能在等待的push操作，队列现在已经不满了
        space_cond.notify_one();
    }

    std::shared_ptr<T> wait_and_pop() {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk, [this] { return !data_queue.empty(); });
        std::shared_ptr<T> res(std::make_shared<T>(std::move(data_queue.front())));
        data_queue.pop();
        // 通知可能在等待的push操作，队列现在已经不满了
        space_cond.notify_one();
        return res;
    }

    bool try_pop(T&value) {
        std::lock_guard<std::mutex> lk(mut);
        if (data_queue.empty())
            return false;
        value = std::move(data_queue.front());
        data_queue.pop();
        // 通知可能在等待的push操作，队列现在已经不满了
        space_cond.notify_one();
        return true;
    }

    std::shared_ptr<T> try_pop() {
        std::lock_guard<std::mutex> lk(mut);
        if (data_queue.empty())
            return std::shared_ptr<T>();
        std::shared_ptr<T> res(std::make_shared<T>(std::move(data_queue.front())));
        data_queue.pop();
        // 通知可能在等待的push操作，队列现在已经不满了
        space_cond.notify_one();
        return res;
    }

    bool empty() const {
        std::lock_guard<std::mutex> lk(mut);
        return data_queue.empty();
    }

    size_t size() const {
        std::lock_guard<std::mutex> lk(mut);
        return data_queue.size();
    }
};

threadsafe_queue<int> queue(5);

// 测试代码
void test1() {
    for (int i = 0; i < 20; ++i) {
        queue.push(i);
        std::cout<< "push " << i << std::endl;
    }
}

void test2() {
    while (true) {
        auto value = queue.wait_and_pop();
        std::cout<< "pop " << *value << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

int main() {

    std::thread t1(test1);
    std::thread t2(test2);

    t1.join();
    t2.join();


    // 测试代码
}
