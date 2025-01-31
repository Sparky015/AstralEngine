//
// Created by Andrew Fagan on 1/15/25.
//

#include <gtest/gtest.h>

#include "Core/Memory/Allocators/STLAllocators/STLStackBasedLinearAllocator.h"

class STLStackBasedLinearAllocatorTest : public ::testing::Test
{
public:
    static constexpr int DEFAULT_ALLOCATION_SIZE = 2056;
    Core::STLStackBasedLinearAllocator<char, DEFAULT_ALLOCATION_SIZE> testAllocator;
};

/**@brief Tests if the allocator is allocating the correct amount of space for an allocation */
TEST_F(STLStackBasedLinearAllocatorTest, allocate_AllocatesCorrectAmountOfSpace)
{
    const char* allocatedAddress = testAllocator.allocate(5);
    const char* allocatedAddress2 = testAllocator.allocate(27);
    const char* allocatedAddress3 = testAllocator.allocate(57);

    EXPECT_EQ(allocatedAddress2 - allocatedAddress, 5); // No need to account for alignment with chars
    EXPECT_EQ(allocatedAddress3 - allocatedAddress2, 27);
    EXPECT_EQ(testAllocator.getUsedBlockSize(), 89); // 5 + 27 + 57 == 89
}

/**@brief Tests if the allocator returns addresses that can be read from and written to */
TEST_F(STLStackBasedLinearAllocatorTest, allocate_ReturnsUseableAddresses)
{
    char* allocatedAddress = testAllocator.allocate(5); // allocates 5 chars
    std::strcpy(allocatedAddress, "abcd\0");
    EXPECT_STREQ(allocatedAddress, "abcd\0");

    char* allocatedAddress2 = testAllocator.allocate(27);
    std::strcpy(allocatedAddress2, "abcdefghijklmnopqrstuvwxyz\0");
    EXPECT_STREQ(allocatedAddress2, "abcdefghijklmnopqrstuvwxyz\0");
}

/**@brief Tests if the allocator throws an error if the allocation size is too big */
TEST_F(STLStackBasedLinearAllocatorTest, allocate_ThrowsOnExcessiveAllocationSize)
{
    Core::STLStackBasedLinearAllocator<char, 2056> testAllocator;
    EXPECT_THROW(testAllocator.allocate(3000), std::bad_alloc);
    EXPECT_THROW(testAllocator.allocate(2057), std::bad_alloc);
    EXPECT_NO_THROW(testAllocator.allocate(2056));
}

/**@brief Tests if the allocator throws an error if the allocation size is too big */
TEST_F(STLStackBasedLinearAllocatorTest, allocate_ThrowsOnExcessiveCumulativeAllocationSize)
{
    Core::STLStackBasedLinearAllocator<char, 2200> testAllocator;
    EXPECT_NO_THROW(testAllocator.allocate(300)); // Total Allocation: 300
    EXPECT_NO_THROW(testAllocator.allocate(400)); // Total Allocation: 700
    EXPECT_NO_THROW(testAllocator.allocate(200)); // Total Allocation: 900
    EXPECT_NO_THROW(testAllocator.allocate(700)); // Total Allocation: 1600
    EXPECT_NO_THROW(testAllocator.allocate(500)); // Total Allocation: 2100
    EXPECT_THROW(testAllocator.allocate(250), std::bad_alloc); // Total Allocation: 2350 -> TOO BIG
}


/**@brief Tests if the Reset method correctly resets the state of the allocator back to the start of the memory block */
TEST_F(STLStackBasedLinearAllocatorTest, reset_CorrectlyResetsAllocatorMemoryBlock)
{
    Core::STLStackBasedLinearAllocator<char, 2200> testAllocator;
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
    EXPECT_EQ(testAllocator.getUsedBlockSize(), 250);
}


/**@brief Tests if the Deallocate method does absolutely nothing when called */
TEST_F(STLStackBasedLinearAllocatorTest, deallocate_DoesNothing)
{
    char* memoryAddress = testAllocator.allocate(500);
    testAllocator.deallocate(memoryAddress, 500);
    EXPECT_EQ(testAllocator.getUsedBlockSize(), 500);

    char* memoryAddress2 = testAllocator.allocate(1000);
    testAllocator.deallocate(memoryAddress2, 1000);
    EXPECT_EQ(testAllocator.getUsedBlockSize(), 1500);

    char* memoryAddress3 = testAllocator.allocate(100);
    testAllocator.deallocate(memoryAddress3, 100);
    EXPECT_EQ(testAllocator.getUsedBlockSize(), 1600);
}

