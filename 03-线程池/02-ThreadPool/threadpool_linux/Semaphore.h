//
// Created by root on 6/5/23.
// 信号量的类

#ifndef WEBSERVER_SEMAPHORE_H
#define WEBSERVER_SEMAPHORE_H
#include <pthread.h>
#include <semaphore.h>

class Semaphore {
private:
    sem_t m_sem;
public:
    Semaphore();
    Semaphore(int num);
    ~Semaphore();
    bool wait();
    bool post();
};


#endif //WEBSERVER_SEMAPHORE_H