/**
  * Copyright (c) 2015, Vlad Zagorodniy
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without
  * modification, are permitted provided that the following conditions are met:
  *    * Redistributions of source code must retain the above copyright
  *      notice, this list of conditions and the following disclaimer.
  *    * Redistributions in binary form must reproduce the above copyright
  *      notice, this list of conditions and the following disclaimer in the
  *      documentation and/or other materials provided with the distribution.
  *    * Neither the name of the author nor the
  *      names of its contributors may be used to endorse or promote products
  *      derived from this software without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY
  * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  */


#ifndef BINARY_BUDDY_MEMORY_ALLOCATOR_H
#define BINARY_BUDDY_MEMORY_ALLOCATOR_H

#include <cstdint>
#include "memory_allocator.h"


//
// Possible improvemnts:
//  -> use bitmaps to indicate free blocks
//  -> restrict max order
//


class BinaryBuddyMemoryAllocator : public MemoryAllocator {
public:
    BinaryBuddyMemoryAllocator(void *buffer, size_t bufferSize, size_t blockSize, size_t order);
    BinaryBuddyMemoryAllocator(size_t blockSize, size_t order);
    virtual ~BinaryBuddyMemoryAllocator();

    /**
      * Allocate chunk of the memory
      *
      * \param size: size of the chunk of the memory.
      *
      * Return: on success - pointer to the chunk of the memory, 
      *         otherwise - nullptr.
      */
    void *allocate(size_t size) override;

    /**
      * Free chunk of the memory
      *
      * \param ptr: pointer to the chunk of the memrory.
      *
      * Return: nothing.
      */
    void free(void *ptr) override;

    /**
      * Get number of the buddies of given order
      * Use it for debug.
      *
      * \param order: given order.
      *
      * Return: number of the buddies,
      *         on invalid order - 0.
      */
    size_t getNumberBuddies(size_t order);
    
private:
    // Buddy
    struct Buddy {
        Buddy *m_next;
    };

    // Block
    struct Block {
        size_t m_order;
        uint8_t m_usage;
    };

    // head buddy list
    Buddy **m_headBuddy;

    // blocks
    Block *m_blocks;

    // other infos
    size_t m_blockSize;
    size_t m_order;
    size_t m_numberBlocks;

private:
    /**
      * Initialize memory allocator
      *
      * \param buffer: memory pool,
      *                if it nullptr - create own buffer.
      * \param bufferSize: size of the buffer.
      * \param order: max order.
      * \param blockSize: block size.
      *
      * Return: nothing.
      *
      */
    void initialize(void *buffer, size_t bufferSize, size_t order, size_t blockSize);

    /**
      * Find neighborhood buddy for given buddy
      *
      * In other words, if given left buddy then it returns right buddy,
      *                 if given right buddy then it returns left buddy.
      *
      * \param buddy: buddy :)
      *
      * Return: neighborhood budy.
      */
    inline Buddy *findNeighborBuddy(Buddy *buddy, size_t order);

    /**
      * Get block ID for given address
      *
      * \param blockPtr: address of a block.
      *
      * Return: block id.
      */
    inline unsigned int getBlockID(void *blockPtr);

    /**
      * Prepend buddy to head buddy list(m_headBuddy)
      *
      * \param buddy: buddy that will be prepended.
      *
      */
    inline void addBuddyToHeadBuddyList(Buddy *buddy, size_t order);

    /**
      * Remove buddy from head buddy list
      *
      * \param buddy: buddy that will be removed from head buddy list.
      * \param order: order of the buddy.
      *
      */
    inline void removeBuddyFromHeadBuddyList(Buddy *buddy, size_t order);

    /**
      * Get order of given size
      *
      * \param size: given size.
      *
      * Return: order.
      */
    inline size_t getOrder(size_t size);

    /**
      * Is it block mergeable
      *
      * \param block: block of the buddy.
      * \param order: order that should have block.
      *
      * Return: on mergeable - true,
      *         otherwise - false.
      */
    inline bool isMergeable(Block *block, size_t order);
};


#endif /* BINARY_BUDDY_MEMORY_ALLOCATOR_H */
