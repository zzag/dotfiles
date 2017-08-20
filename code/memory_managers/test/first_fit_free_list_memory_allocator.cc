#include <first_fit_free_list_memory_allocator.h>
#include <memory_manager.h>
#include <utils.h>
#include <object.h>
#include <gtest/gtest.h>


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


class ObjectB : public Object {
public:
    ObjectB () {
        m_a = 5;
    }

    int coolStuffIsHereToo(int magicNumber) {
        return m_a * magicNumber;
    }

private:
    int m_a;
};


TEST(FirstFitMemoryManagerTestCase, SimpleTest) {
    // create allocator
    MemoryAllocator *allocator = new FirstFitFreeListMemoryAllocator(1024); // 1 KiB
    MemoryManager::setCurrentMemoryManager(new MemoryManager(allocator));

    // save initial object
    // should be the same in the end
    auto initialObject = new ObjectA;
    delete initialObject;

    //
    {
        // get memory
        auto object1 = new ObjectA;
        auto object2 = new ObjectB;
        auto object3 = new ObjectB;
        auto object4 = new ObjectA;
        auto object5 = new ObjectB;
        auto object6 = new ObjectA;
        auto object7 = new ObjectA;
        auto object8 = new ObjectA;
        auto object9 = new ObjectB;
        auto object10= new ObjectB;

        // free memory
        delete object5;
        delete object1;
        delete object2;
        delete object4;
        delete object3;
        delete object10;
        delete object7;
        delete object6;
        delete object9;
        delete object8;

        object4 = new ObjectA;
        ASSERT_EQ(object1, object4);
        delete object4;
    }

    // test simple allocation
    {
        ObjectA *object = new ObjectA;
        int value = object->coolStuffIsHere();
        ASSERT_EQ(value, 10);

        object->setNewData(11, 9);
        ASSERT_EQ(object->coolStuffIsHere(), 20);

        delete object;
    }

    // check alignment
    {
        ObjectA *objectA = new ObjectA;
        size_t adjustmentA = alignAddressAdjustment(objectA);
        ASSERT_EQ(adjustmentA, 0);

        ObjectB *objectB = new ObjectB;
        size_t adjustmentB = alignAddressAdjustment(objectB);
        ASSERT_EQ(adjustmentB, 0);

        delete objectB;
        delete objectA;
    }

    // test initial object
    auto endObject = new ObjectA;
    delete endObject;
    ASSERT_EQ(initialObject, endObject);
}
