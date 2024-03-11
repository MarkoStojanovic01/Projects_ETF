//
// Created by os on 9/4/22.
//

#include "../h/scheduler.hpp"

List<kThread> scheduler::readyCoroutineQueue;

kThread* scheduler::get()
{
    return readyCoroutineQueue.removeFirst();
}

void scheduler::put(kThread *ccb)
{
    readyCoroutineQueue.addLast(ccb);
}