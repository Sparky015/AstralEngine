/**
* @file FrameAllocatorTests.cpp
* @author Andrew Fagan
* @date 1/20/2025
*/

#include <gtest/gtest.h>
#include <cstring>

#include "Core/Memory/Allocators/FrameAllocator.h"

class FrameAllocatorTest : public ::testing::Test
{
public:
    static constexpr int DEFAULT_ALLOCATION_SIZE = 2056;
    Core::FrameAllocator testAllocator = Core::FrameAllocator(DEFAULT_ALLOCATION_SIZE);
};

/**@brief Tests if the allocator is allocating the correct amount of space for an allocation */
TEST_F(FrameAllocatorTest, allocate_AllocatesCorrectAmountOfSpace)
{
    const char* allocatedAddress = (const char*) testAllocator.Allocate(5, alignof(char));
    const char* allocatedAddress2 = (const char*) testAllocator.Allocate(27, alignof(char));
    const char* allocatedAddress3 = (const char*) testAllocator.Allocate(57, alignof(char));

    EXPECT_EQ(allocatedAddress2 - allocatedAddress, 5);
    EXPECT_EQ(allocatedAddress3 - allocatedAddress2, 27);

    //  +1 for each allocation header because char is naturally already aligned, so the allocate method will push it back to fit the header
    EXPECT_EQ(testAllocator.GetUsedBlockSize(), 5 + 27 + 57); // 5 + 27 + 57 == 89
}

/**@brief Tests if the allocator returns addresses that can be read from and written to */
TEST_F(FrameAllocatorTest, allocate_ReturnsUseableAddresses)
{
    char* allocatedAddress = (char*) testAllocator.Allocate(5, alignof(char)); // allocates 5 chars
    std::strcpy(allocatedAddress, "abcd\0");
    EXPECT_STREQ(allocatedAddress, "abcd\0");

    char* allocatedAddress2 = (char*) testAllocator.Allocate(27, alignof(char));
    std::strcpy(allocatedAddress2, "abcdefghijklmnopqrstuvwxyz\0");
    EXPECT_STREQ(allocatedAddress2, "abcdefghijklmnopqrstuvwxyz\0");
}

/**@brief Tests if the allocator throws an error if the allocation size is too big */
TEST_F(FrameAllocatorTest, allocate_ThrowsOnExcessiveAllocationSize)
{
    Core::FrameAllocator testAllocator = Core::FrameAllocator(2056);
    EXPECT_THROW(testAllocator.Allocate(3000, alignof(char)), std::bad_alloc);
    EXPECT_THROW(testAllocator.Allocate(2057, alignof(char)), std::bad_alloc);
    EXPECT_NO_THROW(testAllocator.Allocate(2056, alignof(char)));
}

/**@brief Tests if the allocator throws an error if the allocation size is too big */
TEST_F(FrameAllocatorTest, allocate_ThrowsOnExcessiveCumulativeAllocationSize)
{
    Core::FrameAllocator testAllocator = Core::FrameAllocator(2200);
    EXPECT_NO_THROW(testAllocator.Allocate(300, alignof(char))); // Total Allocation: 300
    EXPECT_NO_THROW(testAllocator.Allocate(400, alignof(char))); // Total Allocation: 700
    EXPECT_NO_THROW(testAllocator.Allocate(200, alignof(char))); // Total Allocation: 900
    EXPECT_NO_THROW(testAllocator.Allocate(700, alignof(char))); // Total Allocation: 1600
    EXPECT_NO_THROW(testAllocator.Allocate(500, alignof(char))); // Total Allocation: 2100
    EXPECT_THROW(testAllocator.Allocate(250, alignof(char)), std::bad_alloc); // Total Allocation: 2350 -> TOO BIG
}


/**@brief Tests if the Reset method correctly resets the state of the allocator back to the start of the memory block */
TEST_F(FrameAllocatorTest, reset_CorrectlyResetsAllocatorMemoryBlock)
{
    Core::FrameAllocator testAllocator = Core::FrameAllocator(2200);
    EXPECT_NO_THROW(testAllocator.Allocate(300, alignof(char))); // Total Allocation: 300
    EXPECT_NO_THROW(testAllocator.Allocate(400, alignof(char))); // Total Allocation: 700
    EXPECT_NO_THROW(testAllocator.Allocate(200, alignof(char))); // Total Allocation: 900
    EXPECT_NO_THROW(testAllocator.Allocate(700, alignof(char))); // Total Allocation: 1600
    EXPECT_NO_THROW(testAllocator.Allocate(500, alignof(char))); // Total Allocation: 2100
    testAllocator.Reset(); // Total Allocation: 0

    EXPECT_NO_THROW(testAllocator.Allocate(300, alignof(char))); // Total Allocation: 300
    EXPECT_NO_THROW(testAllocator.Allocate(400, alignof(char))); // Total Allocation: 700
    EXPECT_NO_THROW(testAllocator.Allocate(200, alignof(char))); // Total Allocation: 900
    EXPECT_NO_THROW(testAllocator.Allocate(700, alignof(char))); // Total Allocation: 1600
    EXPECT_NO_THROW(testAllocator.Allocate(500, alignof(char))); // Total Allocation: 2100
    testAllocator.Reset(); // Total Allocation: 0

    EXPECT_NO_THROW(testAllocator.Allocate(250, alignof(char))); // Total Allocation: 250
    EXPECT_EQ(testAllocator.GetUsedBlockSize(), 250);
}