/**@brief Tests if the GetUsedBlockSize method is returning the accurate amount of space that is currently allocated by the allocator */
TEST_F(STLStackBasedLinearAllocatorTest, getUsedBlockSize_ReturnsTheCorrectAmountOfSpaceCurrentlyAllocated)
{
    Core::STLStackBasedLinearAllocator<char, 2056> testAllocator;

    testAllocator.allocate(5);
    EXPECT_EQ(testAllocator.getUsedBlockSize(), 5);
    testAllocator.allocate(27);
    EXPECT_EQ(testAllocator.getUsedBlockSize(), 5 + 27);
    testAllocator.allocate(57);
    EXPECT_EQ(testAllocator.getUsedBlockSize(), 5 + 27 + 57);
    testAllocator.reset();
    EXPECT_EQ(testAllocator.getUsedBlockSize(), 0);
    testAllocator.allocate(812);
    EXPECT_EQ(testAllocator.getUsedBlockSize(), 812);
    testAllocator.allocate(11);
    EXPECT_EQ(testAllocator.getUsedBlockSize(), 812 + 11);
    testAllocator.allocate(71);
    EXPECT_EQ(testAllocator.getUsedBlockSize(), 812 + 11 + 71);
}

TEST_F(STLStackBasedLinearAllocatorTest, allocate_RespectsTypeAlignment)
{
    struct alignas(16) AlignedStruct { char data[8]; };
    Core::STLStackBasedLinearAllocator<AlignedStruct, 1024> alignedAllocator;

    AlignedStruct* ptr = alignedAllocator.allocate(1);
    EXPECT_EQ(reinterpret_cast<std::uintptr_t>(ptr) % 16, 0);
}

/**@brief Tests if the copy constructor creates a fully independent allocator with matching state */
TEST_F(STLStackBasedLinearAllocatorTest, CopyConstructor_CopiesStateCorrectly)
{
    char* buffer = (char*)testAllocator.allocate(126);
    std::memcpy(buffer, "This is a test!", 15);

    Core::STLStackBasedLinearAllocator<char, 2056> testAllocator2 = Core::STLStackBasedLinearAllocator<char, 2056>(testAllocator);

    EXPECT_EQ(testAllocator2.getCapacity(), testAllocator.getCapacity());
    EXPECT_EQ(testAllocator2.getUsedBlockSize(), testAllocator.getUsedBlockSize());

}


/**@brief Tests if copy assignment operator clones state and creates independent allocator */
TEST_F(STLStackBasedLinearAllocatorTest, CopyAssignmentOperator_ClonesState)
{
    [[maybe_unused]] char* buffer = (char*)testAllocator.allocate(531);

    Core::STLStackBasedLinearAllocator<char, 2056> testAllocator2 = Core::STLStackBasedLinearAllocator<char, 2056>();
    testAllocator2 = testAllocator;

    EXPECT_EQ(testAllocator2.getCapacity(), testAllocator.getCapacity());
    EXPECT_EQ(testAllocator2.getUsedBlockSize(), testAllocator.getUsedBlockSize());

}

/**@brief Tests if move constructor transfers ownership and invalidates source */
TEST_F(STLStackBasedLinearAllocatorTest, MoveConstructor_TransfersOwnership)
{
    [[maybe_unused]] char* buffer = (char*)testAllocator.allocate(731);

    size_t alloc1Capacity = testAllocator.getCapacity();
    size_t alloc1UsedSize = testAllocator.getUsedBlockSize();
    Core::STLStackBasedLinearAllocator<char, 2056> testAllocator2 = Core::STLStackBasedLinearAllocator<char, 2056>(std::move(testAllocator));

    EXPECT_EQ(testAllocator2.getCapacity(), alloc1Capacity);
    EXPECT_EQ(testAllocator2.getUsedBlockSize(), alloc1UsedSize);

    EXPECT_EQ(testAllocator.getCapacity(), 0);
    EXPECT_EQ(testAllocator.getUsedBlockSize(), 0);
}

/**@brief Tests if move assignment operator transfers ownership */
TEST_F(STLStackBasedLinearAllocatorTest, MoveAssignmentOperator_TransfersOwnership)
{
    [[maybe_unused]]  char* buffer = (char*)testAllocator.allocate(221);

    size_t alloc1Capacity = testAllocator.getCapacity();
    size_t alloc1UsedSize = testAllocator.getUsedBlockSize();

    Core::STLStackBasedLinearAllocator<char, 2056> testAllocator2 = Core::STLStackBasedLinearAllocator<char, 2056>();
    testAllocator2 = std::move(testAllocator);

    EXPECT_EQ(testAllocator2.getCapacity(), alloc1Capacity);
    EXPECT_EQ(testAllocator2.getUsedBlockSize(), alloc1UsedSize);

    EXPECT_EQ(testAllocator.getCapacity(), 0);
    EXPECT_EQ(testAllocator.getUsedBlockSize(), 0);
}