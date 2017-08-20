#include "first_fit_free_list_memory_allocator.h"
#include "utils.h"


FirstFitFreeListMemoryAllocator::FirstFitFreeListMemoryAllocator(void *buffer, size_t bufferSize) {
    m_pool = reinterpret_cast<uintptr_t>(buffer);
    m_totalMemory = bufferSize;
    m_availableMemory = m_totalMemory;

    initialize();
}


FirstFitFreeListMemoryAllocator::FirstFitFreeListMemoryAllocator(size_t totalMemory) {
    // init pool
    char *tmp = new char[totalMemory];
    m_pool = reinterpret_cast<uintptr_t>(tmp);
    m_totalMemory = totalMemory;
    m_availableMemory = m_totalMemory;

    initialize();
}


void
FirstFitFreeListMemoryAllocator::initialize() {
    // init memory allocator
    m_root = reinterpret_cast<Node *>(m_pool);
    m_root->m_next = nullptr;
    m_root->m_size = m_totalMemory;
}


void *
FirstFitFreeListMemoryAllocator::allocate(size_t size) {

    if (m_availableMemory < size || size == 0) {
        return nullptr;
    }
    
    // find first fit free memory chunk
    Node *node = m_root;
    Node *prev = nullptr;
    size_t adjustment = 0;
    size_t totalAllocateMemory = 0;
    
    while (node) {
        // calculate align address adjustment
        adjustment = 
            alignAddressAdjustment(node, sizeof(AllocationHeader));
        
        // if its size is less than needed size than
        // go to next free chunk
        totalAllocateMemory = size + adjustment;
        if (node->m_size < totalAllocateMemory) {
            prev = node;
            node = node->m_next;
            continue;
        }
        
        // found
        // exit from loop
        break;
    }
    
    // if there is no such memory chunk 
    // just return nullptr
    if (!node) {
        return nullptr;
    }
    
    // create new free node
    // and replace old free node
    Node *newFreeNode = nullptr;
    size_t newFreeNodeSize = node->m_size - totalAllocateMemory;
    
    if (newFreeNodeSize > 0) {
        newFreeNode = pointerOffset(node, totalAllocateMemory);
        newFreeNode->m_next = node->m_next;
        newFreeNode->m_size = newFreeNodeSize;
    }
    
    // remove old free node
    if (prev) {
        prev->m_next = newFreeNode ? newFreeNode : node->m_next;
    }
    else {
        // replace root
        m_root = !newFreeNode && node->m_next ? node->m_next : newFreeNode;
    }
    
    // init ret value
    void *retVal = pointerOffset(node,  adjustment);
    // initialize AllocationHeader
    AllocationHeader *allocationHeader = 
        reinterpret_cast<AllocationHeader *>(reinterpret_cast<uintptr_t>(retVal) - sizeof(AllocationHeader));
    allocationHeader->m_start = reinterpret_cast<uintptr_t>(node);
    allocationHeader->m_size = totalAllocateMemory;
    
    // update statistics
    m_availableMemory -= totalAllocateMemory;
    
    return retVal;
}


void 
FirstFitFreeListMemoryAllocator::free(void *ptr) {
    uintptr_t chunk = reinterpret_cast<uintptr_t>(ptr);

    // check range
    if (m_pool > chunk || chunk >= m_pool + m_totalMemory) {
        return;
    }

    // chunk range
    AllocationHeader *allocationHeader =
        reinterpret_cast<AllocationHeader *>(chunk - sizeof(AllocationHeader));
    Node *chunkStart = reinterpret_cast<Node *>(allocationHeader->m_start);
    size_t chunkSize = allocationHeader->m_size;
    Node *chunkEnd = reinterpret_cast<Node *>(allocationHeader->m_start + chunkSize);

    // find position where chunk of the memory
    // will be inserted
    Node *prev = nullptr;
    Node *node = m_root;

    while (node) {
        if (node >= chunkEnd) {
            break;
        }

        prev = node;
        node = node->m_next;
    }

    if (!prev) {
        // insert free node at beginning
        chunkStart->m_size = chunkSize;
        chunkStart->m_next = m_root;

        // replace root
        m_root = chunkStart;

        // needed to merge
        prev = m_root;
    }
    else if (pointerOffset(prev, prev->m_size) == chunkStart) {
        // append to the previous
        // free list node
        prev->m_size += chunkSize;
    }
    else {
        // insert between prev and prev->m_next
        chunkStart->m_size = chunkSize;
        chunkStart->m_next = prev->m_next;
        prev->m_next = chunkStart;

        // needed to make merge
        prev = chunkStart;
    }

    // merge previous node and current node
    if (node && node == chunkEnd) {
        //
        // ...[prev] [node] ...
        //
        prev->m_size += node->m_size;
        prev->m_next = node->m_next;
    }

    // update statistics
    m_availableMemory += chunkSize;
}
