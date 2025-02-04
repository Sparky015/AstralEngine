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
    Core::StackBasedLinearAllocator testAllocator = Core::StackBasedLinearAllocator<2056>();
    EXPECT_EQ(testAllocator.Allocate(3000, alignof(char)), nullptr);
    EXPECT_EQ(testAllocator.Allocate(2057, alignof(char)), nullptr);
    EXPECT_NE(testAllocator.Allocate(2056, alignof(char)), nullptr);
}

/**@brief Tests if the allocator throws an error if the allocation size is too big */
TEST_F(StackBasedLinearAllocatorTest, allocate_ThrowsOnExcessiveCumulativeAllocationSize)
{
    Core::StackBasedLinearAllocator testAllocator = Core::StackBasedLinearAllocator<2200>();
    EXPECT_NE(testAllocator.Allocate(300, alignof(char)), nullptr);    // Total: 300
    EXPECT_NE(testAllocator.Allocate(400, alignof(char)), nullptr);    // Total: 700
    EXPECT_NE(testAllocator.Allocate(200, alignof(char)), nullptr);    // Total: 900
    EXPECT_NE(testAllocator.Allocate(700, alignof(char)), nullptr);    // Total: 1600
    EXPECT_NE(testAllocator.Allocate(500, alignof(char)), nullptr);    // Total: 2100
    EXPECT_EQ(testAllocator.Allocate(250, alignof(char)), nullptr);    // Total: 2350 -> TOO BIG
}


/**@brief Tests if the Reset method correctly resets the state of the allocator back to the start of the memory block */
TEST_F(StackBasedLinearAllocatorTest, reset_CorrectlyResetsAllocatorMemoryBlock)
{
    Core::StackBasedLinearAllocator testAllocator = Core::StackBasedLinearAllocator<2200>();
    EXPECT_NE(testAllocator.Allocate(300, alignof(char)), nullptr); // Total Allocation: 300
    EXPECT_NE(testAllocator.Allocate(400, alignof(char)), nullptr); // Total Allocation: 700
    EXPECT_NE(testAllocator.Allocate(200, alignof(char)), nullptr); // Total Allocation: 900
    EXPECT_NE(testAllocator.Allocate(700, alignof(char)), nullptr); // Total Allocation: 1600
    EXPECT_NE(testAllocator.Allocate(500, alignof(char)), nullptr); // Total Allocation: 2100
    testAllocator.Reset(); // Total Allocation: 0

    EXPECT_NE(testAllocator.Allocate(300, alignof(char)), nullptr); // Total Allocation: 300
    EXPECT_NE(testAllocator.Allocate(400, alignof(char)), nullptr); // Total Allocation: 700
    EXPECT_NE(testAllocator.Allocate(200, alignof(char)), nullptr); // Total Allocation: 900
    EXPECT_NE(testAllocator.Allocate(700, alignof(char)), nullptr); // Total Allocation: 1600
    EXPECT_NE(testAllocator.Allocate(500, alignof(char)), nullptr); // Total Allocation: 2100
    testAllocator.Reset(); // Total Allocation: 0

    EXPECT_NE(testAllocator.Allocate(250, alignof(char)), nullptr); // Total Allocation: 250
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

/**@brief Tests if the copy constructor creates a fully independent allocator with matching state */
TEST_F(StackBasedLinearAllocatorTest, CopyConstructor_CopiesStateCorrectly)
{
    [[maybe_unused]] char* buffer = (char*)testAllocator.Allocate(126, alignof(char));

    Core::StackBasedLinearAllocator<2056> testAllocator2 = Core::StackBasedLinearAllocator<2056>(testAllocator);

    EXPECT_EQ(testAllocator2.GetCapacity(), testAllocator.GetCapacity());
    EXPECT_EQ(testAllocator2.GetUsedBlockSize(), testAllocator.GetUsedBlockSize());
}


/**@brief Tests if copy assignment operator clones state and creates independent allocator */
TEST_F(StackBasedLinearAllocatorTest, CopyAssignmentOperator_ClonesState)
{
    [[maybe_unused]] char* buffer = (char*)testAllocator.Allocate(531, alignof(char));

    Core::StackBasedLinearAllocator<2056> testAllocator2 = Core::StackBasedLinearAllocator<2056>();
    testAllocator2 = testAllocator;

    EXPECT_EQ(testAllocator2.GetCapacity(), testAllocator.GetCapacity());
    EXPECT_EQ(testAllocator2.GetUsedBlockSize(), testAllocator.GetUsedBlockSize());

}

/**@brief Tests if move constructor transfers ownership and invalidates source */
TEST_F(StackBasedLinearAllocatorTest, MoveConstructor_TransfersOwnership)
{
    [[maybe_unused]] char* buffer = (char*)testAllocator.Allocate(731, alignof(char));

    size_t alloc1Capacity = testAllocator.GetCapacity();
    size_t alloc1UsedSize = testAllocator.GetUsedBlockSize();

    Core::StackBasedLinearAllocator<2056> testAllocator2 = Core::StackBasedLinearAllocator<2056>(std::move(testAllocator));

    EXPECT_EQ(testAllocator2.GetCapacity(), alloc1Capacity);
    EXPECT_EQ(testAllocator2.GetUsedBlockSize(), alloc1UsedSize);

    EXPECT_EQ(testAllocator.GetCapacity(), 0);
    EXPECT_EQ(testAllocator.GetUsedBlockSize(), 0);
}

/**@brief Tests if move assignment operator transfers ownership */
TEST_F(StackBasedLinearAllocatorTest, MoveAssignmentOperator_TransfersOwnership)
{
    [[maybe_unused]]  char* buffer = (char*)testAllocator.Allocate(221, alignof(char));

    size_t alloc1Capacity = testAllocator.GetCapacity();
    size_t alloc1UsedSize = testAllocator.GetUsedBlockSize();

    Core::StackBasedLinearAllocator<2056> testAllocator2 = Core::StackBasedLinearAllocator<2056>();
    testAllocator2 = std::move(testAllocator);

    EXPECT_EQ(testAllocator2.GetCapacity(), alloc1Capacity);
    EXPECT_EQ(testAllocator2.GetUsedBlockSize(), alloc1UsedSize);

    EXPECT_EQ(testAllocator.GetCapacity(), 0);
    EXPECT_EQ(testAllocator.GetUsedBlockSize(), 0);

}