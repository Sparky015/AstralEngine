/**
* @file RingAllocatorTest.cpp
* @author Andrew Fagan
* @date 2/5/25
*/

#include "Core/Memory/Allocators/RingAllocator.h"
#include <gtest/gtest.h>
#include <cstring>

class RingAllocatorTest : public ::testing::Test
{
public:
    static constexpr int DEFAULT_ALLOCATION_SIZE = 2056;
    Astral::RingAllocator testAllocator = Astral::RingAllocator(DEFAULT_ALLOCATION_SIZE);
};

/**@brief Tests if the allocator is allocating the correct amount of space and maintains proper spacing */
TEST_F(RingAllocatorTest, Allocate_AllocatesCorrectAmountOfSpace)
{
    const char* allocatedAddress = (const char*) testAllocator.Allocate(5, alignof(char));
    const char* allocatedAddress2 = (const char*) testAllocator.Allocate(27, alignof(char));
    const char* allocatedAddress3 = (const char*) testAllocator.Allocate(57, alignof(char));

    EXPECT_EQ(allocatedAddress2 - allocatedAddress, 5);
    EXPECT_EQ(allocatedAddress3 - allocatedAddress2, 27);
}

/**@brief Tests if the allocator properly wraps and continues allocation from start */
TEST_F(RingAllocatorTest, Allocate_WrapsAndContinuesAllocation)
{
    // Remember start address from first allocation
    void* startAddr = testAllocator.Allocate(1, alignof(char));
    testAllocator.Reset();

    // Fill most of the buffer
    size_t firstSize = DEFAULT_ALLOCATION_SIZE - 100;
    void* first = testAllocator.Allocate(firstSize, alignof(char));
    EXPECT_NE(first, nullptr);

    // This allocation should wrap to start
    void* wrapped = testAllocator.Allocate(101, alignof(char));
    EXPECT_NE(wrapped, nullptr);
    EXPECT_EQ(wrapped, startAddr);  // Should match our saved start address
}

/**@brief Tests if wrapped allocations maintain data integrity */
TEST_F(RingAllocatorTest, Allocate_WrappedAllocationsAreUseable)
{
    // Fill most of the buffer
    size_t firstSize = DEFAULT_ALLOCATION_SIZE - 100;
    void* first = testAllocator.Allocate(firstSize, alignof(char));
    EXPECT_NE(first, nullptr);

    // Create wrapped allocation
    char* wrapped = (char*)testAllocator.Allocate(50, alignof(char));
    EXPECT_NE(wrapped, nullptr);

    // Verify we can write to and read from wrapped allocation
    strcpy(wrapped, "test string");
    EXPECT_STREQ(wrapped, "test string");
}

/**@brief Tests that allocation fails if requested size exceeds total capacity */
TEST_F(RingAllocatorTest, Allocate_FailsWhenSizeExceedsCapacity)
{
    EXPECT_THROW((void)testAllocator.Allocate(DEFAULT_ALLOCATION_SIZE + 1, alignof(char)), std::runtime_error);
    EXPECT_NO_THROW((void)testAllocator.Allocate(DEFAULT_ALLOCATION_SIZE, alignof(char)));
}

/**@brief Tests that DoesAllocationWrap correctly identifies wrapping cases */
TEST_F(RingAllocatorTest, DoesAllocationWrap_CorrectlyIdentifiesWrapping)
{
    size_t almostFull = DEFAULT_ALLOCATION_SIZE - 100;
    void* first = testAllocator.Allocate(almostFull, alignof(char));
    EXPECT_NE(first, nullptr);

    // Test both size and alignment cases
    EXPECT_TRUE(testAllocator.DoesAllocationWrap(150, alignof(char)));  // Should wrap due to size
    EXPECT_FALSE(testAllocator.DoesAllocationWrap(50, alignof(char)));  // Should fit without wrap

    // Test alignment forcing wrap
    EXPECT_TRUE(testAllocator.DoesAllocationWrap(90, 64));  // Should wrap due to alignment
}

