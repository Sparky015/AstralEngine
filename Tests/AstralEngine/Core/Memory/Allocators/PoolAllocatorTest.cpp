//
// Created by Andrew Fagan on 1/26/25.
//

#include <gtest/gtest.h>

#include "Core/Memory/Allocators/PoolAllocator.h"
#include <cstring>
#include <unordered_set>

struct TestStructOne
{
    TestStructOne() : a(0), b(1), c(2.5f) {}
    TestStructOne(int a, int b, int c) : a(a), b(b), c(c) {}
    int a;
    int b;
    float c;
};

class PoolAllocatorTest : public ::testing::Test
{
public:
    static constexpr int DEFAULT_ALLOCATION_SIZE = 3;
    Core::PoolAllocator<TestStructOne, DEFAULT_ALLOCATION_SIZE> testAllocator;
};

TEST_F(PoolAllocatorTest, Allocate_ReturnsNullptrWhenOutOfSpace)
{
    // Default pool allocator holds 3 instances of TestStruct
    TestStructOne* testStructPtr = testAllocator.Allocate();
    TestStructOne* testStructPtr2 = testAllocator.Allocate();
    TestStructOne* testStructPtr3 = testAllocator.Allocate();
    TestStructOne* testStructPtr4 = testAllocator.Allocate();

    EXPECT_NE(testStructPtr, nullptr);
    EXPECT_NE(testStructPtr2, nullptr);
    EXPECT_NE(testStructPtr3, nullptr);

    EXPECT_EQ(testStructPtr4, nullptr);
}


TEST_F(PoolAllocatorTest, Allocate_ResetsInstanceOnAllocate)
{
    // Default pool allocator holds 3 instances of TestStruct
    TestStructOne* testStructPtr = testAllocator.Allocate();
    TestStructOne* testStructPtr2 = testAllocator.Allocate();
    TestStructOne* testStructPtr3 = testAllocator.Allocate();

    // testAllocator now has no more instances to allocate

    TestStructOne differentTestStruct;
    differentTestStruct.a = 12;
    differentTestStruct.b = 13;
    differentTestStruct.c = 13.5f;

    // Set the first test struct ptr to something different from the default constructed instance
    *testStructPtr = differentTestStruct;

    // Free only one instance
    testAllocator.Free(testStructPtr);
    testStructPtr = nullptr;

    // Now only one instance is available and it was the previous instance freed
    TestStructOne* newTestStructPtr = testAllocator.Allocate();

    // Now compare the instance to see if it is still the values form differentTestStruct. They should be different
    // because the instance should be reset.
    EXPECT_NE(newTestStructPtr->a, differentTestStruct.a);
    EXPECT_NE(newTestStructPtr->b, differentTestStruct.b);
    EXPECT_NE(newTestStructPtr->c, differentTestStruct.c);
}

TEST_F(PoolAllocatorTest, Free_CanFreeInAnyOrder)
{
    // Default pool allocator holds 3 instances of TestStruct
    TestStructOne* testStructPtr = testAllocator.Allocate(1, 2, 3.0f);
    TestStructOne* testStructPtr2 = testAllocator.Allocate(5, 3, 6.0f);
    TestStructOne* testStructPtr3 = testAllocator.Allocate(2, 9, 8.0f);

    testAllocator.Free(testStructPtr);
    // Should be able to allocate in freed slot
    TestStructOne* testStructPtr4 = testAllocator.Allocate(7, 8, 9.0f);
    EXPECT_EQ(testStructPtr, testStructPtr4);

    testAllocator.Free(testStructPtr3);
    testAllocator.Free(testStructPtr2);
    testAllocator.Free(testStructPtr4);
}

TEST_F(PoolAllocatorTest, Allocate_DoesNotReuseUnfreeddAddresses)
{
    std::unordered_set<TestStructOne*> allocatedAddresses;

    // Allocate all slots
    TestStructOne* ptr1 = testAllocator.Allocate(1, 2, 3.0f);
    TestStructOne* ptr2 = testAllocator.Allocate(5, 3, 6.0f);
    TestStructOne* ptr3 = testAllocator.Allocate(2, 9, 8.0f);

    // Track allocated addresses
    allocatedAddresses.insert(ptr1);
    allocatedAddresses.insert(ptr2);
    allocatedAddresses.insert(ptr3);

    // Free middle pointer
    testAllocator.Free(ptr2);
    allocatedAddresses.erase(ptr2);

    // New allocation should return ptr2's address
    TestStructOne* newPtr = testAllocator.Allocate(7, 8, 9.0f);
    EXPECT_EQ(newPtr, ptr2);

    // Verify no addresses are reused
    EXPECT_EQ(allocatedAddresses.count(newPtr), 0);
    allocatedAddresses.insert(newPtr);

    // Attempt allocation when full
    EXPECT_EQ(testAllocator.Allocate(1, 1, 1.0f), nullptr);

    // Cleanup
    for (auto ptr : allocatedAddresses) {
        testAllocator.Free(ptr);
    }
}