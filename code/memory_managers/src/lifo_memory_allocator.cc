#include "lifo_memory_allocator.h"
#include "utils.h"


LifoMemoryAllocator::LifoMemoryAllocator(
        void *buffer, size_t bufferSize) {
    initialize(buffer, bufferSize);
}


LifoMemoryAllocator::LifoMemoryAllocator(
        size_t stackSize) {
    initialize(nullptr, stackSize);
}


/**
  * Initialize LIFO memory allocator
  */
void
LifoMemoryAllocator::initialize(void *buffer, size_t bufferSize) {
    // init pool
    if (!buffer) {
        buffer = reinterpret_cast<void*>(new char[bufferSize]);
    }

    m_pool = reinterpret_cast<uintptr_t>(buffer);

    // init memory statistics
    m_totalMemory = bufferSize;
    m_availableMemory = m_totalMemory;

    // init previous allocation data
    m_previousAllocation = nullptr;
    m_topStack = 0;
}


/**
  * Allocate memory
  */
void *
LifoMemoryAllocator::allocate(size_t size) {
    // get memory align adjustment
    size_t adjustment = alignAddressAdjustment(
            reinterpret_cast<void *>(m_pool + m_topStack), sizeof(Node));

    if (m_availableMemory < size + adjustment) {
        return nullptr;
    }

    // init ret val
    uintptr_t retVal = m_pool + m_topStack + adjustment;
    
    // init previous allocation data
    Node *previousAllocation = 
        reinterpret_cast<Node *>(retVal - sizeof(Node));
    previousAllocation->m_startAddress = m_pool + m_topStack;
    previousAllocation->m_previousAllocation = m_previousAllocation;

    m_previousAllocation = previousAllocation;

    // move top of the stack
    size_t allocationNeededSize = size + adjustment;
    m_topStack += allocationNeededSize;

    // update statistics
    m_availableMemory -= allocationNeededSize;

    return reinterpret_cast<void *>(retVal);
}


/**
  * Free allocated memory
  */
void
LifoMemoryAllocator::free(void *ptr) {
    uintptr_t chunk = reinterpret_cast<uintptr_t>(ptr);
    chunk -= sizeof(Node);
    Node *allocationData = reinterpret_cast<Node *>(chunk);

    // is it previous allocation?
    if (m_previousAllocation != allocationData) {
        return;
    }

    // update statistics
    m_availableMemory += m_topStack - allocationData->m_startAddress;

    // move top of the stack down
    m_topStack = allocationData->m_startAddress - m_pool;

    // change previous allocation
    m_previousAllocation = m_previousAllocation->m_previousAllocation;
}
