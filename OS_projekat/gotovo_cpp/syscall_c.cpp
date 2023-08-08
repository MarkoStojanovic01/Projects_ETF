//
// Created by os on 8/26/22.
//

#include "../h/syscall_c.hpp"

void *mem_alloc(size_t size) {


    __asm__ volatile("mv a1, %0" : : "r"(size));
    __asm__ volatile("mv a0, %0" : : "r"(0x1));
    __asm__("ecall");
    void* tmp;
    __asm__ volatile("mv %0,a0" : "=r"(tmp));
    return tmp;
}

int mem_free(void *adr) {
    __asm__ volatile("mv a1, %0" : : "r"(adr));
    __asm__ volatile("mv a0, %0" : : "r"(0x2));
    __asm__("ecall");
    int rt;
    __asm__ volatile("mv %0,a0" : "=r"(rt));
    return rt;
}

int thread_create(thread_t* handle,void (*start_routine)(void *), void *arg) {
    __asm__ volatile("mv a3, %0" : : "r"(arg));
    __asm__ volatile("mv a2, %0" : : "r"(start_routine));
    __asm__ volatile("mv a1, %0" : : "r"(handle));
    __asm__ volatile("mv a0, %0" : : "r"(0x11));
    __asm__("ecall");
    int rt;
    __asm__ volatile("mv %0,a0" : "=r"(rt));
    return rt;
}

int thread_exit() {
    __asm__ volatile("mv a0, %0" : : "r"(0x12));
    __asm__("ecall");
    int rt;
    __asm__ volatile("mv %0,a0" : "=r"(rt));
    return rt;
}

void thread_dispatch() {
    __asm__ volatile("mv a0, %0" : : "r"(0x13));
    __asm__("ecall");
}

int sem_open(sem_t *handle, unsigned int init) {
    __asm__ volatile("mv a2, %0" : : "r"(init));
    __asm__ volatile("mv a1, %0" : : "r"(handle));
    __asm__ volatile("mv a0, %0" : : "r"(0x21));
    __asm__("ecall");
    int rt;
    __asm__ volatile("mv %0,a0" : "=r"(rt));
    return rt;
}

int sem_close(sem_t handle) {
    __asm__ volatile("mv a1, %0" : : "r"(handle));
    __asm__ volatile("mv a0, %0" : : "r"(0x22));
    __asm__("ecall");
    int rt;
    __asm__ volatile("mv %0,a0" : "=r"(rt));
    return rt;
}

int sem_wait(sem_t id) {
    __asm__ volatile("mv a1, %0" : : "r"(id));
    __asm__ volatile("mv a0, %0" : : "r"(0x23));
    __asm__("ecall");
    int rt;
    __asm__ volatile("mv %0,a0" : "=r"(rt));
    return rt;
}

int sem_signal(sem_t id) {
    __asm__ volatile("mv a1, %0" : : "r"(id));
    __asm__ volatile("mv a0, %0" : : "r"(0x24));
    __asm__("ecall");
    int rt;
    __asm__ volatile("mv %0,a0" : "=r"(rt));
    return rt;
}

char getc() {
    __asm__ volatile("mv a0, %0" : : "r"(0x41));
    __asm__("ecall");
    char rt;
    __asm__ volatile("mv %0,a0" : "=r"(rt));
    return rt;
}

void putc(char c) {
    __asm__ volatile("mv a1, %0" : : "r"(c));
    __asm__ volatile("mv a0, %0" : : "r"(0x42));
    __asm__("ecall");
}

int thread_create_no_starting(thread_t *handle, void (*start_routine)(void *), void *arg) {
    __asm__ volatile("mv a3, %0" : : "r"(arg));
    __asm__ volatile("mv a2, %0" : : "r"(start_routine));
    __asm__ volatile("mv a1, %0" : : "r"(handle));
    __asm__ volatile("mv a0, %0" : : "r"(0x14));
    __asm__("ecall");
    int rt;
    __asm__ volatile("mv %0,a0" : "=r"(rt));
    return rt;
}

int thread_start(thread_t handle) {
    __asm__ volatile("mv a1, %0" : : "r"(handle));
    __asm__ volatile("mv a0, %0" : : "r"(0x15));
    __asm__("ecall");
    int rt;
    __asm__ volatile("mv %0,a0" : "=r"(rt));
    return rt;
}

