//
// Created by Andrew Fagan on 1/26/25.
//

#include <gtest/gtest.h>

#include "Core/Memory/Pools/ObjectPool.h"
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

class ObjectPoolTest : public ::testing::Test
{
public:
    static constexpr int DEFAULT_ALLOCATION_SIZE = 3;
    Core::ObjectPool<TestStructOne, DEFAULT_ALLOCATION_SIZE> testAllocator;
};


TEST_F(ObjectPoolTest, InitWithElementConstructor_CompilesWithNoDefaultConstructor)
{
    // Default pool allocator holds 3 instances of TestStruct
    NonDefaultConstructorClass testElement = NonDefaultConstructorClass(152);
    Core::ObjectPool<NonDefaultConstructorClass, 2> testAllocator = Core::ObjectPool<NonDefaultConstructorClass, 2>(testElement);
    NonDefaultConstructorClass* object1Ptr = testAllocator.Acquire();
    NonDefaultConstructorClass* object2Ptr = testAllocator.Acquire();

    EXPECT_EQ(object1Ptr->GetElementValue(), object2Ptr->GetElementValue());
}

TEST_F(ObjectPoolTest, Allocate_ReturnsNullptrWhenOutOfSpace)
{
    // Default pool allocator holds 3 instances of TestStruct
    TestStructOne* testStructPtr = testAllocator.Acquire();
    TestStructOne* testStructPtr2 = testAllocator.Acquire();
    TestStructOne* testStructPtr3 = testAllocator.Acquire();
    TestStructOne* testStructPtr4 = testAllocator.Acquire();

    EXPECT_NE(testStructPtr, nullptr);
    EXPECT_NE(testStructPtr2, nullptr);
    EXPECT_NE(testStructPtr3, nullptr);

    EXPECT_EQ(testStructPtr4, nullptr);
}


TEST_F(ObjectPoolTest, Allocate_ResetsInstanceOnAllocate)
{
    // Default pool allocator holds 3 instances of TestStruct
    TestStructOne* testStructPtr = testAllocator.Acquire();
    [[maybe_unused]] TestStructOne* testStructPtr2 = testAllocator.Acquire();
    [[maybe_unused]] TestStructOne* testStructPtr3 = testAllocator.Acquire();

    // testAllocator now has no more instances to allocate

    TestStructOne differentTestStruct;
    differentTestStruct.a = 12;
    differentTestStruct.b = 13;
    differentTestStruct.c = 13.5f;

    // Set the first test struct ptr to something different from the default constructed instance
    *testStructPtr = differentTestStruct;

    // Free only one instance
    testAllocator.Release(testStructPtr);
    testStructPtr = nullptr;

    // Now only one instance is available and it was the previous instance freed
    TestStructOne* newTestStructPtr = testAllocator.Acquire();

    // Now compare the instance to see if it is still the values form differentTestStruct. They should be different
    // because the instance should be reset.
    EXPECT_NE(newTestStructPtr->a, differentTestStruct.a);
    EXPECT_NE(newTestStructPtr->b, differentTestStruct.b);
    EXPECT_NE(newTestStructPtr->c, differentTestStruct.c);
}

TEST_F(ObjectPoolTest, Free_CanFreeInAnyOrder)
{
    // Default pool allocator holds 3 instances of TestStruct
    TestStructOne* testStructPtr = testAllocator.Acquire();
    TestStructOne* testStructPtr2 = testAllocator.Acquire();
    TestStructOne* testStructPtr3 = testAllocator.Acquire();

    testAllocator.Release(testStructPtr);

    // Should be able to allocate in freed slot
    TestStructOne* testStructPtr4 = testAllocator.Acquire();
    EXPECT_EQ(testStructPtr, testStructPtr4);

    testAllocator.Release(testStructPtr3);
    testAllocator.Release(testStructPtr2);
    testAllocator.Release(testStructPtr4);
}

TEST_F(ObjectPoolTest, Allocate_DoesNotReuseUnfreeddAddresses)
{
    std::unordered_set<TestStructOne*> allocatedAddresses;

    // Allocate all slots
    TestStructOne* ptr1 = testAllocator.Acquire();
    TestStructOne* ptr2 = testAllocator.Acquire();
    TestStructOne* ptr3 = testAllocator.Acquire();

    // Track allocated addresses
    allocatedAddresses.insert(ptr1);
    allocatedAddresses.insert(ptr2);
    allocatedAddresses.insert(ptr3);

    // Free middle pointer
    testAllocator.Release(ptr2);
    allocatedAddresses.erase(ptr2);

    // New allocation should return ptr2's address
    TestStructOne* newPtr = testAllocator.Acquire();
    EXPECT_EQ(newPtr, ptr2);

    // Verify no addresses are reused
    EXPECT_EQ(allocatedAddresses.count(newPtr), 0);
    allocatedAddresses.insert(newPtr);

    // Attempt allocation when full
    EXPECT_EQ(testAllocator.Acquire(), nullptr);

    // Cleanup
    for (auto ptr : allocatedAddresses) {
        testAllocator.Release(ptr);
    }
}



