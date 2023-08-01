//
// Created by root on 6/10/23.
//

#ifndef THREADPOOL_TASK_H
#define THREADPOOL_TASK_H
#include "TaskInterface.h"

class Task:TaskInterface {
public:
    void process() override;
};


#endif //THREADPOOL_TASK_H
