//
// Created by os on 9/4/22.
//

#include "../h/kThread.hpp"
#include "../h/Riscv.hpp"
#include "../h/printing.hpp"

kThread *kThread::running = nullptr;

int kThread::createThread(kThread**  handle,kThread::Body body, void* args) {
    *handle = new kThread(body,args,true);
    return 0;
}

void kThread::dispatch() {
    kThread *old = running;
    if (!old->isFinished()) { scheduler::put(old); }
    else delete(old);
    running = scheduler::get();

    kThread::contextSwitch(&old->context, &running->context);
}

void kThread::threadWrapper()
{
    Riscv::popSppSpie();
    running->body(running->args);
    if(t_exit() == -1) printString("Sta??\n");
}

void kThread::yield() {
    __asm__ volatile("mv a0, %0" : : "r"(0x13)); // dispatch
    __asm__("ecall");
}

int kThread::t_exit() {
    running->setFinished(true);
    __asm__ volatile("mv a0, %0" : : "r"(0x13)); // dispatch
    __asm__("ecall");
    return -1;
}

int kThread::createNoStarting(kThread **handle, kThread::Body body, void *args) {
    *handle = new kThread(body,args,false);
    return 0;
}

int kThread::startThread(kThread* handle) {
    if(!handle->started) {
        scheduler::put(handle);
        handle->started = true;
        return 0;
    }
    return -1;
}
