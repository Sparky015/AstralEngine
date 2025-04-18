//
// Created by Andrew Fagan on 1/15/25.
//

#include <gtest/gtest.h>
#include <cstring>

#include "Core/Memory/Allocators/StackAllocator.h"

class StackAllocatorTest : public ::testing::Test
{
public:
    static constexpr int DEFAULT_ALLOCATION_SIZE = 2056;
    Core::StackAllocator testAllocator = Core::StackAllocator(DEFAULT_ALLOCATION_SIZE);
};

/**@brief Tests if the allocator is allocating the correct amount of space for an allocation */
TEST_F(StackAllocatorTest, Allocate_AllocatesCorrectAmountOfSpace)
{
    const char* allocatedAddress = (const char*) testAllocator.Allocate(5, alignof(char));
    const char* allocatedAddress2 = (const char*) testAllocator.Allocate(27, alignof(char));
    const char* allocatedAddress3 = (const char*) testAllocator.Allocate(57, alignof(char));

    EXPECT_EQ(allocatedAddress2 - allocatedAddress, 5 + 1); // + 1 for allocation header
    EXPECT_EQ(allocatedAddress3 - allocatedAddress2, 27 + 1);

    //  +1 for each allocation header because char is naturally already aligned, so the allocate method will push it back to fit the header
    EXPECT_EQ(testAllocator.GetUsedBlockSize(), 5 + 27 + 57 + 3); // 5 + 27 + 57 == 89
}

/**@brief Tests if the allocator returns addresses that can be read from and written to */
TEST_F(StackAllocatorTest, Allocate_ReturnsUseableAddresses)
{
    char* allocatedAddress = (char*) testAllocator.Allocate(5, alignof(char)); // allocates 5 chars
    std::strcpy(allocatedAddress, "abcd\0");
    EXPECT_STREQ(allocatedAddress, "abcd\0");

    char* allocatedAddress2 = (char*) testAllocator.Allocate(27, alignof(char));
    std::strcpy(allocatedAddress2, "abcdefghijklmnopqrstuvwxyz\0");
    EXPECT_STREQ(allocatedAddress2, "abcdefghijklmnopqrstuvwxyz\0");
}

/**@brief Tests if the allocator throws an error if the allocation size is too big */
TEST_F(StackAllocatorTest, Allocate_ThrowsOnExcessiveAllocationSize)
{
    Core::StackAllocator testAllocator = Core::StackAllocator(2056);
    EXPECT_EQ(testAllocator.Allocate(3000, alignof(char)), nullptr);
    EXPECT_EQ(testAllocator.Allocate(2057, alignof(char)), nullptr);
    EXPECT_NE(testAllocator.Allocate(2055, alignof(char)), nullptr); // Minus one of mem block size for allocation header
}

/**@brief Tests if the allocator throws an error if the allocation size is too big */
TEST_F(StackAllocatorTest, Allocate_ThrowsOnExcessiveCumulativeAllocationSize)
{
    Core::StackAllocator testAllocator = Core::StackAllocator(2200);
    EXPECT_NE(testAllocator.Allocate(300, alignof(char)), nullptr);    // Total: 300
    EXPECT_NE(testAllocator.Allocate(400, alignof(char)), nullptr);    // Total: 700
    EXPECT_NE(testAllocator.Allocate(200, alignof(char)), nullptr);    // Total: 900
    EXPECT_NE(testAllocator.Allocate(700, alignof(char)), nullptr);    // Total: 1600
    EXPECT_NE(testAllocator.Allocate(500, alignof(char)), nullptr);    // Total: 2100
    EXPECT_EQ(testAllocator.Allocate(250, alignof(char)), nullptr);    // Total: 2350 -> TOO BIG
}


/**@brief Tests if the Reset method correctly resets the state of the allocator back to the start of the memory block */
TEST_F(StackAllocatorTest, Reset_CorrectlyResetsAllocatorMemoryBlock)
{
    Core::StackAllocator testAllocator = Core::StackAllocator(2200);
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
    EXPECT_EQ(testAllocator.GetUsedBlockSize(), 250 + 1);
}