/**@brief Tests if the natural alignment is maintained even after wrapping */
TEST_F(RingAllocatorTest, Allocate_MaintainsAlignmentAfterWrap)
{
    struct alignas(16) AlignedStruct { char data[8]; };

    // Fill most of buffer
    size_t almostFull = DEFAULT_ALLOCATION_SIZE - 50;
    void* first = testAllocator.Allocate(almostFull, alignof(char));
    EXPECT_NE(first, nullptr);

    // Allocate aligned struct after wrap
    AlignedStruct* aligned = (AlignedStruct*)testAllocator.Allocate(
            sizeof(AlignedStruct), alignof(AlignedStruct));
    EXPECT_NE(aligned, nullptr);
    EXPECT_EQ(reinterpret_cast<std::uintptr_t>(aligned) % 16, 0);
}

/**@brief Tests if Reset properly clears the allocator state */
TEST_F(RingAllocatorTest, Reset_ClearsAllocatorState)
{
    void* first = testAllocator.Allocate(1000, alignof(char));
    EXPECT_NE(first, nullptr);

    testAllocator.Reset();

    // Should be able to allocate full size after reset
    void* afterReset = testAllocator.Allocate(DEFAULT_ALLOCATION_SIZE, alignof(char));
    EXPECT_NE(afterReset, nullptr);
}

/**@brief Tests if move operations properly transfer ownership */
TEST_F(RingAllocatorTest, MoveOperations_TransferOwnership)
{
    void* first = testAllocator.Allocate(100, alignof(char));
    EXPECT_NE(first, nullptr);

    size_t originalCapacity = testAllocator.GetCapacity();

    // Move constructor
    Astral::RingAllocator moved(std::move(testAllocator));
    EXPECT_EQ(moved.GetCapacity(), originalCapacity);

    // Original should be invalidated
    EXPECT_EQ(testAllocator.GetCapacity(), 0);
}

/**@brief Tests buffer resizing behavior, verifying that resize always works since data is temporary */
TEST_F(RingAllocatorTest, ResizeBuffer_AlwaysWorksWithTemporaryData)
{
    // Fill buffer with some data
    void* allocation1 = testAllocator.Allocate(100, alignof(char));
    EXPECT_NE(allocation1, nullptr);

    // Should work even with data in buffer since data is temporary
    EXPECT_TRUE(testAllocator.ResizeBuffer());
    EXPECT_EQ(testAllocator.GetCapacity(), DEFAULT_ALLOCATION_SIZE * 2);

    // Fill new larger buffer
    void* allocation2 = testAllocator.Allocate(DEFAULT_ALLOCATION_SIZE + 100, alignof(char));
    EXPECT_NE(allocation2, nullptr);

    // Should be able to resize again
    EXPECT_TRUE(testAllocator.ResizeBuffer());
    EXPECT_EQ(testAllocator.GetCapacity(), DEFAULT_ALLOCATION_SIZE * 4);

    // Previous allocations should be considered invalid after resize
    // but new allocations should work with new size
    void* allocation3 = testAllocator.Allocate(DEFAULT_ALLOCATION_SIZE * 2, alignof(char));
    EXPECT_NE(allocation3, nullptr);
}

/**@brief Tests multiple wrap-around scenarios */
TEST_F(RingAllocatorTest, Allocate_HandlesMultipleWraps)
{
    const size_t smallSize = 100;
    std::vector<void*> allocations;

    // Fill buffer multiple times
    for(int i = 0; i < 5; i++) {
        size_t almostFull = DEFAULT_ALLOCATION_SIZE - smallSize;
        void* large = testAllocator.Allocate(almostFull, alignof(char));
        EXPECT_NE(large, nullptr);

        void* small = testAllocator.Allocate(smallSize/2, alignof(char));
        EXPECT_NE(small, nullptr);
        EXPECT_TRUE(testAllocator.DoesAllocationWrap(almostFull, alignof(char)));
    }
}

/**@brief Tests edge case allocation sizes */
TEST_F(RingAllocatorTest, Allocate_HandlesEdgeCaseSizes)
{
    // Zero size allocation should succeed but return valid pointer
    void* zeroSize = testAllocator.Allocate(0, alignof(char));
    EXPECT_NE(zeroSize, nullptr);

    // Single byte allocation
    void* oneByte = testAllocator.Allocate(1, alignof(char));
    EXPECT_NE(oneByte, nullptr);

    // Allocation size matching alignment
    void* alignSize = testAllocator.Allocate(alignof(max_align_t), alignof(max_align_t));
    EXPECT_NE(alignSize, nullptr);
    EXPECT_EQ(reinterpret_cast<std::uintptr_t>(alignSize) % alignof(max_align_t), 0);
}

