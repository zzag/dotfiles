#include "memory_manager.h"


// init instance of current memory manger
MemoryManager *MemoryManager::m_currentManager = nullptr;


MemoryManager::MemoryManager(MemoryAllocator *allocator) {
    m_memoryAllocator = allocator;
}


MemoryManager::~MemoryManager() {
    delete m_memoryAllocator;
}


void *
MemoryManager::allocate(size_t size) {
    if (m_memoryAllocator) {
        return m_memoryAllocator->allocate(size);
    }

    return nullptr;
}


void 
MemoryManager::free(void *ptr) {
    if (m_memoryAllocator) {
        m_memoryAllocator->free(ptr);
    }
}
