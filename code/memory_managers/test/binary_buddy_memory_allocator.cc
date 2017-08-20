#include <binary_buddy_memory_allocator.h>
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


TEST(BinaryBuddyTestCase, SimpleTest) {
    // Init allocator
    BinaryBuddyMemoryAllocator *allocator = new BinaryBuddyMemoryAllocator(16 * 1024, 4);
    
    // Create memory manager
    MemoryManager *manager = new MemoryManager(allocator);

    // Init current memory manager
    MemoryManager::setCurrentMemoryManager(manager);

    //
    // Current memory 
    //
    // number - order
    // [] - used memory
    //
    // For example, 4 - order, [0] - used memory
    //
    // +---------------------------------------------------------------+
    // |                         4                                     |
    // +---------------------------------------------------------------+
    //

    // allocate memory
    auto object = new ObjectA;
    //
    // Current memory 
    //
    // +---------------------------------------------------------------+
    // |[0]| 0 |   1   |       2       |               3               |
    // +---------------------------------------------------------------+
    //
    {
        ASSERT_EQ(allocator->getNumberBuddies(0), 1);
        ASSERT_EQ(allocator->getNumberBuddies(1), 1);
        ASSERT_EQ(allocator->getNumberBuddies(2), 1);
        ASSERT_EQ(allocator->getNumberBuddies(3), 1);
        ASSERT_EQ(allocator->getNumberBuddies(4), 0);
        ASSERT_EQ(allocator->getNumberBuddies(5), 0);
    
        ASSERT_EQ(object->coolStuffIsHere(), 10);
        object->setNewData(3, 4);
        ASSERT_EQ(object->coolStuffIsHere(), 7);
    }


    // allocate memory
    auto object2 = new ObjectA;
    //
    // Current memory 
    //
    // +---------------------------------------------------------------+
    // |[0]|[0]|   1   |       2       |               3               |
    // +---------------------------------------------------------------+
    //
    {
        ASSERT_EQ(allocator->getNumberBuddies(0), 0);
        ASSERT_EQ(allocator->getNumberBuddies(1), 1);
        ASSERT_EQ(allocator->getNumberBuddies(2), 1);
        ASSERT_EQ(allocator->getNumberBuddies(3), 1);
        ASSERT_EQ(allocator->getNumberBuddies(4), 0);
        ASSERT_EQ(allocator->getNumberBuddies(5), 0);

        ASSERT_EQ(object2->coolStuffIsHere(), 10);
        object2->setNewData(3, 4);
        ASSERT_EQ(object2->coolStuffIsHere(), 7);
    }


    // allocate memory
    auto object3 = new ObjectA;
    //
    // Current memory 
    //
    // +---------------------------------------------------------------+
    // |[0]|[0]|[0]| 0 |       2       |               3               |
    // +---------------------------------------------------------------+
    //
    {
        ASSERT_EQ(allocator->getNumberBuddies(0), 1);
        ASSERT_EQ(allocator->getNumberBuddies(1), 0);
        ASSERT_EQ(allocator->getNumberBuddies(2), 1);
        ASSERT_EQ(allocator->getNumberBuddies(3), 1);
        ASSERT_EQ(allocator->getNumberBuddies(4), 0);
        ASSERT_EQ(allocator->getNumberBuddies(5), 0);

        ASSERT_EQ(object3->coolStuffIsHere(), 10);
        object3->setNewData(3, 4);
        ASSERT_EQ(object3->coolStuffIsHere(), 7);
    }
 

    // allocate memory
    auto object4 = new ObjectA;
    //
    // Current memory 
    //
    // +---------------------------------------------------------------+
    // |[0]|[0]|[0]|[0]|       2       |               3               |
    // +---------------------------------------------------------------+
    //
    {
        ASSERT_EQ(allocator->getNumberBuddies(0), 0);
        ASSERT_EQ(allocator->getNumberBuddies(1), 0);
        ASSERT_EQ(allocator->getNumberBuddies(2), 1);
        ASSERT_EQ(allocator->getNumberBuddies(3), 1);
        ASSERT_EQ(allocator->getNumberBuddies(4), 0);
        ASSERT_EQ(allocator->getNumberBuddies(5), 0);

        ASSERT_EQ(object4->coolStuffIsHere(), 10);
        object4->setNewData(3, 4);
        ASSERT_EQ(object4->coolStuffIsHere(), 7);
    }


    // allocate memory
    auto object5 = new ObjectA;
    //
    // Current memory 
    //
    // +---------------------------------------------------------------+
    // |[0]|[0]|[0]|[0]|[0]| 0 |   1   |               3               |
    // +---------------------------------------------------------------+
    //
    {
        ASSERT_EQ(allocator->getNumberBuddies(0), 1);
        ASSERT_EQ(allocator->getNumberBuddies(1), 1);
        ASSERT_EQ(allocator->getNumberBuddies(2), 0);
        ASSERT_EQ(allocator->getNumberBuddies(3), 1);
        ASSERT_EQ(allocator->getNumberBuddies(4), 0);
        ASSERT_EQ(allocator->getNumberBuddies(5), 0);

        ASSERT_EQ(object5->coolStuffIsHere(), 10);
        object5->setNewData(3, 4);
        ASSERT_EQ(object5->coolStuffIsHere(), 7);
    }

    // allocate memory
    auto object6 = new ObjectA;
    //
    // Current memory 
    //
    // +---------------------------------------------------------------+
    // |[0]|[0]|[0]|[0]|[0]|[0]|   1   |               3               |
    // +---------------------------------------------------------------+
    //
    {
        ASSERT_EQ(allocator->getNumberBuddies(0), 0);
        ASSERT_EQ(allocator->getNumberBuddies(1), 1);
        ASSERT_EQ(allocator->getNumberBuddies(2), 0);
        ASSERT_EQ(allocator->getNumberBuddies(3), 1);
        ASSERT_EQ(allocator->getNumberBuddies(4), 0);
        ASSERT_EQ(allocator->getNumberBuddies(5), 0);

        ASSERT_EQ(object6->coolStuffIsHere(), 10);
        object6->setNewData(3, 4);
        ASSERT_EQ(object6->coolStuffIsHere(), 7);
    }

    // allocate memory
    auto object7 = new ObjectA;
    //
    // Current memory 
    //
    // +---------------------------------------------------------------+
    // |[0]|[0]|[0]|[0]|[0]|[0]|[0]| 0 |               3               |
    // +---------------------------------------------------------------+
    //
    {
        ASSERT_EQ(allocator->getNumberBuddies(0), 1);
        ASSERT_EQ(allocator->getNumberBuddies(1), 0);
        ASSERT_EQ(allocator->getNumberBuddies(2), 0);
        ASSERT_EQ(allocator->getNumberBuddies(3), 1);
        ASSERT_EQ(allocator->getNumberBuddies(4), 0);
        ASSERT_EQ(allocator->getNumberBuddies(5), 0);

        ASSERT_EQ(object7->coolStuffIsHere(), 10);
        object7->setNewData(3, 4);
        ASSERT_EQ(object7->coolStuffIsHere(), 7);
    }

    // allocate memory
    auto object8 = new ObjectA;
    //
    // Current memory 
    //
    // +---------------------------------------------------------------+
    // |[0]|[0]|[0]|[0]|[0]|[0]|[0]|[0]|               3               |
    // +---------------------------------------------------------------+
    //
    {
        ASSERT_EQ(allocator->getNumberBuddies(0), 0);
        ASSERT_EQ(allocator->getNumberBuddies(1), 0);
        ASSERT_EQ(allocator->getNumberBuddies(2), 0);
        ASSERT_EQ(allocator->getNumberBuddies(3), 1);
        ASSERT_EQ(allocator->getNumberBuddies(4), 0);
        ASSERT_EQ(allocator->getNumberBuddies(5), 0);

        ASSERT_EQ(object8->coolStuffIsHere(), 10);
        object8->setNewData(3, 4);
        ASSERT_EQ(object8->coolStuffIsHere(), 7);
    }

    // allocate 32 KiB
    char *object9 = reinterpret_cast<char *>(allocator->allocate(32 * 1024));
    //
    // Current memory 
    //
    // +---------------------------------------------------------------+
    // |[0]|[0]|[0]|[0]|[0]|[0]|[0]|[0]|  [1]  |   1   |       2       |
    // +---------------------------------------------------------------+
    //
    {
        ASSERT_EQ(allocator->getNumberBuddies(0), 0);
        ASSERT_EQ(allocator->getNumberBuddies(1), 1);
        ASSERT_EQ(allocator->getNumberBuddies(2), 1);
        ASSERT_EQ(allocator->getNumberBuddies(3), 0);
        ASSERT_EQ(allocator->getNumberBuddies(4), 0);
        ASSERT_EQ(allocator->getNumberBuddies(5), 0);
    }

    // allocate 64 KiB
    char *object10 = reinterpret_cast<char *>(allocator->allocate(64 * 1024));
    //
    // Current memory 
    //
    // +---------------------------------------------------------------+
    // |[0]|[0]|[0]|[0]|[0]|[0]|[0]|[0]|  [1]  |   1   |     [2]       |
    // +---------------------------------------------------------------+
    //
    {
        ASSERT_EQ(allocator->getNumberBuddies(0), 0);
        ASSERT_EQ(allocator->getNumberBuddies(1), 1);
        ASSERT_EQ(allocator->getNumberBuddies(2), 0);
        ASSERT_EQ(allocator->getNumberBuddies(3), 0);
        ASSERT_EQ(allocator->getNumberBuddies(4), 0);
        ASSERT_EQ(allocator->getNumberBuddies(5), 0);
    }

    // allocate 32 KiB
    char *object11 = reinterpret_cast<char *>(allocator->allocate(32 * 1024));
    //
    // Current memory 
    //
    // +---------------------------------------------------------------+
    // |[0]|[0]|[0]|[0]|[0]|[0]|[0]|[0]|  [1]  |  [1]  |     [2]       |
    // +---------------------------------------------------------------+
    //
    {
        ASSERT_EQ(allocator->getNumberBuddies(0), 0);
        ASSERT_EQ(allocator->getNumberBuddies(1), 0);
        ASSERT_EQ(allocator->getNumberBuddies(2), 0);
        ASSERT_EQ(allocator->getNumberBuddies(3), 0);
        ASSERT_EQ(allocator->getNumberBuddies(4), 0);
        ASSERT_EQ(allocator->getNumberBuddies(5), 0);
    }

    // no available memory
    // should return null
    auto memNull = allocator->allocate(1);
    ASSERT_EQ(memNull, nullptr);

    allocator->free(object11);
    //
    // Current memory 
    //
    // +---------------------------------------------------------------+
    // |[0]|[0]|[0]|[0]|[0]|[0]|[0]|[0]|  [1]  |  1  |       [2]       |
    // +---------------------------------------------------------------+
    //
    {
        ASSERT_EQ(allocator->getNumberBuddies(0), 0);
        ASSERT_EQ(allocator->getNumberBuddies(1), 1);
        ASSERT_EQ(allocator->getNumberBuddies(2), 0);
        ASSERT_EQ(allocator->getNumberBuddies(3), 0);
        ASSERT_EQ(allocator->getNumberBuddies(4), 0);
        ASSERT_EQ(allocator->getNumberBuddies(5), 0);
    }

    allocator->free(object10);
    //
    // Current memory 
    //
    // +---------------------------------------------------------------+
    // |[0]|[0]|[0]|[0]|[0]|[0]|[0]|[0]|  [1]  |   1   |      2        |
    // +---------------------------------------------------------------+
    //
    {
        ASSERT_EQ(allocator->getNumberBuddies(0), 0);
        ASSERT_EQ(allocator->getNumberBuddies(1), 1);
        ASSERT_EQ(allocator->getNumberBuddies(2), 1);
        ASSERT_EQ(allocator->getNumberBuddies(3), 0);
        ASSERT_EQ(allocator->getNumberBuddies(4), 0);
        ASSERT_EQ(allocator->getNumberBuddies(5), 0);
    }

    allocator->free(object9);
    //
    // Current memory 
    //
    // +---------------------------------------------------------------+
    // |[0]|[0]|[0]|[0]|[0]|[0]|[0]|[0]|               3               |
    // +---------------------------------------------------------------+
    //
    {
        ASSERT_EQ(allocator->getNumberBuddies(0), 0);
        ASSERT_EQ(allocator->getNumberBuddies(1), 0);
        ASSERT_EQ(allocator->getNumberBuddies(2), 0);
        ASSERT_EQ(allocator->getNumberBuddies(3), 1);
        ASSERT_EQ(allocator->getNumberBuddies(4), 0);
        ASSERT_EQ(allocator->getNumberBuddies(5), 0);
    }

    delete object7;
    //
    // Current memory 
    //
    // +---------------------------------------------------------------+
    // |[0]|[0]|[0]|[0]|[0]|[0]| 0 |[0]|               3               |
    // +---------------------------------------------------------------+
    //
    {
        ASSERT_EQ(allocator->getNumberBuddies(0), 1);
        ASSERT_EQ(allocator->getNumberBuddies(1), 0);
        ASSERT_EQ(allocator->getNumberBuddies(2), 0);
        ASSERT_EQ(allocator->getNumberBuddies(3), 1);
        ASSERT_EQ(allocator->getNumberBuddies(4), 0);
        ASSERT_EQ(allocator->getNumberBuddies(5), 0);
    }

    delete object8;
    //
    // Current memory 
    //
    // +---------------------------------------------------------------+
    // |[0]|[0]|[0]|[0]|[0]|[0]|   1   |               3               |
    // +---------------------------------------------------------------+
    //
    {
        ASSERT_EQ(allocator->getNumberBuddies(0), 0);
        ASSERT_EQ(allocator->getNumberBuddies(1), 1);
        ASSERT_EQ(allocator->getNumberBuddies(2), 0);
        ASSERT_EQ(allocator->getNumberBuddies(3), 1);
        ASSERT_EQ(allocator->getNumberBuddies(4), 0);
        ASSERT_EQ(allocator->getNumberBuddies(5), 0);
    }

    delete object5;
    //
    // Current memory 
    //
    // +---------------------------------------------------------------+
    // |[0]|[0]|[0]|[0]| 0 |[0]|   1   |               3               |
    // +---------------------------------------------------------------+
    //
    {
        ASSERT_EQ(allocator->getNumberBuddies(0), 1);
        ASSERT_EQ(allocator->getNumberBuddies(1), 1);
        ASSERT_EQ(allocator->getNumberBuddies(2), 0);
        ASSERT_EQ(allocator->getNumberBuddies(3), 1);
        ASSERT_EQ(allocator->getNumberBuddies(4), 0);
        ASSERT_EQ(allocator->getNumberBuddies(5), 0);
    }

    delete object6;
    //
    // Current memory 
    //
    // +---------------------------------------------------------------+
    // |[0]|[0]|[0]|[0]|        2      |               3               |
    // +---------------------------------------------------------------+
    //
    {
        ASSERT_EQ(allocator->getNumberBuddies(0), 0);
        ASSERT_EQ(allocator->getNumberBuddies(1), 0);
        ASSERT_EQ(allocator->getNumberBuddies(2), 1);
        ASSERT_EQ(allocator->getNumberBuddies(3), 1);
        ASSERT_EQ(allocator->getNumberBuddies(4), 0);
        ASSERT_EQ(allocator->getNumberBuddies(5), 0);
    }


    delete object;
    //
    // Current memory 
    //
    // +---------------------------------------------------------------+
    // | 0 |[0]|[0]|[0]|        2      |               3               |
    // +---------------------------------------------------------------+
    //
    {
        ASSERT_EQ(allocator->getNumberBuddies(0), 1);
        ASSERT_EQ(allocator->getNumberBuddies(1), 0);
        ASSERT_EQ(allocator->getNumberBuddies(2), 1);
        ASSERT_EQ(allocator->getNumberBuddies(3), 1);
        ASSERT_EQ(allocator->getNumberBuddies(4), 0);
        ASSERT_EQ(allocator->getNumberBuddies(5), 0);
    }

    delete object3;
    //
    // Current memory 
    //
    // +---------------------------------------------------------------+
    // | 0 |[0]| 0 |[0]|        2      |               3               |
    // +---------------------------------------------------------------+
    //
    {
        ASSERT_EQ(allocator->getNumberBuddies(0), 2);
        ASSERT_EQ(allocator->getNumberBuddies(1), 0);
        ASSERT_EQ(allocator->getNumberBuddies(2), 1);
        ASSERT_EQ(allocator->getNumberBuddies(3), 1);
        ASSERT_EQ(allocator->getNumberBuddies(4), 0);
        ASSERT_EQ(allocator->getNumberBuddies(5), 0);
    }

    delete object4;
    //
    // Current memory 
    //
    // +---------------------------------------------------------------+
    // | 0 |[0]|   1   |        2      |               3               |
    // +---------------------------------------------------------------+
    //
    {
        ASSERT_EQ(allocator->getNumberBuddies(0), 1);
        ASSERT_EQ(allocator->getNumberBuddies(1), 1);
        ASSERT_EQ(allocator->getNumberBuddies(2), 1);
        ASSERT_EQ(allocator->getNumberBuddies(3), 1);
        ASSERT_EQ(allocator->getNumberBuddies(4), 0);
        ASSERT_EQ(allocator->getNumberBuddies(5), 0);
    }

    delete object2;
    //
    // +---------------------------------------------------------------+
    // |                         4                                     |
    // +---------------------------------------------------------------+
    //
    {
        ASSERT_EQ(allocator->getNumberBuddies(0), 0);
        ASSERT_EQ(allocator->getNumberBuddies(1), 0);
        ASSERT_EQ(allocator->getNumberBuddies(2), 0);
        ASSERT_EQ(allocator->getNumberBuddies(3), 0);
        ASSERT_EQ(allocator->getNumberBuddies(4), 1);
        ASSERT_EQ(allocator->getNumberBuddies(5), 0);
    }
    delete manager;
}
