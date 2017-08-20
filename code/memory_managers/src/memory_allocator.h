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


#ifndef MEMORY_ALLOCATOR_H
#define MEMORY_ALLOCATOR_H

#include <cstddef>
#include <cstdint>


class MemoryAllocator {
public:
    /**
      * Init
      */
    MemoryAllocator(uintptr_t pool = 0, size_t totalMemory = 0) {
        m_pool = pool;
        m_totalMemory = totalMemory;
        m_availableMemory = m_totalMemory;
    }

    /**
      * Dispose
      */
    virtual ~MemoryAllocator() {
        if (m_pool) {
            char *temp = 
                reinterpret_cast<char *>(m_pool);
            delete temp;
        }
    }

    /**
      * Allocate chunk of the memory
      */
    virtual void *allocate(size_t size) = 0;

    /**
      * Free chunk of the memory
      */
    virtual void free(void *ptr) = 0;

    /**
      * Get total memory
      */
    size_t getTotalMemory() {
        return m_totalMemory;
    }

    /**
      * Get available memory
      */
    size_t getAvailableMemory() {
        return m_availableMemory;
    }

    /**
      * Get used memory
      */
    size_t getUsedMemory() {
        return m_totalMemory - m_availableMemory;
    }

protected:
    uintptr_t m_pool;
    size_t m_totalMemory;
    size_t m_availableMemory;
};

#endif /* MEMORY_ALLOCATOR_H */