/**@brief Tests invalid alignment values */
TEST_F(RingAllocatorTest, Allocate_HandlesInvalidAlignment)
{
    // Non-power-of-two alignment
    EXPECT_ANY_THROW((void)testAllocator.Allocate(10, 3));
    EXPECT_ANY_THROW((void)testAllocator.Allocate(10, 6));

    // Zero alignment
    EXPECT_ANY_THROW((void)testAllocator.Allocate(10, 0));

    // Excessive alignment
    EXPECT_ANY_THROW((void)testAllocator.Allocate(10, DEFAULT_ALLOCATION_SIZE * 2));
}

/**@brief Tests exact buffer end cases and wrapping */
TEST_F(RingAllocatorTest, Allocate_HandlesExactBufferAllocation)
{
    // Fill to exact end
    void* exact = testAllocator.Allocate(DEFAULT_ALLOCATION_SIZE, alignof(char));
    EXPECT_NE(exact, nullptr);
    testAllocator.Reset();

    // Fill to exact end in two pieces
    void* first = testAllocator.Allocate(DEFAULT_ALLOCATION_SIZE - 128, alignof(char));
    void* second = testAllocator.Allocate(128, alignof(char));
    EXPECT_NE(first, nullptr);
    EXPECT_NE(second, nullptr);

    // Should wrap to start
    testAllocator.Reset();
    void* wrapped = testAllocator.Allocate(256, alignof(char));
    EXPECT_NE(wrapped, nullptr);
}

/**@brief Tests mixed alignment sequences */
TEST_F(RingAllocatorTest, Allocate_HandlesMixedAlignments)
{
    struct alignas(2) Align2 { char x; };
    struct alignas(4) Align4 { int x; };
    struct alignas(8) Align8 { double x; };
    struct alignas(16) Align16 { char x[16]; };

    // Sequence of different alignments
    void* a2 = testAllocator.Allocate(sizeof(Align2), alignof(Align2));
    void* a4 = testAllocator.Allocate(sizeof(Align4), alignof(Align4));
    void* a8 = testAllocator.Allocate(sizeof(Align8), alignof(Align8));
    void* a16 = testAllocator.Allocate(sizeof(Align16), alignof(Align16));

    EXPECT_NE(a2, nullptr);
    EXPECT_NE(a4, nullptr);
    EXPECT_NE(a8, nullptr);
    EXPECT_NE(a16, nullptr);

    EXPECT_EQ(reinterpret_cast<std::uintptr_t>(a2) % alignof(Align2), 0);
    EXPECT_EQ(reinterpret_cast<std::uintptr_t>(a4) % alignof(Align4), 0);
    EXPECT_EQ(reinterpret_cast<std::uintptr_t>(a8) % alignof(Align8), 0);
    EXPECT_EQ(reinterpret_cast<std::uintptr_t>(a16) % alignof(Align16), 0);
}

/**@brief Tests fragmentation scenarios */
TEST_F(RingAllocatorTest, Allocate_HandleFragmentation)
{
    // Create fragmentation pattern
    std::vector<void*> small_allocs;
    for(int i = 0; i < 10; i++) {
        void* small = testAllocator.Allocate(64, alignof(char));
        EXPECT_NE(small, nullptr);
        small_allocs.push_back(small);

        // Create gaps by skipping some space
        (void)testAllocator.Allocate(32, alignof(char));
    }

    // Try large allocation that should wrap
    void* large = testAllocator.Allocate(DEFAULT_ALLOCATION_SIZE - 200, alignof(char));
    EXPECT_NE(large, nullptr);
}

/**@brief Tests worst-case alignment padding scenarios */
TEST_F(RingAllocatorTest, Allocate_HandlesWorstCaseAlignmentPadding)
{
    // Create worst-case scenario with alternating small/large alignments
    for(int i = 0; i < 5; i++) {
        void* small = testAllocator.Allocate(3, alignof(char));
        EXPECT_NE(small, nullptr);

        void* large = testAllocator.Allocate(5, alignof(max_align_t));
        EXPECT_NE(large, nullptr);
        EXPECT_EQ(reinterpret_cast<std::uintptr_t>(large) % alignof(max_align_t), 0);
    }
}