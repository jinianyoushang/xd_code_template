//
// Created by root on 6/10/23.
//

#include <iostream>
#include <thread>
#include "Task.h"

void Task::process() {
    for (int i = 0; i < 10; ++i) {
        std::cout<<"process "<<i<<std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

}
