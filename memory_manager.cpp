#include "memory_manager.h"

MemoryManager::MemoryManager()
{
    //ctor
}

MemoryManager::~MemoryManager()
{
    //dtor
}

MemoryManager::MemoryManager(const MemoryManager& other)
{
    //copy ctor
}

MemoryManager& MemoryManager::operator=(const MemoryManager& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    return *this;
}

