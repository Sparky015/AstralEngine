//
// Created by Andrew Fagan on 1/15/25.
//

#include <gtest/gtest.h>

#include "Core/Memory/Allocators/LinearAllocator.h"

class LinearAllocatorTest : public ::testing::Test
{
public:
    static constexpr int DEFAULT_ALLOCATION_SIZE = 2056;
    Core::LinearAllocator<DEFAULT_ALLOCATION_SIZE> testAllocator;
};

/**@brief Tests if the allocator is allocating the correct amount of space for an allocation */
TEST_F(LinearAllocatorTest, allocate_AllocatesCorrectAmountOfSpace)
{
    const char* allocatedAddress = (const char*)testAllocator.allocate(5, alignof(char));
    const char* allocatedAddress2 = (const char*)testAllocator.allocate(27, alignof(char));
    const char* allocatedAddress3 = (const char*)testAllocator.allocate(57, alignof(char));

    EXPECT_EQ(allocatedAddress2 - allocatedAddress, 5 + 1); // + 1 for allocation header
    EXPECT_EQ(allocatedAddress3 - allocatedAddress2, 27 + 1);

    //  +1 for each allocation header because char is naturally already aligned, so the allocate method will push it back to fit the header
    EXPECT_EQ(testAllocator.getUsedBlockSize(), 5 + 27 + 57 + 3); // 5 + 27 + 57 == 89
}

/**@brief Tests if the allocator returns addresses that can be read from and written to */
TEST_F(LinearAllocatorTest, allocate_ReturnsUseableAddresses)
{
    char* allocatedAddress = (char*)testAllocator.allocate(5, alignof(char)); // allocates 5 chars
    std::strcpy(allocatedAddress, "abcd\0");
    EXPECT_STREQ(allocatedAddress, "abcd\0");

    char* allocatedAddress2 = (char*)testAllocator.allocate(27, alignof(char));
    std::strcpy(allocatedAddress2, "abcdefghijklmnopqrstuvwxyz\0");
    EXPECT_STREQ(allocatedAddress2, "abcdefghijklmnopqrstuvwxyz\0");
}

/**@brief Tests if the allocator throws an error if the allocation size is too big */
TEST_F(LinearAllocatorTest, allocate_ThrowsOnExcessiveAllocationSize)
{
    Core::LinearAllocator<2056> testAllocator;
    EXPECT_THROW(testAllocator.allocate(3000, alignof(char)), std::bad_alloc);
    EXPECT_THROW(testAllocator.allocate(2056, alignof(char)), std::bad_alloc); // Size will be 2027 with allocation header
    EXPECT_NO_THROW(testAllocator.allocate(2055, alignof(char)));
}

/**@brief Tests if the allocator throws an error if the allocation size is too big */
TEST_F(LinearAllocatorTest, allocate_ThrowsOnExcessiveCumulativeAllocationSize)
{
    Core::LinearAllocator<2200> testAllocator;
    EXPECT_NO_THROW(testAllocator.allocate(300, alignof(char))); // Total Allocation: 300
    EXPECT_NO_THROW(testAllocator.allocate(400, alignof(char))); // Total Allocation: 700
    EXPECT_NO_THROW(testAllocator.allocate(200, alignof(char))); // Total Allocation: 900
    EXPECT_NO_THROW(testAllocator.allocate(700, alignof(char))); // Total Allocation: 1600
    EXPECT_NO_THROW(testAllocator.allocate(500, alignof(char))); // Total Allocation: 2100
    EXPECT_THROW(testAllocator.allocate(250, alignof(char)), std::bad_alloc); // Total Allocation: 2350 -> TOO BIG
}


/**@brief Tests if the reset method correctly resets the state of the allocator back to the start of the memory block */
TEST_F(LinearAllocatorTest, reset_CorrectlyResetsAllocatorMemoryBlock)
{
    Core::LinearAllocator<2200> testAllocator;
    EXPECT_NO_THROW(testAllocator.allocate(300, alignof(char))); // Total Allocation: 300
    EXPECT_NO_THROW(testAllocator.allocate(400, alignof(char))); // Total Allocation: 700
    EXPECT_NO_THROW(testAllocator.allocate(200, alignof(char))); // Total Allocation: 900
    EXPECT_NO_THROW(testAllocator.allocate(700, alignof(char))); // Total Allocation: 1600
    EXPECT_NO_THROW(testAllocator.allocate(500, alignof(char))); // Total Allocation: 2100
    testAllocator.reset(); // Total Allocation: 0

    EXPECT_NO_THROW(testAllocator.allocate(300, alignof(char))); // Total Allocation: 300
    EXPECT_NO_THROW(testAllocator.allocate(400, alignof(char))); // Total Allocation: 700
    EXPECT_NO_THROW(testAllocator.allocate(200, alignof(char))); // Total Allocation: 900
    EXPECT_NO_THROW(testAllocator.allocate(700, alignof(char))); // Total Allocation: 1600
    EXPECT_NO_THROW(testAllocator.allocate(500, alignof(char))); // Total Allocation: 2100
    testAllocator.reset(); // Total Allocation: 0

    EXPECT_NO_THROW(testAllocator.allocate(250, alignof(char))); // Total Allocation: 250
    EXPECT_EQ(testAllocator.getUsedBlockSize(), 250 + 1);
}


