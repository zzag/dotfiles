#include "utils.h"



size_t
__alignAddressAdjustment(void *address, size_t alignment) {
    size_t adjustment = alignment - ((uintptr_t)address & (alignment - 1));

    if (adjustment == alignment) {
        return 0;
    }
    
    return adjustment;
}


size_t 
__alignAddressAdjustment(void *address, size_t alignment, size_t headerSize) {
    // get align adjustment for main "container"
    size_t adjustment = __alignAddressAdjustment(address, alignment);

    // resize adjustment if headerSize is bigger than adjustment
    if (adjustment < headerSize) {
        //
        //  || - aligned address
        //
        //  ][ - header edge
        //
        //
        //  ||][ adjustment || delta ][ ... || ...
        //
        //
        size_t delta = headerSize - adjustment;
        void *tmpAddress = (void *)((uintptr_t)address + headerSize);
        adjustment += delta + __alignAddressAdjustment(tmpAddress, alignment);
    }

    return adjustment;
}


/**
  * Get align address adjustment
  */
size_t
alignAddressAdjustment(
        void *ptr, size_t headerSize) {
#ifdef MM_X86
    // x86
    return __alignAddressAdjustment(ptr, 4, headerSize);
#elif MM_X86_64
    // x64
    return __alignAddressAdjustment(ptr, 16, headerSize);
#else
#error Unknown platform
#endif
}


/**
  * Round up
  */
unsigned int
roundUp2(unsigned int value) {
    // 0, next 2^0 = 1
    if (!value) {
        return 1;
    }

    // is it power of 2?
    if (!(value & (value - 1))) {
        return value;
    }

    value--;
    value |= value >> 1;
    value |= value >> 2;
    value |= value >> 4;
    value |= value >> 8;
    value |= value >> 16;
    value++;

    return value;
}
