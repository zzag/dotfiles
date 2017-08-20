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


#ifndef MONITOR_MEMORY_ALLOCATOR_H
#define MONITOR_MEMORY_ALLOCATOR_H

#include <cstddef>
#include "memory_allocator.h"


class MonitorMemoryAllocator : public MemoryAllocator {
public:
    MonitorMemoryAllocator(MemoryAllocator *allocator);
    virtual ~MonitorMemoryAllocator();


    void *allocate(size_t size) override;
    void free(void *ptr) override;

    size_t getNumberAllocations() const {
        return m_numberAllocations;
    }

    size_t getNumberDeallocations() const {
        return m_numberDeallocations;
    }

    size_t getNumberSuccessfulAllocations() const {
        return m_numberSuccessfulAllocations;
    }

    size_t getNumberFailedAllocations() const {
        return m_numberFailedAllocations;
    }

    size_t getTotalAllocatedMemory() const {
        return m_totalAllocatedMemory;
    }

private:
    // allocator
    MemoryAllocator *m_allocator;

    // statistics
    size_t m_numberAllocations;
    size_t m_numberDeallocations;
    size_t m_numberSuccessfulAllocations;
    size_t m_numberFailedAllocations;
    size_t m_totalAllocatedMemory;
};

#endif /* MONITOR_MEMORY_ALLOCATOR_H */
