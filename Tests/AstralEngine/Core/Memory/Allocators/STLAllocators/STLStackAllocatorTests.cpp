//
// Created by Andrew Fagan on 1/15/25.
//

#include <gtest/gtest.h>

#include "Core/Memory/Allocators/STLAllocators/STLStackAllocator.h"
#include <cstring>

class STLStackAllocatorTest : public ::testing::Test
{
public:
    static constexpr int DEFAULT_ALLOCATION_SIZE = 2056;
    Core::STLStackAllocator<char> testAllocator = Core::STLStackAllocator<char>(DEFAULT_ALLOCATION_SIZE);
};

/**@brief Tests if the allocator is allocating the correct amount of space for an allocation */
TEST_F(STLStackAllocatorTest, allocate_AllocatesCorrectAmountOfSpace)
{
    const char* allocatedAddress = testAllocator.allocate(5);
    const char* allocatedAddress2 = testAllocator.allocate(27);
    const char* allocatedAddress3 = testAllocator.allocate(57);

    EXPECT_EQ(allocatedAddress2 - allocatedAddress, 5 + 1); // No need to account for alignment with chars
    EXPECT_EQ(allocatedAddress3 - allocatedAddress2, 27 + 1);
    EXPECT_EQ(testAllocator.getUsedBlockSize(), 5 + 27 + 57 + 3); // +3 for allocation header (1 for each allocation)
}

/**@brief Tests if the allocator returns addresses that can be read from and written to */
TEST_F(STLStackAllocatorTest, allocate_ReturnsUseableAddresses)
{
    char* allocatedAddress = testAllocator.allocate(5); // allocates 5 chars
    std::strcpy(allocatedAddress, "abcd\0");
    EXPECT_STREQ(allocatedAddress, "abcd\0");

    char* allocatedAddress2 = testAllocator.allocate(27);
    std::strcpy(allocatedAddress2, "abcdefghijklmnopqrstuvwxyz\0");
    EXPECT_STREQ(allocatedAddress2, "abcdefghijklmnopqrstuvwxyz\0");
}

/**@brief Tests if the allocator throws an error if the allocation size is too big */
TEST_F(STLStackAllocatorTest, allocate_ThrowsOnExcessiveAllocationSize)
{
    Core::STLStackAllocator<char> testAllocator = Core::STLStackAllocator<char>(2056);
    EXPECT_THROW(testAllocator.allocate(3000), std::bad_alloc);
    EXPECT_THROW(testAllocator.allocate(2057), std::bad_alloc);
    EXPECT_NO_THROW(testAllocator.allocate(2055)); // 2056 minus one to take into account of the allocation header
}

/**@brief Tests if the allocator throws an error if the allocation size is too big */
TEST_F(STLStackAllocatorTest, allocate_ThrowsOnExcessiveCumulativeAllocationSize)
{
    Core::STLStackAllocator<char> testAllocator = Core::STLStackAllocator<char>(2200);
    EXPECT_NO_THROW(testAllocator.allocate(300)); // Total Allocation: 300
    EXPECT_NO_THROW(testAllocator.allocate(400)); // Total Allocation: 700
    EXPECT_NO_THROW(testAllocator.allocate(200)); // Total Allocation: 900
    EXPECT_NO_THROW(testAllocator.allocate(700)); // Total Allocation: 1600
    EXPECT_NO_THROW(testAllocator.allocate(500)); // Total Allocation: 2100
    EXPECT_THROW(testAllocator.allocate(250), std::bad_alloc); // Total Allocation: 2350 -> TOO BIG
}


/**@brief Tests if the Reset method correctly resets the state of the allocator back to the start of the memory block */
TEST_F(STLStackAllocatorTest, reset_CorrectlyResetsAllocatorMemoryBlock)
{
    Core::STLStackAllocator<char> testAllocator = Core::STLStackAllocator<char>(2200);
    EXPECT_NO_THROW(testAllocator.allocate(300)); // Total Allocation: 300
    EXPECT_NO_THROW(testAllocator.allocate(400)); // Total Allocation: 700
    EXPECT_NO_THROW(testAllocator.allocate(200)); // Total Allocation: 900
    EXPECT_NO_THROW(testAllocator.allocate(700)); // Total Allocation: 1600
    EXPECT_NO_THROW(testAllocator.allocate(500)); // Total Allocation: 2100
    testAllocator.reset(); // Total Allocation: 0

    EXPECT_NO_THROW(testAllocator.allocate(300)); // Total Allocation: 300
    EXPECT_NO_THROW(testAllocator.allocate(400)); // Total Allocation: 700
    EXPECT_NO_THROW(testAllocator.allocate(200)); // Total Allocation: 900
    EXPECT_NO_THROW(testAllocator.allocate(700)); // Total Allocation: 1600
    EXPECT_NO_THROW(testAllocator.allocate(500)); // Total Allocation: 2100
    testAllocator.reset(); // Total Allocation: 0

    EXPECT_NO_THROW(testAllocator.allocate(250)); // Total Allocation: 250
    EXPECT_EQ(testAllocator.getUsedBlockSize(), 250 + 1);
}


