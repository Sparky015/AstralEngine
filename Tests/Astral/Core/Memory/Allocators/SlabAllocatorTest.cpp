//
// Created by Andrew Fagan on 1/26/25.
//

#include <gtest/gtest.h>

#include "Core/Memory/Allocators/SlabAllocator.h"
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

class NonDefaultConstructorClass
{
public:
    explicit NonDefaultConstructorClass(const int a) : a(a) {};

    int GetElementValue() const  { return a; }

private:
    int a;
};

class SlabAllocatorTest : public ::testing::Test
{
public:
    static constexpr int DEFAULT_ALLOCATION_SIZE = 3;
    Astral::SlabAllocator<TestStructOne, DEFAULT_ALLOCATION_SIZE> testAllocator = Astral::SlabAllocator<TestStructOne, DEFAULT_ALLOCATION_SIZE>{};
};


TEST_F(SlabAllocatorTest, InitWithElementConstructor_CompilesWithNoDefaultConstructor)
{
    // Default pool allocator holds 3 instances of TestStruct
    NonDefaultConstructorClass testElement = NonDefaultConstructorClass(152);
    Astral::SlabAllocator<NonDefaultConstructorClass, 2> testAllocator = Astral::SlabAllocator<NonDefaultConstructorClass, 2>(testElement);
    NonDefaultConstructorClass* object1Ptr = testAllocator.Allocate();
    NonDefaultConstructorClass* object2Ptr = testAllocator.Allocate();

    EXPECT_EQ(object1Ptr->GetElementValue(), object2Ptr->GetElementValue());
}

TEST_F(SlabAllocatorTest, Allocate_ReturnsNullptrWhenOutOfSpace)
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


TEST_F(SlabAllocatorTest, Allocate_InstanceKeepsStateOverAllocates)
{
    // Default pool allocator holds 3 instances of TestStruct
    TestStructOne* testStructPtr = testAllocator.Allocate();
    [[maybe_unused]] TestStructOne* testStructPtr2 = testAllocator.Allocate();
    [[maybe_unused]] TestStructOne* testStructPtr3 = testAllocator.Allocate();

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

    // Now compare the instance to see if it is still the values form differentTestStruct. They should be the same
    // because the instances should not be reset.
    EXPECT_EQ(newTestStructPtr->a, differentTestStruct.a);
    EXPECT_EQ(newTestStructPtr->b, differentTestStruct.b);
    EXPECT_EQ(newTestStructPtr->c, differentTestStruct.c);
}

TEST_F(SlabAllocatorTest, Free_CanFreeInAnyOrder)
{
    // Default pool allocator holds 3 instances of TestStruct
    TestStructOne* testStructPtr = testAllocator.Allocate();
    TestStructOne* testStructPtr2 = testAllocator.Allocate();
    TestStructOne* testStructPtr3 = testAllocator.Allocate();

    testAllocator.Free(testStructPtr);

    // Should be able to allocate in freed slot
    TestStructOne* testStructPtr4 = testAllocator.Allocate();
    EXPECT_EQ(testStructPtr, testStructPtr4);

    testAllocator.Free(testStructPtr3);
    testAllocator.Free(testStructPtr2);
    testAllocator.Free(testStructPtr4);
}

TEST_F(SlabAllocatorTest, Allocate_DoesNotReuseUnfreeddAddresses)
{
    std::unordered_set<TestStructOne*> allocatedAddresses;

    // Allocate all slots
    TestStructOne* ptr1 = testAllocator.Allocate();
    TestStructOne* ptr2 = testAllocator.Allocate();
    TestStructOne* ptr3 = testAllocator.Allocate();

    // Track allocated addresses
    allocatedAddresses.insert(ptr1);
    allocatedAddresses.insert(ptr2);
    allocatedAddresses.insert(ptr3);

    // Free middle pointer
    testAllocator.Free(ptr2);
    allocatedAddresses.erase(ptr2);

    // New allocation should return ptr2's address
    TestStructOne* newPtr = testAllocator.Allocate();
    EXPECT_EQ(newPtr, ptr2);

    // Verify no addresses are reused
    EXPECT_EQ(allocatedAddresses.count(newPtr), 0);
    allocatedAddresses.insert(newPtr);

    // Attempt allocation when full
    EXPECT_EQ(testAllocator.Allocate(), nullptr);

    // Cleanup
    for (auto ptr : allocatedAddresses) {
        testAllocator.Free(ptr);
    }
}



