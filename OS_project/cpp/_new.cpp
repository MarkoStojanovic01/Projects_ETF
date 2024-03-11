//
// Created by os on 9/4/22.

#include "../h/MemoryAllocator.hpp"

using size_t = decltype(sizeof(0));


void *operator new[](size_t size)
{
    return MemoryAllocator::getInstance()->kmalloc(size);
}

void operator delete[](void *p) noexcept
{
    MemoryAllocator::getInstance()->kfree(p);
}
//


