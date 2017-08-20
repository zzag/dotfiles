#include "binary_buddy_memory_allocator.h"
#include "utils.h"
#include <cmath>


const uint8_t BUDDY_USAGE_FREE = 0;
const uint8_t BUDDY_USAGE_USED = 1;


BinaryBuddyMemoryAllocator::BinaryBuddyMemoryAllocator(void *buffer, size_t bufferSize, size_t blockSize, size_t order) {
    initialize(buffer, bufferSize, order, blockSize);
}


BinaryBuddyMemoryAllocator::BinaryBuddyMemoryAllocator(size_t blockSize, size_t order) {    
    initialize(nullptr, 0, order, blockSize);
}


void
BinaryBuddyMemoryAllocator::initialize(void *buffer, size_t bufferSize, size_t order, size_t blockSize) {
    // init order
    m_order = order;

    // check block size
    if (blockSize < sizeof(Buddy)) {
        blockSize = sizeof(Buddy);
    }

    m_blockSize = blockSize;
    m_numberBlocks = (1UL << m_order);

    // init total memory size
    if (buffer) {
        m_totalMemory = bufferSize / m_blockSize * m_numberBlocks;
    }
    else {
        m_totalMemory = m_numberBlocks * m_blockSize;
    }
    
    m_availableMemory = m_totalMemory;

    // create memory pool
    if (buffer) {
        m_pool = reinterpret_cast<uintptr_t>(buffer);
    }
    else {
        char *tmp = new char[m_totalMemory];
        m_pool = reinterpret_cast<uintptr_t>(tmp);
    }

    // init head buddies
    m_headBuddy = new Buddy*[m_order + 1];

    for (int i = 0; i < m_order; i++) {
        m_headBuddy[i] = nullptr;
    }

    // init buddy info
    m_blocks = new Block[m_numberBlocks];

    for (int i = 0; i < m_numberBlocks; i++) {
        m_blocks[i].m_order = 0;
        m_blocks[i].m_usage = BUDDY_USAGE_FREE;
    }

    // init buddy with order m_order
    Buddy *buddy = reinterpret_cast<Buddy *>(m_pool);
    buddy->m_next = nullptr;
    m_headBuddy[m_order] = buddy;
}


BinaryBuddyMemoryAllocator::~BinaryBuddyMemoryAllocator() {
    delete[] m_headBuddy;
    delete[] m_blocks;
}


/**
  * Allocate chunk of the memory
  */
void *
BinaryBuddyMemoryAllocator::allocate(size_t size) {
    // get order of the chunk
    unsigned int order = getOrder(size);

    // check order
    if (order > m_order) {
        return nullptr;
    }
    
    for (int i = order; i <= m_order; i++) {
        // is there available buddy?
        if (m_headBuddy[i] == nullptr) {
            continue;
        }

        // save it
        Buddy *retVal = m_headBuddy[i];
        
        // remove from list
        m_headBuddy[i] = retVal->m_next;
        
        // mark as used
        unsigned int blockId = getBlockID(retVal);
        Block *block = &(m_blocks[blockId]);
        block->m_usage = BUDDY_USAGE_USED;
        block->m_order = order;
        
        // split
        while (i > order) {
            i--;
            
            // buddy
            Buddy *buddy = 
                reinterpret_cast<Buddy *>(reinterpret_cast<uintptr_t>(retVal) + (1UL << i) * m_blockSize);
            
            // get block
            blockId = getBlockID(buddy);
            block = &(m_blocks[blockId]);
            
            // set order
            block->m_order = i;
    
            // set usage
            block->m_usage = BUDDY_USAGE_FREE;

            // add to head buddy
            buddy->m_next = m_headBuddy[i];
            m_headBuddy[i] = buddy;
        }
        
        // update statistics
        m_availableMemory -= (1UL << order) * m_blockSize;

        return retVal;
    }

    return nullptr;
}


/**
  * Free chunk of the memory
  */
