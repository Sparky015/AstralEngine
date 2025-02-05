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

/**@brief Tests if the IsEmpty method is returning the accurate amount of space that is currently allocated by the allocator */
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

/**@brief Tests if rebind-shared allocators maintain the same underlying state across copies. */
TEST_F(STLStackBasedLinearAllocatorTest, rebind_SharesStateBetweenInstances)
{
    // Allocate some memory using the original testAllocator
    // We'll allocate 100 chars (bytes) to ensure the allocator's used size increases
    char* initialAllocation = testAllocator.allocate(100);
    std::memset(initialAllocation, 1, 100); // Fill with some data just to show it's allocated
    EXPECT_EQ(testAllocator.getUsedBlockSize(), 100);

    // Create a rebind-based allocator (switching from char to int, for example) using the same underlying memory
    Core::STLStackBasedLinearAllocator<char, 2056>::rebind<int>::other rebindAllocator{testAllocator};

    // Allocate memory using the rebind-based allocator
    // This allocation should increase the used block size of both allocators since they share the same memory
    [[maybe_unused]] int* reboundAllocation = rebindAllocator.allocate(5);
    EXPECT_EQ(testAllocator.getUsedBlockSize(), 100 + 5 * sizeof(int))
                        << "Used block size should account for additional int memory allocated by rebind";

    // Verify that both allocators now report the same used block size
    EXPECT_EQ(rebindAllocator.getUsedBlockSize(), testAllocator.getUsedBlockSize())
                        << "The rebind allocator and original allocator should share the same used memory";

    // Allocate more memory using the original allocator
    char* secondAllocation = testAllocator.allocate(20);
    std::memset(secondAllocation, 2, 20); // Fill new allocation with different data
    EXPECT_EQ(testAllocator.getUsedBlockSize(), (100 + 5 * sizeof(int) + 20))
                        << "Used block size should account for the total memory allocated so far";

    // The rebind-based allocator should also see the updated used block size
    EXPECT_EQ(rebindAllocator.getUsedBlockSize(), testAllocator.getUsedBlockSize())
                        << "Rebind allocator should reflect the same used size after further allocations by the original allocator";

    // Reset using the rebind-based allocator. This should reset the underlying memory block both allocators share.
    rebindAllocator.reset();
    EXPECT_EQ(testAllocator.getUsedBlockSize(), 0)
                        << "Both allocators should show 0 used block size after the reset on one of them";
    EXPECT_EQ(rebindAllocator.getUsedBlockSize(), 0)
                        << "Rebind allocator should also show a used block size of 0 after the reset";
}