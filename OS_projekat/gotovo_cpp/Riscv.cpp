//
// Created by os on 8/26/22.
//

#include "../h/Riscv.hpp"
#include "../lib/console.h"
#include "../h/MemoryAllocator.hpp"
#include "../h/kThread.hpp"
#include "../h/printing.hpp"
#include "../h/kSemaphore.hpp"

void Riscv::popSppSpie()
{
    Riscv::mc_sstatus(Riscv::SSTATUS_SPP);
    __asm__ volatile("csrw sepc, ra");
    __asm__ volatile("sret");
}

void Riscv::handleSupervisorTrap(){

    uint64 scause = r_scause();

    uint8 code;
    __asm__ volatile("mv %0,a0" : "=r"(code));

    if(scause == 0x0000000000000008UL || scause == 0x0000000000000009UL) {
        if (code == 0x01) { // mem_alloc
            size_t size;
            uint64 volatile sepc = r_sepc() + 4;
            uint64 volatile sstatus = r_sstatus();
            __asm__ volatile("mv %0,a1" : "=r"(size));
            void *adr = MemoryAllocator::getInstance()->kmalloc(size);
            __asm__ volatile("mv a0, %0" : : "r"(adr));
            w_sstatus(sstatus);
            w_sepc(sepc);
        } else if (code == 0x02) { // mem_free
            char *adr;
            uint64 volatile sepc = r_sepc() + 4;
            uint64 volatile sstatus = r_sstatus();
            __asm__ volatile("mv %0,a1" : "=r"(adr));
            int val = MemoryAllocator::getInstance()->kfree(adr);
            __asm__ volatile("mv a0, %0" : : "r"(val));
            w_sstatus(sstatus);
            w_sepc(sepc);
        } else if (code == 0x11) { // thread_create
            void *args;
            kThread **handle = (kThread**)MemoryAllocator::getInstance()->kmalloc(sizeof(kThread*));
            kThread::Body body;
            uint64 volatile sepc1;
            __asm__ volatile("csrr s1,sepc");
            __asm__ volatile("mv %0,s1" : "=r"(sepc1));
            sepc1 += 4;
            uint64 volatile sstatus = r_sstatus();
            __asm__ volatile("ld s1, 11*8(fp)");
            __asm__ volatile("mv %0,s1" : "=r"(handle));
            __asm__ volatile("ld s1, 12*8(fp)");
            __asm__ volatile("mv %0,s1" : "=r"(body));
            __asm__ volatile("ld s1, 13*8(fp)");
            __asm__ volatile("mv %0,s1" : "=r"(args));
            int val = kThread::createThread(handle, body, args);
            __asm__ volatile("mv a0, %0" : : "r"(val));
            w_sstatus(sstatus);
            w_sepc(sepc1);
        } else if (code == 0x13) {  // dispatch
            uint64 volatile sepc = r_sepc() + 4;
            uint64 volatile sstatus = r_sstatus();
            kThread::dispatch();
            w_sstatus(sstatus);
            w_sepc(sepc);
        }
        else if(code == 0x12){  // exit
            uint64 volatile sepc = r_sepc() + 4;
            uint64 volatile sstatus = r_sstatus();
            int val = kThread::t_exit();
            __asm__ volatile("mv a0, %0" : : "r"(val));
            w_sstatus(sstatus);
            w_sepc(sepc);
        }
        else if (code == 0x14) { // thread_create_no_starting
            void *args;
            kThread **handle = (kThread**)MemoryAllocator::getInstance()->kmalloc(sizeof(kThread*));
            kThread::Body body;
            uint64 volatile sepc1;
            __asm__ volatile("csrr s1,sepc");
            __asm__ volatile("mv %0,s1" : "=r"(sepc1));
            sepc1 += 4;
            uint64 volatile sstatus = r_sstatus();
            __asm__ volatile("ld s1, 11*8(fp)");
            __asm__ volatile("mv %0,s1" : "=r"(handle));
            __asm__ volatile("ld s1, 12*8(fp)");
            __asm__ volatile("mv %0,s1" : "=r"(body));
            __asm__ volatile("ld s1, 13*8(fp)");
            __asm__ volatile("mv %0,s1" : "=r"(args));
            int val = kThread::createNoStarting(handle, body, args);
            __asm__ volatile("mv a0, %0" : : "r"(val));
            w_sstatus(sstatus);
            w_sepc(sepc1);
        }
        else if(code == 0x15){  // start
            kThread* handle;
            uint64 volatile sepc = r_sepc() + 4;
            uint64 volatile sstatus = r_sstatus();
            __asm__ volatile("mv %0,a1" : "=r"(handle));
            int val = kThread::startThread(handle);
            __asm__ volatile("mv a0, %0" : : "r"(val));
            w_sstatus(sstatus);
            w_sepc(sepc);
        }
        else if(code == 0x20){ // block
            uint64 volatile sepc = r_sepc() + 4;
            uint64 volatile sstatus = r_sstatus();
            kSemaphore* sem;
            __asm__ volatile("mv %0,a1" : "=r"(sem));
            sem->block();
            w_sstatus(sstatus);
            w_sepc(sepc);
        }
        else if(code == 0x21){
            kSemaphore **handle = (kSemaphore**)MemoryAllocator::getInstance()->kmalloc(sizeof(kSemaphore*));
            int val;
            uint64 volatile sepc1;
            __asm__ volatile("csrr s1,sepc");
            __asm__ volatile("mv %0,s1" : "=r"(sepc1));
            sepc1 += 4;
            uint64 volatile sstatus = r_sstatus();
            __asm__ volatile("ld s1, 11*8(fp)");
            __asm__ volatile("mv %0,s1" : "=r"(handle));
            __asm__ volatile("ld s1, 12*8(fp)");
            __asm__ volatile("mv %0,s1" : "=r"(val));
            int ret = kSemaphore::create_sem(handle,val);
            __asm__ volatile("mv a0, %0" : : "r"(ret));
            w_sstatus(sstatus);
            w_sepc(sepc1);
        }
        else if(code == 0x22){
            kSemaphore *handle;
            uint64 volatile sepc1;
            __asm__ volatile("csrr s1,sepc");
            __asm__ volatile("mv %0,s1" : "=r"(sepc1));
            sepc1 += 4;
            uint64 volatile sstatus = r_sstatus();
            __asm__ volatile("ld s1, 11*8(fp)");
            __asm__ volatile("mv %0,s1" : "=r"(handle));
            delete handle;
            int ret = 0;
            __asm__ volatile("mv a0, %0" : : "r"(ret));
            w_sstatus(sstatus);
            w_sepc(sepc1);
        }
        else if(code == 0x23){
            kSemaphore *handle;
            uint64 volatile sepc1;
            __asm__ volatile("csrr s1,sepc");
            __asm__ volatile("mv %0,s1" : "=r"(sepc1));
            sepc1 += 4;
            uint64 volatile sstatus = r_sstatus();
            __asm__ volatile("ld s1, 11*8(fp)");
            __asm__ volatile("mv %0,s1" : "=r"(handle));
            int ret = handle->wait();
            __asm__ volatile("mv a0, %0" : : "r"(ret));
            w_sstatus(sstatus);
            w_sepc(sepc1);
        }
        else if(code == 0x24){
            kSemaphore *handle;
            uint64 volatile sepc1;
            __asm__ volatile("csrr s1,sepc");
            __asm__ volatile("mv %0,s1" : "=r"(sepc1));
            sepc1 += 4;
            uint64 volatile sstatus = r_sstatus();
            __asm__ volatile("ld s1, 11*8(fp)");
            __asm__ volatile("mv %0,s1" : "=r"(handle));
            int ret = handle->signal();
            __asm__ volatile("mv a0, %0" : : "r"(ret));
            w_sstatus(sstatus);
            w_sepc(sepc1);
        }
        else if(code == 0x41){
            uint64 volatile sepc = r_sepc() + 4;
            uint64 volatile sstatus = r_sstatus();
            char ret = __getc();
            __asm__ volatile("mv a0, %0" : : "r"(ret));
            w_sstatus(sstatus);
            w_sepc(sepc);
        }
        else if(code == 0x42){
            char c;
            uint64 volatile sepc1;
            __asm__ volatile("csrr s1,sepc");
            __asm__ volatile("mv %0,s1" : "=r"(sepc1));
            sepc1 += 4;
            uint64 volatile sstatus = r_sstatus();
            __asm__ volatile("ld s1, 11*8(fp)");
            __asm__ volatile("mv %0,s1" : "=r"(c));
            __putc(c);
            w_sstatus(sstatus);
            w_sepc(sepc1);
        }
    }
    else if(scause == 0x8000000000000001UL){
        mc_sip(SIP_SSIP);
    }
    else if(scause == 0x8000000000000009UL){
        console_handler();
    }
    else{
        uint64  temp;
        temp = r_scause();
        printString("\nScause: ");
        printInt(temp);
        temp = r_sepc();
        printString("\nSepc: ");
        printInt(temp);
        temp = r_stval();
        printString("\nStval: ");
        printInt(temp);
        int i;
        for(i = 0; i< 50000; i++);
    }
}