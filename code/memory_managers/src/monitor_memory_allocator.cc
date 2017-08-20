#include "monitor_memory_allocator.h"


MonitorMemoryAllocator::MonitorMemoryAllocator(
        MemoryAllocator *allocator) {
    // init allocator
    m_allocator = allocator;

    // init statistics
    m_numberAllocations = 0;
    m_numberDeallocations = 0;
    m_numberSuccessfulAllocations = 0;
    m_numberFailedAllocations = 0;
    m_totalAllocatedMemory = 0;
}


MonitorMemoryAllocator::~MonitorMemoryAllocator() {
    delete m_allocator;
}


/**
  * Allocate memory
  */
void *
MonitorMemoryAllocator::allocate(size_t size) {
    // allocate memory
    void *retVal = m_allocator->allocate(size);

    // update statistics
    m_numberAllocations++;

    if (retVal) {
        m_numberSuccessfulAllocations++;
        m_totalAllocatedMemory += size;
    }
    else {
        m_numberFailedAllocations++;
    }

    return retVal;
}


/**
  * Free memory
  */
void
MonitorMemoryAllocator::free(void *ptr) {
    // free memory
    m_allocator->free(ptr);

    // update statistics
    m_numberDeallocations++;
}
