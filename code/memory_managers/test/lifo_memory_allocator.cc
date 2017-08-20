#include <lifo_memory_allocator.h>
#include <memory_manager.h>
#include <object.h>
#include <gtest/gtest.h>


// sample object
class ObjectA : public Object {
public:
    ObjectA () {
        m_a = 1;
        m_b = 9;
    }

    int coolStuffIsHere () {
        return m_a + m_b;
    }

    void setNewData(int a, int b) {
        m_a = a;
        m_b = b;
    }

private:
    int m_a;
    int m_b;
};


TEST(LIFOMemoryAllocatorTestCase, SimpleTest) {
    // init memory allocator
    LifoMemoryAllocator *allocator =
        new LifoMemoryAllocator(5 * 1024);

    // init memory manager
    MemoryManager *manager = new MemoryManager(allocator);
    MemoryManager::setCurrentMemoryManager(manager);

    {
        size_t initialTopStack = allocator->topStack();
        
        auto object0 = new ObjectA;
        size_t topStack0 = allocator->topStack();

        auto object1 = new ObjectA;
        size_t topStack1 = allocator->topStack();

        auto object2 = new ObjectA;
        size_t topStack2 = allocator->topStack();

        delete object1;
        ASSERT_EQ(allocator->topStack(), topStack2);

        delete object2;
        ASSERT_EQ(allocator->topStack(), topStack1);

        delete object1;
        ASSERT_EQ(allocator->topStack(), topStack0);

        delete object0;
        ASSERT_EQ(allocator->topStack(), initialTopStack);
    }
}