/**@brief Tests if the Deallocate method frees the previous allocation that was made */
TEST_F(STLStackAllocatorTest, deallocate_ReleasesPreviousAllocationMemory)
{
    char* memoryAddress = testAllocator.allocate(500);
    testAllocator.deallocate(memoryAddress, 500);
    EXPECT_EQ(testAllocator.getUsedBlockSize(), 0);

    char* memoryAddress2 = testAllocator.allocate(500);
    char* memoryAddress3 = testAllocator.allocate(1000);
    EXPECT_EQ(testAllocator.getUsedBlockSize(), 500 + 1000 + 2); // +2 for allocation headers (1 for each allocation)
    testAllocator.deallocate(memoryAddress3, 1000);
    EXPECT_EQ(testAllocator.getUsedBlockSize(), 500 + 1);
    testAllocator.deallocate(memoryAddress2, 500);
    EXPECT_EQ(testAllocator.getUsedBlockSize(), 0);

    char* memoryAddress4 = testAllocator.allocate(500);
    char* memoryAddress5 = testAllocator.allocate(250);
    char* memoryAddress6 = testAllocator.allocate(700);
    EXPECT_EQ(testAllocator.getUsedBlockSize(), 500 + 250 + 700 + 3);
    testAllocator.deallocate(memoryAddress6, 700);
    EXPECT_EQ(testAllocator.getUsedBlockSize(), 500 + 250 + 2);
    testAllocator.deallocate(memoryAddress5, 250);
    EXPECT_EQ(testAllocator.getUsedBlockSize(), 500 + 1);
    testAllocator.deallocate(memoryAddress4, 500);
    EXPECT_EQ(testAllocator.getUsedBlockSize(), 0);
}

/**@brief Tests if the GetUsedBlockSize method is returning the accurate amount of space that is currently allocated by the allocator */
TEST_F(STLStackAllocatorTest, getUsedBlockSize_ReturnsTheCorrectAmountOfSpaceCurrentlyAllocated)
{
    Core::STLStackAllocator<char> testAllocator = Core::STLStackAllocator<char>(2056);

    testAllocator.allocate(5);
    EXPECT_EQ(testAllocator.getUsedBlockSize(), 5 + 1); // allocation size + 1 for header
    testAllocator.allocate(27);
    EXPECT_EQ(testAllocator.getUsedBlockSize(), 5 + 27 + 2);
    testAllocator.allocate(57);
    EXPECT_EQ(testAllocator.getUsedBlockSize(), 5 + 27 + 57 + 3);
    testAllocator.reset();
    EXPECT_EQ(testAllocator.getUsedBlockSize(), 0);
    testAllocator.allocate(812);
    EXPECT_EQ(testAllocator.getUsedBlockSize(), 812 + 1);
    testAllocator.allocate(11);
    EXPECT_EQ(testAllocator.getUsedBlockSize(), 812 + 11 + 2);
    testAllocator.allocate(71);
    EXPECT_EQ(testAllocator.getUsedBlockSize(), 812 + 11 + 71 + 3);
}

/**@brief Tests if the natural alignment is applied to allocations */
TEST_F(STLStackAllocatorTest, allocate_RespectsTypeAlignment)
{
    struct alignas(16) AlignedStruct { char data[8]; };
    Core::STLStackAllocator<AlignedStruct> alignedAllocator = Core::STLStackAllocator<AlignedStruct>(128);

    AlignedStruct* ptr = alignedAllocator.allocate(2);
    EXPECT_EQ(reinterpret_cast<std::uintptr_t>(ptr) % 16, 0);
    AlignedStruct* ptr2 = alignedAllocator.allocate(3);
    EXPECT_EQ(reinterpret_cast<std::uintptr_t>(ptr2) % 16, 0);
}