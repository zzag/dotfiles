#include <pool_memory_allocator.h>
#include <memory_manager.h>
#include <gtest/gtest.h>


TEST(PoolTestCase, SimpleTest) {
    // init memory allocator
    PoolMemoryAllocator *allocator = new PoolMemoryAllocator(1024, 5);

    // init memory manager
    MemoryManager *manager = new MemoryManager(allocator);
    MemoryManager::setCurrentMemoryManager(manager);

    {
        auto buffer = allocator->allocate(1024);

        ASSERT_NE(buffer, nullptr);
        ASSERT_EQ(allocator->getAvailableMemory(), 4096);

        allocator->free(buffer);
    }

    {
        // allocate memory
        auto buffer0 = allocator->allocate(1024);
        auto buffer1 = allocator->allocate(4096);
        auto buffer2 = allocator->allocate(1024);
        auto buffer3 = allocator->allocate(1024);
        auto buffer4 = allocator->allocate(1024);
        auto buffer5 = allocator->allocate(1024);
        auto buffer6 = allocator->allocate(1024);

        // test
        ASSERT_NE(buffer0, nullptr);
        ASSERT_EQ(buffer1, nullptr);
        ASSERT_NE(buffer2, nullptr);
        ASSERT_NE(buffer3, nullptr);
        ASSERT_NE(buffer4, nullptr);
        ASSERT_NE(buffer5, nullptr);
        ASSERT_EQ(buffer6, nullptr);

        // free
        allocator->free(buffer6);
        allocator->free(buffer5);
        allocator->free(buffer4);
        allocator->free(buffer3);
        allocator->free(buffer2);
        allocator->free(buffer1);
        allocator->free(buffer0);
    }

    delete manager;
}
