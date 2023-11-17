//
// Created by root on 6/5/23.
//

#include <semaphore.h>
#include <exception>
#include "Semaphore.h"

Semaphore::Semaphore() {
    if (sem_init(&m_sem, 0, 0) != 0) {
        throw std::exception();
    }
}

Semaphore::Semaphore(int num) {
    if (sem_init(&m_sem, 0, num) != 0) {
        throw std::exception();
    }
}

Semaphore::~Semaphore() {
    sem_destroy(&m_sem);
}

bool Semaphore::wait() {
    return sem_wait(&m_sem) == 0;
}

bool Semaphore::post() {
    return sem_post(&m_sem) == 0;
}
