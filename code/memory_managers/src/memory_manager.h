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


#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include "memory_allocator.h"


class MemoryManager {
public:
    MemoryManager(MemoryAllocator *allocator);
    ~MemoryManager();

    /**
      * Allocate chunk of the memory
      *
      * \param size: size of the chunk.
      *
      * Return:
      *  Success: pointer to the chunk.
      *  Failure: nullptr.
      *
      */
    void *allocate(size_t size);

    /**
      * Free chunk of the memory
      *
      * \param ptr: pointer to the chunk.
      *
      */
    void free(void *ptr);

    /**
      * Set current memory manager
      *
      */
    static void setCurrentMemoryManager(MemoryManager *manager) {
        m_currentManager = manager;
    }
    
    /**
      * Get current memory manager
      *
      */
    static MemoryManager *instance() {
        return m_currentManager;
    }

private:
    static MemoryManager *m_currentManager;
    MemoryAllocator *m_memoryAllocator;
};


#endif /* MEMORY_MANAGER_H */