/**@brief Tests if the Deallocate method frees the previous allocation that was made */
TEST_F(StackAllocatorTest, Deallocate_ReleasesPreviousAllocationMemory)
{
    char* memoryAddress = (char*) testAllocator.Allocate(500, alignof(char));
    testAllocator.Deallocate(memoryAddress, 500);
    EXPECT_EQ(testAllocator.GetUsedBlockSize(), 0);

    char* memoryAddress2 = (char*) testAllocator.Allocate(500, alignof(char));
    char* memoryAddress3 = (char*) testAllocator.Allocate(1000, alignof(char));
    EXPECT_EQ(testAllocator.GetUsedBlockSize(), 500 + 1000 + 2); // +2 for allocation headers (1 for each allocation)
    testAllocator.Deallocate(memoryAddress3, 1000);
    EXPECT_EQ(testAllocator.GetUsedBlockSize(), 500 + 1);
    testAllocator.Deallocate(memoryAddress2, 500);
    EXPECT_EQ(testAllocator.GetUsedBlockSize(), 0);

    char* memoryAddress4 = (char*) testAllocator.Allocate(500, alignof(char));
    char* memoryAddress5 = (char*) testAllocator.Allocate(250, alignof(char));
    char* memoryAddress6 = (char*) testAllocator.Allocate(700, alignof(char));
    EXPECT_EQ(testAllocator.GetUsedBlockSize(), 500 + 250 + 700 + 3);
    testAllocator.Deallocate(memoryAddress6, 700);
    EXPECT_EQ(testAllocator.GetUsedBlockSize(), 500 + 250 + 2);
    testAllocator.Deallocate(memoryAddress5, 250);
    EXPECT_EQ(testAllocator.GetUsedBlockSize(), 500 + 1);
    testAllocator.Deallocate(memoryAddress4, 500);
    EXPECT_EQ(testAllocator.GetUsedBlockSize(), 0);
}

/**@brief Tests if the GetUsedBlockSize method is returning the accurate amount of space that is currently allocated by the allocator */
TEST_F(StackAllocatorTest, GetUsedBlockSize_ReturnsTheCorrectAmountOfSpaceCurrentlyAllocated)
{
    Core::StackAllocator testAllocator = Core::StackAllocator(2056);

    testAllocator.Allocate(5, alignof(char));
    EXPECT_EQ(testAllocator.GetUsedBlockSize(), 5 + 1); // allocation size + 1 for header
    testAllocator.Allocate(27, alignof(char));
    EXPECT_EQ(testAllocator.GetUsedBlockSize(), 5 + 27 + 2);
    testAllocator.Allocate(57, alignof(char));
    EXPECT_EQ(testAllocator.GetUsedBlockSize(), 5 + 27 + 57 + 3);
    testAllocator.Reset();
    EXPECT_EQ(testAllocator.GetUsedBlockSize(), 0);
    testAllocator.Allocate(812, alignof(char));
    EXPECT_EQ(testAllocator.GetUsedBlockSize(), 812 + 1);
    testAllocator.Allocate(11, alignof(char));
    EXPECT_EQ(testAllocator.GetUsedBlockSize(), 812 + 11 + 2);
    testAllocator.Allocate(71, alignof(char));
    EXPECT_EQ(testAllocator.GetUsedBlockSize(), 812 + 11 + 71 + 3);
}

/**@brief Tests if the natural alignment is applied to allocations */
TEST_F(StackAllocatorTest, Allocate_RespectsTypeAlignment)
{
    struct alignas(16) AlignedStruct { char data[8]; };
    Core::StackAllocator alignedAllocator = Core::StackAllocator(128);

    AlignedStruct* ptr = (AlignedStruct*) alignedAllocator.Allocate(2, alignof(AlignedStruct));
    EXPECT_EQ(reinterpret_cast<std::uintptr_t>(ptr) % 16, 0);
    AlignedStruct* ptr2 = (AlignedStruct*) alignedAllocator.Allocate(3, alignof(AlignedStruct));
    EXPECT_EQ(reinterpret_cast<std::uintptr_t>(ptr2) % 16, 0);
}

