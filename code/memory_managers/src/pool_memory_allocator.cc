#include "pool_memory_allocator.h"


PoolMemoryAllocator::PoolMemoryAllocator(
        void *buffer, size_t objectSize, size_t numberObjects) {
    initialize(buffer, objectSize, numberObjects);
}


PoolMemoryAllocator::PoolMemoryAllocator(
        size_t objectSize, size_t numberObjects) {
    initialize(nullptr, objectSize, numberObjects);
}


/**
  * Initialize memory pool
  */
void
PoolMemoryAllocator::initialize(
        void *buffer, size_t objectSize, size_t numberObjects) {
    if (objectSize < sizeof(Node)) {
        objectSize = sizeof(Node);
    }

    // init object info
    m_objectSize = objectSize;
    m_numberObjects = numberObjects;

    // init total memory size
    m_totalMemory = m_objectSize * m_numberObjects;
    m_availableMemory = m_totalMemory;

    // init pool
    if (buffer) {
        m_pool = reinterpret_cast<uintptr_t>(buffer);
    }
    else {
        char *tmp = new char[m_totalMemory];
        m_pool = reinterpret_cast<uintptr_t>(tmp);
    }

    // init objects
    Node *prev = reinterpret_cast<Node *>(m_pool);
    prev->m_next = nullptr;
    m_freeObjects = prev;

    for (size_t i = 1; i < m_numberObjects; i++) {
        Node *node = reinterpret_cast<Node *>(m_pool + i * m_objectSize);
        prev->m_next = node;
        node->m_next = nullptr;

        prev = node;
    }
}


/**
  * Allocate chunk of the memory
  */
void *
PoolMemoryAllocator::allocate(size_t size) {

    // check size
    if (size > m_objectSize) {
        return nullptr;
    }

    // is there available memory?
    if (!m_freeObjects) {
        return nullptr;
    }

    // get top of the free list, it is return value
    Node *retVal = m_freeObjects;

    // remove it from free list
    m_freeObjects = retVal->m_next;

    // update statistics
    m_availableMemory -= m_objectSize;

    return retVal;
}


/**
  * Free chunk of the memory
  */
void
PoolMemoryAllocator::free(void *ptr) {
    // check range
    uintptr_t address = reinterpret_cast<uintptr_t>(ptr);

    if (address < m_pool || address >= m_pool + m_totalMemory) {
        return;
    }

    // ok, cast it to Node
    Node *node = reinterpret_cast<Node *>(ptr);

    // prepend it to free list
    node->m_next = m_freeObjects;
    m_freeObjects = node;

    // update statistics
    m_availableMemory += m_objectSize;
}
