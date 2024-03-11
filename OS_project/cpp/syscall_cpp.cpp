//
// Created by os on 9/9/22.
//

#include "../h/syscall_cpp.hpp"

Thread::Thread(void (*body)(void *), void *arg) {
    thread_create_no_starting(&myHandle,body,arg);
}

int Thread::start() {
    return thread_start(myHandle);
}

void Thread::dispatch() {
    thread_dispatch();
}

Thread::Thread() {
    thread_create_no_starting(&myHandle,threadWrapper,this);
}

void Thread::threadWrapper(void *arg) {
    Thread* t = (Thread*)arg;
    t->run();
}

Semaphore::Semaphore(unsigned int init) {
    sem_open(&myHandle,init);
}

int Semaphore::wait() {
    return sem_wait(myHandle);
}

Semaphore::~Semaphore() {
    sem_close(myHandle);
}

int Semaphore::signal() {
    return sem_signal(myHandle);
}

char Console::getc() {
    return ::getc();
}

void Console::putc(char c) {
    ::putc(c);
}

void *operator new(size_t size) {
    return mem_alloc(size);
}

void operator delete(void *p) {
    mem_free(p);
}
