//
// Created by xd on 6/10/23.
//
#include <iostream>
#include "Threadpool.h"
#include "Task.h"

using namespace std;

int main(){
    Threadpool<Task> threadpool(2);

    Task task;
    Task task2;
    Task task3;

    threadpool.append(&task);
    threadpool.append(&task2);
    threadpool.append(&task3);

//    task.process();
    getchar();
    return 0;
}