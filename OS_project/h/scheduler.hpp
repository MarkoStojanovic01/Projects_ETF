//
// Created by os on 9/4/22.
//

#ifndef PROJECT_BASE_SCHEDULER_HPP
#define PROJECT_BASE_SCHEDULER_HPP

#include "list.hpp"

class kThread;

class scheduler {
private:
    static List<kThread> readyCoroutineQueue;

public:
    static kThread *get();

    static void put(kThread *ccb);
};


#endif //PROJECT_BASE_SCHEDULER_HPP