/**@brief Tests if the natural alignment is applied when different alignment requirements request allocations */
TEST_F(StackAllocatorTest, Allocate_RespectsMultipleTypesAlignment)
{
    struct alignas(16) AlignedStruct { char data[8]; };
    Core::StackAllocator alignedAllocator = Core::StackAllocator(500);

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
TEST_F(StackAllocatorTest, RollbackToMarker_RollsBackToTheCorrectSpot)
{
    Core::StackAllocator allocator = Core::StackAllocator(500);

    Core::StackAllocator::Marker bottomMarker = allocator.GetMarker();

    allocator.Allocate(sizeof(int) * 5, alignof(int));
    allocator.Allocate(sizeof(char) * 3, alignof(char));

    int size1 = allocator.GetUsedBlockSize();
    Core::StackAllocator::Marker marker1 = allocator.GetMarker();

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

/**@brief Tests if move constructor transfers ownership and invalidates source */
TEST_F(StackAllocatorTest, MoveConstructor_TransfersOwnership)
{
    [[maybe_unused]]  char* buffer = (char*)testAllocator.Allocate(731, alignof(char));

    size_t alloc1Capacity = testAllocator.GetCapacity();
    size_t alloc1UsedSize = testAllocator.GetUsedBlockSize();
    Core::StackAllocator::Marker alloc1CurrentMarker = testAllocator.GetMarker();
    Core::StackAllocator testAllocator2 = Core::StackAllocator(std::move(testAllocator));


    EXPECT_EQ(testAllocator2.GetCapacity(), alloc1Capacity);
    EXPECT_EQ(testAllocator2.GetUsedBlockSize(), alloc1UsedSize);
    EXPECT_EQ(testAllocator2.GetMarker(), alloc1CurrentMarker);

    EXPECT_EQ(testAllocator.GetCapacity(), 0);
    EXPECT_EQ(testAllocator.GetUsedBlockSize(), 0);

    // Find the relative original ptr location
    unsigned char* originalPtr = testAllocator2.GetMarker() - 731;

    // Should be able to reverse the allocation with no errors
    EXPECT_NO_THROW(testAllocator2.Deallocate(originalPtr, 731));
}

/**@brief Tests if move assignment operator transfers ownership */
TEST_F(StackAllocatorTest, MoveAssignmentOperator_TransfersOwnership)
{
    [[maybe_unused]]  char* buffer = (char*)testAllocator.Allocate(221, alignof(char));

    size_t alloc1Capacity = testAllocator.GetCapacity();
    size_t alloc1UsedSize = testAllocator.GetUsedBlockSize();
    Core::StackAllocator::Marker alloc1CurrentMarker = testAllocator.GetMarker();
    Core::StackAllocator testAllocator2 = Core::StackAllocator(12);

    testAllocator2 = std::move(testAllocator);

    EXPECT_EQ(testAllocator2.GetCapacity(), alloc1Capacity);
    EXPECT_EQ(testAllocator2.GetUsedBlockSize(), alloc1UsedSize);
    EXPECT_EQ(testAllocator2.GetMarker(), alloc1CurrentMarker);

    EXPECT_EQ(testAllocator.GetCapacity(), 0);
    EXPECT_EQ(testAllocator.GetUsedBlockSize(), 0);

    // Find the relative original ptr location
    unsigned char* originalPtr = testAllocator2.GetMarker() - 221;

    // Should be able to reverse the allocation with no errors
    EXPECT_NO_THROW(testAllocator2.Deallocate(originalPtr, 221));
}

/** @brief Tests if the method resizes the memory block by doubling the capacity of the allocator. */
TEST_F(StackAllocatorTest, ResizeMemoryBlock_CorrectlyResizesToCorrectSize)
{
    size_t currentCapacity = testAllocator.GetCapacity();
    size_t currentUsedSize = testAllocator.GetUsedBlockSize();

    EXPECT_EQ(currentUsedSize, 0);
    EXPECT_TRUE(testAllocator.ResizeBuffer());

    EXPECT_EQ(testAllocator.GetUsedBlockSize(), 0);
    EXPECT_EQ(testAllocator.GetCapacity(), currentCapacity * 2);
}