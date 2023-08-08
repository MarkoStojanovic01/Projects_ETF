//
// Created by os on 9/6/22.
//

#ifndef PROJECT_BASE_KSEMAPHORE_HPP
#define PROJECT_BASE_KSEMAPHORE_HPP

#include "../lib/hw.h"
#include "kThread.hpp"

class kSemaphore {

public:
    kSemaphore(int init ) : val(init){
        semDeleted = false;
    }
    ~kSemaphore(){
        kThread* cur = nullptr;
        bool blockedNotEmpty = false;
        while((cur = blocked.removeFirst()) != 0){
            scheduler::put(cur);
            blockedNotEmpty = true;
        }
        if(blockedNotEmpty) semDeleted = true;
    }

    static int create_sem(kSemaphore** handle, int val);

    int wait();
    int signal();

    int getValue()const{return val;}

protected:
    void block();
    void unblock();

private:

    void* operator new(size_t size){
        return MemoryAllocator::getInstance()->kmalloc(size);
    }
    void operator delete(void* p){
        MemoryAllocator::getInstance()->kfree(p);
    }

    int val;
    List<kThread> blocked;

    bool volatile semDeleted;

    friend class Riscv;
};


#endif //PROJECT_BASE_KSEMAPHORE_HPP
