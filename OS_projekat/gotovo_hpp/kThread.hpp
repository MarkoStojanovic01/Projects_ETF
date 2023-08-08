//
// Created by os on 9/4/22.
//

#ifndef PROJECT_BASE_KTHREAD_HPP
#define PROJECT_BASE_KTHREAD_HPP

#include "../lib/hw.h"
#include "scheduler.hpp"
#include "MemoryAllocator.hpp"

class kThread {

public:

    ~kThread(){delete[] stack;}

    using Body = void(*)(void*);

    bool isFinished() const { return finished; }

    void setFinished(bool value) { finished = value; }

    static int startThread(kThread* handle);

    static int createThread(kThread** handle,Body body, void* args);

    static int createNoStarting(kThread** handle,Body body, void* args);

    static kThread* running;

    static void yield();

    static int t_exit();

    void* operator new(size_t size){
        return MemoryAllocator::getInstance()->kmalloc(size);
    }
    void operator delete(void* p){
        MemoryAllocator::getInstance()->kfree(p);
    }

private:

    kThread(Body body, void* args, bool starting):
        args(args),
        body(body),
        stack(body != nullptr ?
            new uint64[STACK_SIZE] : nullptr),
        context({(uint64) &threadWrapper,
                 stack != nullptr ? (uint64) &stack[STACK_SIZE] : 0
        }),
        finished(false)
    {
        if(body != nullptr){
            if(starting) {
                scheduler::put(this);
                started = true;
            }
            else started = false;
        }
        else {
            running = this;
            started = true;
        }
    }

    struct Context
    {
        uint64 ra;
        uint64 sp;
    };

    void* args;
    Body body;
    uint64 *stack;
    Context context;
    bool finished;
    bool started;

    friend class Riscv;
    friend class kSemaphore;

    static void threadWrapper();

    static void contextSwitch(Context *oldContext,
                              Context *runningContext);

    static void dispatch();

    static uint64 constexpr STACK_SIZE = 1024;
};


#endif //PROJECT_BASE_KTHREAD_HPP
