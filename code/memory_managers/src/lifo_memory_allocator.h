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


#ifndef LIFO_MEMORY_ALLOCATOR_H
#define LIFO_MEMORY_ALLOCATOR_H

#include <cstddef>
#include <cstdint>
#include "memory_allocator.h"


class LifoMemoryAllocator : public MemoryAllocator {
public:
    LifoMemoryAllocator(void *buffer, size_t bufferSize);
    LifoMemoryAllocator(size_t stackSize);

    /**
      * Allocate chunk of the memory
      *
      * \param size: size of the chunk of the memory.
      *
      * Return: on success - pointer to the allocated chunk of the memory,
      *         on failure - nullptr.
      */
    void *allocate(size_t size) override;

    /**
      * Free chunk of the memory
      *
      * \param ptr: pointer to the chunk of the memory.
      *
      * Return: nothing.
      */
    void free(void *ptr) override;

    /**
      * Get top of the "stack"
      *
      * Return: top of the stack.
      */
    size_t topStack() const {
        return m_topStack;
    }

private:
    // define data that is needed to 
    // find previous allocation
    struct Node {
        uintptr_t m_startAddress;
        Node *m_previousAllocation;
    };

    Node *m_previousAllocation;

    // stack grows from bottom to up!
    // 0 - bottom
    // bufferSize-1 - top
    uintptr_t m_topStack;

private:
    /**
      * Initialize memory allocator
      *
      * \param buffer: memory pool,
      *                nullptr - create own pool.
      * \param bufferSize - size of the buffer.
      *
      * Return: nothing.
      */
    void initialize(void *buffer, size_t bufferSize);
};

#endif /* LIFO_MEMORY_ALLOCATOR_H */
