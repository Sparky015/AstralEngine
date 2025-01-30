//
// Created by Andrew Fagan on 1/15/25.
//

#include <gtest/gtest.h>

#include "Core/Memory/Allocators/StackBasedLinearAllocator.h"
#include <cstring>

class StackBasedLinearAllocatorTest : public ::testing::Test
{
public:
    static constexpr int DEFAULT_ALLOCATION_SIZE = 2056;
    Core::StackBasedLinearAllocator<DEFAULT_ALLOCATION_SIZE> testAllocator;
};

/**@brief Tests if the allocator is allocating the correct amount of space for an allocation */
TEST_F(StackBasedLinearAllocatorTest, allocate_AllocatesCorrectAmountOfSpace)
{
    const char* allocatedAddress = (const char*) testAllocator.Allocate(5, alignof(char));
    const char* allocatedAddress2 = (const char*) testAllocator.Allocate(27, alignof(char));
    const char* allocatedAddress3 = (const char*) testAllocator.Allocate(57, alignof(char));

    EXPECT_EQ(allocatedAddress2 - allocatedAddress, 5 );
    EXPECT_EQ(allocatedAddress3 - allocatedAddress2, 27);

    EXPECT_EQ(testAllocator.GetUsedBlockSize(), 5 + 27 + 57 );
}

/**@brief Tests if the allocator returns addresses that can be read from and written to */
TEST_F(StackBasedLinearAllocatorTest, allocate_ReturnsUseableAddresses)
{
    char* allocatedAddress = (char*) testAllocator.Allocate(5, alignof(char)); // allocates 5 chars
    std::strcpy(allocatedAddress, "abcd\0");
    EXPECT_STREQ(allocatedAddress, "abcd\0");

    char* allocatedAddress2 = (char*) testAllocator.Allocate(27, alignof(char));
    std::strcpy(allocatedAddress2, "abcdefghijklmnopqrstuvwxyz\0");
    EXPECT_STREQ(allocatedAddress2, "abcdefghijklmnopqrstuvwxyz\0");
}

/**@brief Tests if the allocator throws an error if the allocation size is too big */
TEST_F(StackBasedLinearAllocatorTest, allocate_ThrowsOnExcessiveAllocationSize)
{
    Core::StackBasedLinearAllocator<2056> testAllocator;
    EXPECT_THROW(testAllocator.Allocate(3000, alignof(char)), std::bad_alloc);
    EXPECT_THROW(testAllocator.Allocate(2057, alignof(char)), std::bad_alloc);
    EXPECT_NO_THROW(testAllocator.Allocate(2056, alignof(char)));
}

/**@brief Tests if the allocator throws an error if the allocation size is too big */
TEST_F(StackBasedLinearAllocatorTest, allocate_ThrowsOnExcessiveCumulativeAllocationSize)
{
    Core::StackBasedLinearAllocator<2200> testAllocator;
    EXPECT_NO_THROW(testAllocator.Allocate(300, alignof(char))); // Total Allocation: 300
    EXPECT_NO_THROW(testAllocator.Allocate(400, alignof(char))); // Total Allocation: 700
    EXPECT_NO_THROW(testAllocator.Allocate(200, alignof(char))); // Total Allocation: 900
    EXPECT_NO_THROW(testAllocator.Allocate(700, alignof(char))); // Total Allocation: 1600
    EXPECT_NO_THROW(testAllocator.Allocate(500, alignof(char))); // Total Allocation: 2100
    EXPECT_THROW(testAllocator.Allocate(250, alignof(char)), std::bad_alloc); // Total Allocation: 2350 -> TOO BIG
}


/**@brief Tests if the Reset method correctly resets the state of the allocator back to the start of the memory block */
TEST_F(StackBasedLinearAllocatorTest, reset_CorrectlyResetsAllocatorMemoryBlock)
{
    Core::StackBasedLinearAllocator<2200> testAllocator;
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

/**@brief Tests if the GetUsedBlockSize method is returning the accurate amount of space that is currently allocated by the allocator */
TEST_F(StackBasedLinearAllocatorTest, getUsedBlockSize_ReturnsTheCorrectAmountOfSpaceCurrentlyAllocated)
{
    Core::StackBasedLinearAllocator<2056> testAllocator;

    testAllocator.Allocate(5, alignof(char));
    EXPECT_EQ(testAllocator.GetUsedBlockSize(), 5 );
    testAllocator.Allocate(27, alignof(char));
    EXPECT_EQ(testAllocator.GetUsedBlockSize(), 5 + 27 );
    testAllocator.Allocate(57, alignof(char));
    EXPECT_EQ(testAllocator.GetUsedBlockSize(), 5 + 27 + 57 );
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
TEST_F(StackBasedLinearAllocatorTest, allocate_RespectsTypeAlignment)
{
    struct alignas(16) AlignedStruct { char data[8]; };
    Core::StackBasedLinearAllocator<1024> alignedAllocator;

    AlignedStruct* ptr = (AlignedStruct*) alignedAllocator.Allocate(1, alignof(AlignedStruct));
    EXPECT_EQ(reinterpret_cast<std::uintptr_t>(ptr) % 16, 0);
}

/**@brief Tests if the natural alignment is applied when different alignment requirements request allocations */
TEST_F(StackBasedLinearAllocatorTest, allocate_RespectsMultipleTypesAlignment)
{
    struct alignas(16) AlignedStruct { char data[8]; };
    Core::StackBasedLinearAllocator<1024> alignedAllocator;

    AlignedStruct* ptr = (AlignedStruct*) alignedAllocator.Allocate(sizeof(AlignedStruct), alignof(AlignedStruct));
    EXPECT_EQ(reinterpret_cast<std::uintptr_t>(ptr) % alignof(AlignedStruct), 0);

    uint64* ptr2 = (uint64*) alignedAllocator.Allocate(sizeof(uint64), alignof(uint64));
    EXPECT_EQ(reinterpret_cast<std::uintptr_t>(ptr2) % alignof(uint64), 0);

    float* ptr3 = (float*) alignedAllocator.Allocate(sizeof(float), alignof(float));
    EXPECT_EQ(reinterpret_cast<std::uintptr_t>(ptr3) % alignof(float), 0);
}

