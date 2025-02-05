/**
* @file DoubleBufferedAllocatorTest.cpp
* @author Andrew Fagan
* @date 1/21/2025
*/

#include "Core/Memory/Allocators/DoubleBufferedAllocator.h"
#include <gtest/gtest.h>
#include <cstring>

class DoubleBufferedAllocatorTest : public ::testing::Test
{
public:
    static constexpr int DEFAULT_ALLOCATION_SIZE = 2056;
    Core::DoubleBufferedAllocator testAllocator = Core::DoubleBufferedAllocator(DEFAULT_ALLOCATION_SIZE);
};

/**@brief Tests if the allocator is allocating the correct amount of space for an allocation */
TEST_F(DoubleBufferedAllocatorTest, Allocate_AllocatesCorrectAmountOfSpace)
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
TEST_F(DoubleBufferedAllocatorTest, Allocate_ReturnsUseableAddresses)
{
    char* allocatedAddress = (char*) testAllocator.Allocate(5, alignof(char)); // allocates 5 chars
    std::strcpy(allocatedAddress, "abcd\0");
    EXPECT_STREQ(allocatedAddress, "abcd\0");

    char* allocatedAddress2 = (char*) testAllocator.Allocate(27, alignof(char));
    std::strcpy(allocatedAddress2, "abcdefghijklmnopqrstuvwxyz\0");
    EXPECT_STREQ(allocatedAddress2, "abcdefghijklmnopqrstuvwxyz\0");
}

/**@brief Tests if the allocator throws an error if the allocation size is too big */
TEST_F(DoubleBufferedAllocatorTest, Allocate_ThrowsOnExcessiveAllocationSize)
{
    Core::DoubleBufferedAllocator testAllocator = Core::DoubleBufferedAllocator(2056);
    EXPECT_EQ(testAllocator.Allocate(3000, alignof(char)), nullptr);
    EXPECT_EQ(testAllocator.Allocate(2057, alignof(char)), nullptr);
    EXPECT_NE(testAllocator.Allocate(2056, alignof(char)), nullptr);
}

/**@brief Tests if the allocator throws an error if the allocation size is too big */
TEST_F(DoubleBufferedAllocatorTest, Allocate_ThrowsOnExcessiveCumulativeAllocationSize)
{
    Core::DoubleBufferedAllocator testAllocator = Core::DoubleBufferedAllocator(2200);
    EXPECT_NE(testAllocator.Allocate(300, alignof(char)), nullptr);    // Total: 300
    EXPECT_NE(testAllocator.Allocate(400, alignof(char)), nullptr);    // Total: 700
    EXPECT_NE(testAllocator.Allocate(200, alignof(char)), nullptr);    // Total: 900
    EXPECT_NE(testAllocator.Allocate(700, alignof(char)), nullptr);    // Total: 1600
    EXPECT_NE(testAllocator.Allocate(500, alignof(char)), nullptr);    // Total: 2100
    EXPECT_EQ(testAllocator.Allocate(250, alignof(char)), nullptr);    // Total: 2350 -> TOO BIG
}

