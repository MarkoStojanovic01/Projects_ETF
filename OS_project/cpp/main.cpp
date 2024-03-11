#include "../h/printing.hpp"
#include "../h/kThread.hpp"
#include "../h/Riscv.hpp"
#include "../h/syscall_cpp.hpp"

extern void userMain(void* arg);


int main(){

    Riscv::w_stvec((uint64) &Riscv::supervisorTrap);

    /*
    Thread* threads[2];

    threads[0] = new Thread(nullptr, nullptr);
    threads[1] = new Thread(userMain, nullptr);
    printString("ThreadUserMain created\n");
    threads[1]->start();
    while(!threads[1]->myHandle->isFinished()){
        thread_dispatch();
    }
     */

    thread_t threads[2];

    thread_create(&threads[0],nullptr,nullptr);

    thread_create(&threads[1],userMain,nullptr);
    printString("ThreadUserMain created\n");

    while(!threads[1]->isFinished()){
        thread_dispatch();
    }


    /*
    __asm__ volatile("csrr s1,sepc");

    printString("Sistemski rezim!!!\n");
    */
    return 0;
}