void
BinaryBuddyMemoryAllocator::free(void *ptr) {
    uintptr_t chunk = reinterpret_cast<uintptr_t>(ptr);
    
    // check range
    if (chunk < m_pool || chunk >= m_pool + m_totalMemory) {
        return;
    }

    // find block
    unsigned int blockId = getBlockID(ptr);
    Block *chunkInfo = &(m_blocks[blockId]);

    // is order valid?
    if (chunkInfo->m_order > m_order) {
        return;
    }

    // is it used?
    if (chunkInfo->m_usage == BUDDY_USAGE_FREE) {
        return;
    }

    // coalesce
    int order = chunkInfo->m_order;
    Block *blockInfo = chunkInfo;
    Buddy *buddy = reinterpret_cast<Buddy *>(ptr);
    
    while (order < m_order) {
        // find buddy
        Buddy *neighborBuddy = findNeighborBuddy(buddy, order);
        
        // get block id
        unsigned int id = getBlockID(neighborBuddy);
        Block *neighborBuddyBlockInfo = &(m_blocks[id]);

        // is it mergeable?
        if (isMergeable(neighborBuddyBlockInfo, order) == false) {
            break;
        }

        // remove from head list @neighborBuddy
        removeBuddyFromHeadBuddyList(neighborBuddy, order);

        if (neighborBuddy < buddy) {
            buddy = neighborBuddy;
            blockInfo = neighborBuddyBlockInfo;
        }

        order++;
        blockInfo->m_order = order;
    }

    // update info
    blockInfo->m_order = order;
    blockInfo->m_usage = BUDDY_USAGE_FREE;

    // add to the head buddy list
    buddy->m_next = m_headBuddy[order];
    m_headBuddy[order] = buddy;

    // update statistics
    m_availableMemory += (1UL << order) * m_blockSize;
}


/**
  * Get number of buddies of given order
  */
size_t 
BinaryBuddyMemoryAllocator::getNumberBuddies(size_t order) {
    // check order
    if (order > m_order) {
        return 0;
    }

    // init return value
    size_t retVal = 0;

    // traverse
    Buddy *node = m_headBuddy[order];

    while (node) {
        retVal++;
        node = node->m_next;
    }

    return retVal;
}


// workers

/**
  * Find neighborhood buddy
  */
BinaryBuddyMemoryAllocator::Buddy *
BinaryBuddyMemoryAllocator::findNeighborBuddy(
        BinaryBuddyMemoryAllocator::Buddy *buddy, size_t order) {
    // convert buddy address to uintptr_t
    uintptr_t neighborBuddy = reinterpret_cast<uintptr_t>(buddy);
    neighborBuddy = neighborBuddy - m_pool;
    
    size_t buddySize = (1UL << order) * m_blockSize;
    neighborBuddy = neighborBuddy ^ buddySize;
    
    /**
      * Also, you can do this
      * 
      * First, you should find out is it left buddy or right buddy
      * Next, if it is left buddy then right buddy has offset buddySize
      *       if it is right buddy then left buddy has offset -buddySize
      *
      * But using XOR is much better
      *

    int isRightBuddy = 
        neighborBuddy & buddySize;

    if (isRightBuddy) {
        // its right buddy
        // return left buddy
        neighborBuddy -= buddySize;
    }
    else {
        // its left buddy
        // return right buddy
        neighborBuddy += buddySize;
    }
    */

    neighborBuddy += m_pool;

    return reinterpret_cast<Buddy *>(neighborBuddy);
}


/**
  * Get block ID from address
  */
unsigned int 
BinaryBuddyMemoryAllocator::getBlockID(void *blockPtr) {
    uintptr_t block = reinterpret_cast<uintptr_t>(blockPtr);
    return (block - m_pool) / m_blockSize;
}


/**
  * Add buddy to the head buddy list
  */
void 
BinaryBuddyMemoryAllocator::addBuddyToHeadBuddyList(
        BinaryBuddyMemoryAllocator::Buddy *buddy, size_t order) {
    buddy->m_next = m_headBuddy[order];
    m_headBuddy[order] = buddy;
}


/**
  * Remove buddy from head buddy list
  */
void
BinaryBuddyMemoryAllocator::removeBuddyFromHeadBuddyList(
        BinaryBuddyMemoryAllocator::Buddy *buddy, size_t order) {
    // init previous node and current node
    Buddy *prev = nullptr;
    Buddy *node = m_headBuddy[order];
    
    // find @buddy
    while (node && node != buddy) {
        node = node->m_next;
    }
    
    // remove it
    if (!prev) { 
        m_headBuddy[order] = node->m_next;
    }
    else {
        prev->m_next = node->m_next;
    }
}


/**
  * Get order
  */
size_t
BinaryBuddyMemoryAllocator::getOrder(size_t size) {
    size_t rounded = roundUp2(size);
    size_t order = rounded / m_blockSize;
    
    order = 
        static_cast<size_t>(std::log2(static_cast<double>(order)));

    return order;
}


/**
  * Is buddy mergeable?
  */
bool 
BinaryBuddyMemoryAllocator::isMergeable(Block *block, size_t order) {
    if (block->m_order != order) {
        return false;
    }

    if (block->m_usage != BUDDY_USAGE_FREE) {
        return false;
    }

    return true;
}
