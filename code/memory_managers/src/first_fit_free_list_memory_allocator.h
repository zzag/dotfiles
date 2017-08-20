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


#ifndef FIRST_FIT_FREE_LIST_MEMORY_ALLOCATOR_H
#define FIRST_FIT_FREE_LIST_MEMORY_ALLOCATOR_H

#include "memory_allocator.h"


class FirstFitFreeListMemoryAllocator : public MemoryAllocator {
public:
    FirstFitFreeListMemoryAllocator(void *buffer, size_t bufferSize);
    FirstFitFreeListMemoryAllocator(size_t totalMemory);

    void *allocate(size_t size) override;
    void free(void *ptr) override;

private:
    // define Node type
    struct Node {
        Node *m_next;
        size_t m_size;
    };
    
    struct AllocationHeader {
        uintptr_t m_start;
        size_t m_size;
    };

    Node *m_root;

private:
    /**
      * Initialize memory allocator
      */
    void initialize();
};


#endif /* FIRST_FIT_FREE_LIST_MEMORY_ALLOCATOR_H */