// /**@brief Tests if the Deallocate method frees the previous allocation that was made */
// TEST_F(FrameAllocatorTest, deallocate_DoesNothing)
// {
//     char* memoryAddress = (char*) testAllocator.Allocate(500, alignof(char));
//     testAllocator.Deallocate(memoryAddress, 500);
//     EXPECT_EQ(testAllocator.GetUsedBlockSize(), 500);
//
//     char* memoryAddress2 = (char*) testAllocator.Allocate(1000, alignof(char));
//     testAllocator.Deallocate(memoryAddress2, 1000);
//     EXPECT_EQ(testAllocator.GetUsedBlockSize(), 1500);
//
//     char* memoryAddress3 = (char*) testAllocator.Allocate(100, alignof(char));
//     testAllocator.Deallocate(memoryAddress3, 100);
//     EXPECT_EQ(testAllocator.GetUsedBlockSize(), 1600);
// }

/**@brief Tests if the GetUsedBlockSize method is returning the accurate amount of space that is currently allocated by the allocator */
TEST_F(FrameAllocatorTest, getUsedBlockSize_ReturnsTheCorrectAmountOfSpaceCurrentlyAllocated)
{
    Core::FrameAllocator testAllocator = Core::FrameAllocator(2056);

    testAllocator.Allocate(5, alignof(char));
    EXPECT_EQ(testAllocator.GetUsedBlockSize(), 5); // allocation size + 1 for header
    testAllocator.Allocate(27, alignof(char));
    EXPECT_EQ(testAllocator.GetUsedBlockSize(), 5 + 27);
    testAllocator.Allocate(57, alignof(char));
    EXPECT_EQ(testAllocator.GetUsedBlockSize(), 5 + 27 + 57);
    testAllocator.Reset();
    EXPECT_EQ(testAllocator.GetUsedBlockSize(), 0);
    testAllocator.Allocate(812, alignof(char));
    EXPECT_EQ(testAllocator.GetUsedBlockSize(), 812);
    testAllocator.Allocate(11, alignof(char));
    EXPECT_EQ(testAllocator.GetUsedBlockSize(), 812 + 11);
    testAllocator.Allocate(71, alignof(char));
    EXPECT_EQ(testAllocator.GetUsedBlockSize(), 812 + 11 + 71);
}

/**@brief Tests if the natural alignment is applied to allocations */
TEST_F(FrameAllocatorTest, allocate_RespectsTypeAlignment)
{
    struct alignas(16) AlignedStruct { char data[8]; };
    Core::FrameAllocator alignedAllocator = Core::FrameAllocator(128);

    AlignedStruct* ptr = (AlignedStruct*) alignedAllocator.Allocate(2, alignof(AlignedStruct));
    EXPECT_EQ(reinterpret_cast<std::uintptr_t>(ptr) % 16, 0);
    AlignedStruct* ptr2 = (AlignedStruct*) alignedAllocator.Allocate(3, alignof(AlignedStruct));
    EXPECT_EQ(reinterpret_cast<std::uintptr_t>(ptr2) % 16, 0);
}

/**@brief Tests if the natural alignment is applied when different alignment requirements request allocations */
TEST_F(FrameAllocatorTest, allocate_RespectsMultipleTypesAlignment)
{
    struct alignas(16) AlignedStruct { char data[8]; };
    Core::FrameAllocator alignedAllocator = Core::FrameAllocator(500);

    AlignedStruct* ptr = (AlignedStruct*) alignedAllocator.Allocate(sizeof(AlignedStruct), alignof(AlignedStruct));
    EXPECT_EQ(reinterpret_cast<std::uintptr_t>(ptr) % alignof(AlignedStruct), 0);

    uint64* ptr2 = (uint64*) alignedAllocator.Allocate(sizeof(uint64), alignof(uint64));
    EXPECT_EQ(reinterpret_cast<std::uintptr_t>(ptr2) % alignof(uint64), 0);

    struct TestStruct
    {
        int a;
        float b;
        char c;
        double d;
        int16 e;
        char f[7];
    };

    TestStruct* ptr3 = (TestStruct*) alignedAllocator.Allocate(sizeof(TestStruct), alignof(TestStruct));
    EXPECT_EQ(reinterpret_cast<std::uintptr_t>(ptr3) % alignof(TestStruct), 0);

    float* ptr4 = (float*) alignedAllocator.Allocate(sizeof(float), alignof(float));
    EXPECT_EQ(reinterpret_cast<std::uintptr_t>(ptr4) % alignof(float), 0);
}

/**@brief Tests if the rollback marker moves back to the correct spot */
TEST_F(FrameAllocatorTest, RollbackToMarker_RollsBackToTheCorrectSpot)
{
    Core::FrameAllocator allocator = Core::FrameAllocator(500);
    Core::FrameAllocator::Marker bottomMarker = allocator.GetMarker();

    allocator.Allocate(sizeof(int) * 5, alignof(int));
    allocator.Allocate(sizeof(char) * 3, alignof(char));

    int size1 = allocator.GetUsedBlockSize();
    Core::FrameAllocator::Marker marker1 = allocator.GetMarker();

    allocator.Allocate(sizeof(int) * 7, alignof(int));
    allocator.Allocate(sizeof(char) * 3, alignof(char));
    allocator.Allocate(sizeof(uint64) * 3, alignof(uint64));

    allocator.RollbackToMarker(marker1);
    EXPECT_EQ(size1, allocator.GetUsedBlockSize());
    EXPECT_EQ(marker1, allocator.GetMarker());

    allocator.RollbackToMarker(bottomMarker);
    EXPECT_EQ(0, allocator.GetUsedBlockSize());
    EXPECT_EQ(bottomMarker, allocator.GetMarker());
}