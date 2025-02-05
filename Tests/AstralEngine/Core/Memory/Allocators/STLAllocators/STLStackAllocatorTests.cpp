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

/**@brief Tests if the IsEmpty method is returning the accurate amount of space that is currently allocated by the allocator */
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

/**@brief Tests if the copy constructor creates a fully independent allocator with matching state */
TEST_F(STLStackAllocatorTest, CopyConstructor_CopiesSharedStateCorrectly)
{
    char* buffer = (char*)testAllocator.allocate(126);
    std::memcpy(buffer, "This is a test!", 15);

    Core::STLStackAllocator<char> testAllocator2 = Core::STLStackAllocator<char>(testAllocator);

    EXPECT_EQ(testAllocator2.getCapacity(), testAllocator.getCapacity());
    EXPECT_EQ(testAllocator2.getUsedBlockSize(), testAllocator.getUsedBlockSize());
    EXPECT_EQ(testAllocator2.GetMarker(), testAllocator.GetMarker());

    // Find the relative original ptr location
    char* originalPtr = (char*)testAllocator2.GetMarker() - 126;

    // Should be able to reverse the allocation with no errors
    EXPECT_NO_THROW(testAllocator2.deallocate(originalPtr, 126));
}


/**@brief Tests if copy assignment operator clones state and creates independent allocator */
TEST_F(STLStackAllocatorTest, CopyAssignmentOperator_ClonesSharedState)
{
    [[maybe_unused]] char* buffer = (char*)testAllocator.allocate(531);

    Core::STLStackAllocator<char> testAllocator2 = Core::STLStackAllocator<char>(12);
    testAllocator2 = testAllocator;

    EXPECT_EQ(testAllocator2.getCapacity(), testAllocator.getCapacity());
    EXPECT_EQ(testAllocator2.getUsedBlockSize(), testAllocator.getUsedBlockSize());
    EXPECT_EQ(testAllocator2.GetMarker(), testAllocator.GetMarker());

    // Find the relative original ptr location
    char* originalPtr = (char*)testAllocator2.GetMarker() - 531;

    // Should be able to reverse the allocation with no errors
    EXPECT_NO_THROW(testAllocator2.deallocate(originalPtr, 531));
}

/**@brief Tests if rebind-shared allocators maintain the same underlying state across copies. */
TEST_F(STLStackAllocatorTest, rebind_SharesStateBetweenInstances)
{
    // Allocate some memory using the original testAllocator
    // We'll allocate 100 chars (bytes) to ensure the allocator's used size increases
    char* initialAllocation = testAllocator.allocate(100);
    std::memset(initialAllocation, 1, 100); // Fill with some data just to show it's allocated
    EXPECT_EQ(testAllocator.getUsedBlockSize(), 100 + 1); // +1 for allocations header

    // Create a rebind-based allocator (switching from char to int, for example) using the same underlying memory
    Core::STLStackAllocator<char>::rebind<int>::other rebindAllocator{testAllocator};

    // Allocate memory using the rebind-based allocator
    // This allocation should increase the used block size of both allocators since they share the same memory
    [[maybe_unused]] int* reboundAllocation = rebindAllocator.allocate(5);

    // +1 & +3 for allocation headers (+1 offset for char and +3 in this case for int)
    EXPECT_EQ(testAllocator.getUsedBlockSize(), 100 + 5 * sizeof(int) + 4)
                        << "Used block size should account for additional int memory allocated by rebind";

    // Verify that both allocators now report the same used block size
    EXPECT_EQ(rebindAllocator.getUsedBlockSize(), testAllocator.getUsedBlockSize())
                        << "The rebind allocator and original allocator should share the same used memory";

    // Allocate more memory using the original allocator
    char* thirdAllocation = testAllocator.allocate(20);
    std::memset(thirdAllocation, 2, 20); // Fill new allocation with different data

    // +1 & +3 & +1 for allocation headers (+1 offset for char and +3 in this case for int)
    EXPECT_EQ(testAllocator.getUsedBlockSize(), (100 + 5 * sizeof(int) + 20) + 5)
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