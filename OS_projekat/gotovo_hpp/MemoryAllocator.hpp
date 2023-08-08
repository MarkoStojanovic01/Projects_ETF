#ifndef PROJECT_BASE_MEMORYALLOCATOR_HPP
#define PROJECT_BASE_MEMORYALLOCATOR_HPP

#include "../lib/hw.h"


class MemoryAllocator {


protected:



public:

    typedef struct freeMem{
        freeMem* next;
        size_t size;
    } FreeMem;

    static MemoryAllocator* getInstance();

    void* kmalloc(size_t size);

    int kfree(void* adr);


private:
    MemoryAllocator(){
    };

    void tryToJoin(FreeMem* cur);

    FreeMem* freeMemHead;

    static MemoryAllocator* instance;

};


#endif //PROJECT_BASE_MEMORYALLOCATOR_HPP