#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H


class MemoryManager
{
    public:
        MemoryManager();
        ~MemoryManager();
        MemoryManager(const MemoryManager& other);
        MemoryManager& operator=(const MemoryManager& other);
    protected:
    private:
};

#endif // MEMORYMANAGER_H