/**@brief Tests if the natural alignment is applied when different alignment requirements request allocations */
TEST_F(DoubleBufferedAllocatorTest, Allocate_RespectsMultipleTypesAlignment)
{
    struct alignas(16) AlignedStruct { char data[8]; };
    Core::DoubleBufferedAllocator alignedAllocator = Core::DoubleBufferedAllocator(500);

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

/**@brief Tests if the ClearCurrentBuffer method correctly resets the state of the allocator back to the start of the memory block */
TEST_F(DoubleBufferedAllocatorTest, ClearCurrentBuffer_CorrectlyResetsAllocatorMemoryBlock)
{
    Core::DoubleBufferedAllocator testAllocator = Core::DoubleBufferedAllocator(2200);
    EXPECT_NE(testAllocator.Allocate(300, alignof(char)), nullptr); // Total Allocation: 300
    EXPECT_NE(testAllocator.Allocate(400, alignof(char)), nullptr); // Total Allocation: 700
    EXPECT_NE(testAllocator.Allocate(200, alignof(char)), nullptr); // Total Allocation: 900
    EXPECT_NE(testAllocator.Allocate(700, alignof(char)), nullptr); // Total Allocation: 1600
    EXPECT_NE(testAllocator.Allocate(500, alignof(char)), nullptr); // Total Allocation: 2100
    testAllocator.ClearCurrentBuffer(); // Total Allocation: 0

    EXPECT_NE(testAllocator.Allocate(300, alignof(char)), nullptr); // Total Allocation: 300
    EXPECT_NE(testAllocator.Allocate(400, alignof(char)), nullptr); // Total Allocation: 700
    EXPECT_NE(testAllocator.Allocate(200, alignof(char)), nullptr); // Total Allocation: 900
    EXPECT_NE(testAllocator.Allocate(700, alignof(char)), nullptr); // Total Allocation: 1600
    EXPECT_NE(testAllocator.Allocate(500, alignof(char)), nullptr); // Total Allocation: 2100
    testAllocator.ClearCurrentBuffer(); // Total Allocation: 0

    EXPECT_NE(testAllocator.Allocate(250, alignof(char)), nullptr); // Total Allocation: 250
    EXPECT_EQ(testAllocator.GetUsedBlockSize(), 250);
}

/**@brief Tests if the IsEmpty method is returning the accurate amount of space that is currently allocated by the allocator */
TEST_F(DoubleBufferedAllocatorTest, GetUsedBlockSize_ReturnsTheCorrectAmountOfSpaceCurrentlyAllocated)
{
    Core::DoubleBufferedAllocator testAllocator = Core::DoubleBufferedAllocator(2056);

    testAllocator.Allocate(5, alignof(char));
    EXPECT_EQ(testAllocator.GetUsedBlockSize(), 5); // allocation size + 1 for header
    testAllocator.Allocate(27, alignof(char));
    EXPECT_EQ(testAllocator.GetUsedBlockSize(), 5 + 27);
    testAllocator.Allocate(57, alignof(char));
    EXPECT_EQ(testAllocator.GetUsedBlockSize(), 5 + 27 + 57);
    testAllocator.ClearCurrentBuffer();
    EXPECT_EQ(testAllocator.GetUsedBlockSize(), 0);
    testAllocator.Allocate(812, alignof(char));
    EXPECT_EQ(testAllocator.GetUsedBlockSize(), 812);
    testAllocator.Allocate(11, alignof(char));
    EXPECT_EQ(testAllocator.GetUsedBlockSize(), 812 + 11);
    testAllocator.Allocate(71, alignof(char));
    EXPECT_EQ(testAllocator.GetUsedBlockSize(), 812 + 11 + 71);
}

/**@brief Tests if the natural alignment is applied to allocations */
TEST_F(DoubleBufferedAllocatorTest, Allocate_RespectsTypeAlignment)
{
    struct alignas(16) AlignedStruct { char data[8]; };
    Core::DoubleBufferedAllocator alignedAllocator = Core::DoubleBufferedAllocator(128);

    AlignedStruct* ptr = (AlignedStruct*) alignedAllocator.Allocate(2, alignof(AlignedStruct));
    EXPECT_EQ(reinterpret_cast<std::uintptr_t>(ptr) % 16, 0);
    AlignedStruct* ptr2 = (AlignedStruct*) alignedAllocator.Allocate(3, alignof(AlignedStruct));
    EXPECT_EQ(reinterpret_cast<std::uintptr_t>(ptr2) % 16, 0);
}

/**@brief Tests if the rollback marker moves back to the correct spot */
TEST_F(DoubleBufferedAllocatorTest, RollbackToMarker_RollsBackToTheCorrectSpot)
{
    Core::DoubleBufferedAllocator allocator = Core::DoubleBufferedAllocator(500);
    Core::DoubleBufferedAllocator::Marker bottomMarker = allocator.GetMarker();

    allocator.Allocate(sizeof(int) * 5, alignof(int));
    allocator.Allocate(sizeof(char) * 3, alignof(char));

    int size1 = allocator.GetUsedBlockSize();
    Core::DoubleBufferedAllocator::Marker marker1 = allocator.GetMarker();

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

/**@brief Tests if swap buffer method maintains the state of the swapped buffer */
TEST_F(DoubleBufferedAllocatorTest, SwapBuffer_MaintainsBlockStateAfterSwap)
{
    Core::DoubleBufferedAllocator allocator = Core::DoubleBufferedAllocator(500);

    allocator.Allocate(sizeof(int) * 4, alignof(int));
    int* address = (int*)allocator.Allocate(sizeof(int) * 3, alignof(int));
    *address = 2958211;
    Core::DoubleBufferedAllocator::Marker marker1 = allocator.GetMarker();
    int size1 = allocator.GetUsedBlockSize();

    allocator.SwapBuffers();

    allocator.Allocate(sizeof(char) * 3, alignof(char));
    int* address2 = (int*)allocator.Allocate(sizeof(int) * 3, alignof(int));
    *address2 = 2222931;
    Core::DoubleBufferedAllocator::Marker marker2 = allocator.GetMarker();
    int size2 = allocator.GetUsedBlockSize();

    allocator.SwapBuffers();

    EXPECT_EQ(size1, allocator.GetUsedBlockSize());
    EXPECT_EQ(marker1, allocator.GetMarker());
    EXPECT_EQ(*address, 2958211);

    allocator.ClearCurrentBuffer();
    allocator.SwapBuffers();

    EXPECT_EQ(size2, allocator.GetUsedBlockSize());
    EXPECT_EQ(marker2, allocator.GetMarker());
    EXPECT_EQ(*address2, 2222931);
}

/**@brief Tests if the move constructor properly transfers ownership of both buffers */
TEST_F(DoubleBufferedAllocatorTest, MoveConstructor_TransfersOwnershipCorrectly)
{
    // Setup some data in both buffers
    [[maybe_unused]] char* buffer1Data = (char*)testAllocator.Allocate(521, alignof(char));
    std::strcpy(buffer1Data, "First Allocation");
    testAllocator.SwapBuffers();
    [[maybe_unused]] char* buffer2Data = (char*)testAllocator.Allocate(128, alignof(char));
    std::strcpy(buffer2Data, "Second Allocation");

    // Create second allocator and move assign
    Core::DoubleBufferedAllocator secondAllocator(std::move(testAllocator));

    // Verify moved allocator has correct data
    EXPECT_EQ(secondAllocator.GetUsedBlockSize(), 128);
    secondAllocator.SwapBuffers();
    EXPECT_EQ(secondAllocator.GetUsedBlockSize(), 521);
}

/**@brief Tests if the move assignment operator properly transfers ownership of both buffers */
TEST_F(DoubleBufferedAllocatorTest, MoveAssignment_TransfersOwnershipCorrectly)
{
    // Setup some data in both buffers
    [[maybe_unused]] char* buffer1Data = (char*)testAllocator.Allocate(119, alignof(char));
    std::strcpy(buffer1Data, "First Allocation");
    testAllocator.SwapBuffers();
    [[maybe_unused]] char* buffer2Data = (char*)testAllocator.Allocate(132, alignof(char));
    std::strcpy(buffer2Data, "Second Allocation");

    // Create second allocator and move assign
    Core::DoubleBufferedAllocator secondAllocator(1024);
    secondAllocator = std::move(testAllocator);

    // Verify moved allocator has correct data
    EXPECT_EQ(secondAllocator.GetUsedBlockSize(), 132);
    secondAllocator.SwapBuffers();
    EXPECT_EQ(secondAllocator.GetUsedBlockSize(), 119);
}

/** @brief Tests if the method resizes the memory block by doubling the capacity of the allocator. */
TEST_F(DoubleBufferedAllocatorTest, ResizeMemoryBlock_CorrectlyResizesToCorrectSize)
{
    size_t currentCapacity = testAllocator.GetActiveBufferCapacity();
    size_t currentUsedSize = testAllocator.GetUsedBlockSize();

    EXPECT_EQ(currentUsedSize, 0);
    EXPECT_TRUE(testAllocator.ResizeActiveBuffer());

    EXPECT_EQ(testAllocator.GetUsedBlockSize(), 0);
    EXPECT_EQ(testAllocator.GetActiveBufferCapacity(), currentCapacity * 2);
}