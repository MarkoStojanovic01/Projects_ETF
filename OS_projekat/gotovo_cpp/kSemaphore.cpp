//
// Created by os on 9/6/22.
//

#include "../h/kSemaphore.hpp"

void kSemaphore::block() {
    kThread *old = kThread::running;
    blocked.addLast(old);
    kThread::running = scheduler::get();
    kThread::contextSwitch(&old->context, &kThread::running->context);
}

void kSemaphore::unblock() {
    scheduler::put(blocked.removeFirst());
}

int kSemaphore::wait() {
    if(--val < 0){
        __asm__ volatile("mv a1, %0" : : "r"(this));
        __asm__ volatile("mv a0, %0" : : "r"(0x20)); // block
        __asm__("ecall");
    }
    if(semDeleted) return -1;
    else return 0;
}

int kSemaphore::signal() {
    if(val++ < 0){
       unblock();
    }
    return 0;
}

int kSemaphore::create_sem(kSemaphore **handle, int val) {
    *handle = new kSemaphore(val);
    return 0;
}

