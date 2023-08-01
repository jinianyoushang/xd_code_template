//
// Created by root on 6/5/23.
// 线程池类

#ifndef WEBSERVER_THREADPOOL_H
#define WEBSERVER_THREADPOOL_H

#include <pthread.h>
#include <exception>
#include <vector>
#include <list>
#include <iostream>
#include <string>
#include "locker.h"
#include "Semaphore.h"

//线程池类，定义成模板类是为了代码的复用，模板参数T是任务类
template<typename T>
class Threadpool {
private:
    //线程的数量
    int m_thread_number;
    //线程池数组,
    std::vector<pthread_t> m_threads;
    //请求队列中最多允许的，等待处理的请求数量
    int m_max_requests;
    //请求队列
    std::list<T *> m_workqueue;
    //互斥锁
    Locker m_queuelocker;
    //信号量用来判断是否有任务需要处理
    Semaphore m_queuestat;
    //是否结束线程
    bool m_stop{false};

    static void *worker(void *arg);
    void run();
public:
    explicit Threadpool(int mThreadNumber = 8, int mMaxRequests = 10000);
    ~Threadpool();
    //添加任务
    bool append(T *request);
};

template<typename T>
void Threadpool<T>::run() {
    while(!m_stop){
        m_queuestat.wait(); //信号量减一，放在最前面
        m_queuelocker.lock();
        if(m_workqueue.empty()){
            m_queuelocker.unlock();
            continue;
        }
        T *request=m_workqueue.front();
        m_workqueue.pop_front();
        m_queuelocker.unlock();

        //开始处理任务
        if(!request){
            continue;
        }
        std::cout<<"开始处理任务"<<std::endl;
        request->process();//每个任务都要定义process
        std::cout<<"处理任务完毕"<<std::endl;
    }
}

template<typename T>
Threadpool<T>::Threadpool(int mThreadNumber, int mMaxRequests) : m_thread_number(mThreadNumber),
                                                                 m_max_requests(mMaxRequests) {
    if (mThreadNumber <= 0 || mMaxRequests <= 0) {
        throw std::exception();
    }
    m_threads.resize(mThreadNumber);
    for (int i = 0; i < m_thread_number; ++i) {
        std::cout << "create the " << i << "th thread" << std::endl;
        if (pthread_create(&m_threads[i], nullptr, worker, this) != 0) {
            throw std::exception();
        }
        if (pthread_detach(m_threads[i])) {
            throw std::exception();
        }
    }

}

template<typename T>
Threadpool<T>::~Threadpool() {
    m_stop = true;
}

template<typename T>
void *Threadpool<T>::worker(void *arg) {
    auto pool=static_cast<Threadpool*>(arg);
    pool->run();
    return pool;
}

template<typename T>
bool Threadpool<T>::append(T *request) {

    m_queuelocker.lock();
    if (m_workqueue.size() > m_max_requests) {
        m_queuelocker.unlock();
        return false;
    }
    m_workqueue.push_back(request);
    m_queuelocker.unlock();
    m_queuestat.post(); //信号量加一，放在最后
    return true;
}

#endif //WEBSERVER_THREADPOOL_H
