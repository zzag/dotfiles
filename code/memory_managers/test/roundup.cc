#include <utils.h>
#include <gtest/gtest.h>


TEST(RoundUpTestCase, SimpleTest) {
    unsigned int value0 = 0;
    unsigned int expected0 = 1;
    unsigned int value1 = 1;
    unsigned int expected1 = 1;
    unsigned int value2 = 2;
    unsigned int expected2 = 2;
    unsigned int value3 = 3;
    unsigned int expected3 = 4;
    unsigned int value4 = 6;
    unsigned int expected4 = 8;
    unsigned int value5 = 555;
    unsigned int expected5 = 1024;
    unsigned int value6 = 16777000;
    unsigned int expected6 = 16777216;


    ASSERT_EQ(roundUp2(value0), expected0);
    ASSERT_EQ(roundUp2(value1), expected1);
    ASSERT_EQ(roundUp2(value2), expected2);
    ASSERT_EQ(roundUp2(value3), expected3);
    ASSERT_EQ(roundUp2(value4), expected4);
    ASSERT_EQ(roundUp2(value5), expected5);
    ASSERT_EQ(roundUp2(value6), expected6);
}