/**@brief Tests if the deallocate method does absolutely nothing when called */
TEST_F(LinearAllocatorTest, deallocate_DoesNothing)
{
    char* memoryAddress = (char*)testAllocator.allocate(500, alignof(char));
    testAllocator.deallocate(memoryAddress, 500);
    EXPECT_EQ(testAllocator.getUsedBlockSize(), 500 + 1);

    char* memoryAddress2 = (char*)testAllocator.allocate(1000, alignof(char));
    testAllocator.deallocate(memoryAddress2, 1000);
    EXPECT_EQ(testAllocator.getUsedBlockSize(), 1500 + 2);

    char* memoryAddress3 = (char*)testAllocator.allocate(100, alignof(char));
    testAllocator.deallocate(memoryAddress3, 100);
    EXPECT_EQ(testAllocator.getUsedBlockSize(), 1600 + 3);
}

/**@brief Tests if the getUsedBlockSize method is returning the accurate amount of space that is currently allocated by the allocator */
TEST_F(LinearAllocatorTest, getUsedBlockSize_ReturnsTheCorrectAmountOfSpaceCurrentlyAllocated)
{
    Core::LinearAllocator<2056> testAllocator;

    testAllocator.allocate(5, alignof(char));
    EXPECT_EQ(testAllocator.getUsedBlockSize(), 5 + 1);
    testAllocator.allocate(27, alignof(char));
    EXPECT_EQ(testAllocator.getUsedBlockSize(), 5 + 27 + 2);
    testAllocator.allocate(57, alignof(char));
    EXPECT_EQ(testAllocator.getUsedBlockSize(), 5 + 27 + 57 + 3);
    testAllocator.reset();
    EXPECT_EQ(testAllocator.getUsedBlockSize(), 0);
    testAllocator.allocate(812, alignof(char));
    EXPECT_EQ(testAllocator.getUsedBlockSize(), 812 + 1);
    testAllocator.allocate(11, alignof(char));
    EXPECT_EQ(testAllocator.getUsedBlockSize(), 812 + 11 + 2);
    testAllocator.allocate(71, alignof(char));
    EXPECT_EQ(testAllocator.getUsedBlockSize(), 812 + 11 + 71 + 3);
}

/**@brief Tests if the natural alignment is applied to allocations */
TEST_F(LinearAllocatorTest, allocate_RespectsTypeAlignment)
{
    struct alignas(16) AlignedStruct { char data[8]; };
    Core::LinearAllocator<1024> alignedAllocator;

    AlignedStruct* ptr = (AlignedStruct*)alignedAllocator.allocate(1, alignof(AlignedStruct));
    EXPECT_EQ(reinterpret_cast<std::uintptr_t>(ptr) % 16, 0);
}

/**@brief Tests if the natural alignment is applied when different alignment requirements request allocations */
TEST_F(LinearAllocatorTest, allocate_RespectsMultipleTypesAlignment)
{
    struct alignas(16) AlignedStruct { char data[8]; };
    Core::LinearAllocator<1024> alignedAllocator;

    AlignedStruct* ptr = (AlignedStruct*)alignedAllocator.allocate(sizeof(AlignedStruct), alignof(AlignedStruct));
    EXPECT_EQ(reinterpret_cast<std::uintptr_t>(ptr) % alignof(AlignedStruct), 0);

    uint64* ptr2 = (uint64*)alignedAllocator.allocate(sizeof(uint64), alignof(uint64));
    EXPECT_EQ(reinterpret_cast<std::uintptr_t>(ptr2) % alignof(uint64), 0);

    float* ptr3 = (float*)alignedAllocator.allocate(sizeof(float), alignof(float));
    EXPECT_EQ(reinterpret_cast<std::uintptr_t>(ptr3) % alignof(float), 0);
}

