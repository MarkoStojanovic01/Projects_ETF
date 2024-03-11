//
// Created by os on 8/26/22.
//

#ifndef PROJECT_BASE_SYSCALL_C_HPP
#define PROJECT_BASE_SYSCALL_C_HPP

#include "../lib/hw.h"

void* mem_alloc(size_t size);

int mem_free(void* adr);

class kThread;
typedef kThread* thread_t;

int thread_create(thread_t* handle,void(*start_routine)(void*), void* arg);

int thread_create_no_starting(thread_t* handle,void(*start_routine)(void*), void* arg);

int thread_exit();

void thread_dispatch();

int thread_start(thread_t handle);

class kSemaphore;
typedef kSemaphore* sem_t;

int sem_open(sem_t* handle, unsigned init);

int sem_close(sem_t handle);

int sem_wait(sem_t id);

int sem_signal(sem_t id);

const int EOF = -1;

char getc();

void putc(char c);



#endif //PROJECT_BASE_SYSCALL_C_HPP
