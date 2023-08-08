#include "../h/MemoryAllocator.hpp"

MemoryAllocator* MemoryAllocator::instance = nullptr;

void *MemoryAllocator::kmalloc(size_t size) {
    FreeMem* cur = freeMemHead, *prev = nullptr;
    size_t realSize = ((size/MEM_BLOCK_SIZE) + (size%MEM_BLOCK_SIZE?1:0) )*MEM_BLOCK_SIZE;
    for (; cur != nullptr; prev = cur, cur = cur->next)
        if(cur->size >= realSize) break;

    if (cur == nullptr) return nullptr;

    size_t remaining = cur->size - realSize;
    if (remaining >= sizeof(FreeMem)/8 + MEM_BLOCK_SIZE) {
        cur->size = realSize;
        size_t offset = sizeof(FreeMem) + 8*realSize;
        FreeMem *newFreeMem = (FreeMem *) ((char *) cur + offset);
        if (prev) prev->next = newFreeMem;
        else freeMemHead = newFreeMem;
        newFreeMem->next = cur->next;
        newFreeMem->size = remaining - sizeof(FreeMem)/8;
    }
    else {
        if (prev) prev->next = cur->next;
        else freeMemHead = cur->next;
    }
    cur->next = nullptr;
    return (char*)cur + sizeof(FreeMem);
}

MemoryAllocator *MemoryAllocator::getInstance() {
    if(instance == nullptr) {
        instance = (MemoryAllocator*) HEAP_START_ADDR;
        size_t offset = sizeof(MemoryAllocator);
        instance->freeMemHead = (MemoryAllocator::FreeMem *) ((char*)HEAP_START_ADDR + offset);
        instance->freeMemHead->next = nullptr;
        instance->freeMemHead->size = ((size_t) HEAP_END_ADDR - (size_t) HEAP_START_ADDR)/8
                                      - (size_t) sizeof(FreeMem) - (size_t) sizeof(MemoryAllocator);
    }
    return instance;
}

int MemoryAllocator::kfree(void* adr) {
    FreeMem* cur = 0;
    if(!freeMemHead || adr<(char*)freeMemHead)
        cur = 0;
    else {
        for (cur = freeMemHead; cur->next != 0 && adr > (char *) (cur->next); cur = cur->next);
    }
    char* realAdr = (char*)adr - sizeof(freeMem);
    FreeMem* newSeg = (FreeMem*)realAdr;
    if(cur) newSeg->next = cur->next;
    else newSeg->next = freeMemHead;
    if(cur) cur->next = newSeg;
    else freeMemHead = newSeg;
    adr = 0;
    tryToJoin(newSeg);
    tryToJoin(cur);


    return 0;
}

void MemoryAllocator::tryToJoin(MemoryAllocator::FreeMem *cur) {
    if(!cur) return;
    size_t betwn = (char*)(cur->next) - (char*)cur - 8*cur->size - sizeof(FreeMem);
    if(cur->next && (char*)(cur->next) - (char*)cur - 8*cur->size - sizeof(FreeMem)
                    < sizeof(MemoryAllocator::FreeMem) + 8*MEM_BLOCK_SIZE){
        cur->size += cur->next->size + sizeof(MemoryAllocator::FreeMem)/8 + betwn/8;
        cur->next = cur->next->next;
    }
}


