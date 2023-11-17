#define _GLIBCXX_DEBUG

#include <bits/stdc++.h>

using namespace std;


class Task {
    atomic_int m_int = 1;
public:
    void process() {
        this_thread::sleep_for(chrono::seconds(1));
        cout << m_int++ << endl;
    }
};


template<typename T>
class ThreadPool {
private:
    queue<T *> q;
    int m_threadNumber;
    int m_queueMaxSize;
    atomic_bool m_exit = false;
    mutex m_mutex;
    condition_variable m_condition;
public:
    ThreadPool(int threadNumber = 8, int queueMaxSize = 65535) : m_threadNumber(threadNumber),
                                                                 m_queueMaxSize(queueMaxSize) {

        //创建线程
        for (int i = 0; i < m_threadNumber; ++i) {
            thread t(worker, this);
            t.detach();
        }
    }

    ~ThreadPool() {
        m_exit = true;
    }

    bool append(T *task) {
        unique_lock<mutex> lock(m_mutex);
        if (q.size() >= m_queueMaxSize) {
            return false;
        }
        q.push(task);
        m_condition.notify_one();
        return true;
    }

    static void worker(void *arg) {
        auto *pool = (ThreadPool *) arg;
        pool->run();
    }

    void run() {
        while (!m_exit) {
            unique_lock<mutex> lock(m_mutex);
            m_condition.wait(lock, [&] {
                return !q.empty();
            });
            auto task = q.front();
            q.pop();
            lock.unlock();
            //可以并发执行
            task->process();
        }
    }
};


int main() {
    Task task;
    ThreadPool<Task> pool;
    for (int i = 0; i < 100; ++i) {
        pool.append(&task);
    }

    //主线程等待线程池
    this_thread::sleep_for(chrono::seconds(100));
    return 0;
}