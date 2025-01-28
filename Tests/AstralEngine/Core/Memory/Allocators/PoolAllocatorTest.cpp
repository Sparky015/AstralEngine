//
// Created by Andrew Fagan on 1/26/25.
//

#include <gtest/gtest.h>

#include "Core/Memory/Allocators/PoolAllocator.h"
#include <algorithm>
#include <unordered_set>


class PoolAllocatorTest : public ::testing::Test
{
public:
    static constexpr int NUMBER_OF_BLOCKS = 10;
    Core::PoolAllocator<1024, NUMBER_OF_BLOCKS> testAllocator;
};


/**@brief Tests that a single block allocation is successful */
TEST_F(PoolAllocatorTest, Allocate_SingleBlock)
{
    void* block = testAllocator.Allocate();
    ASSERT_NE(block, nullptr);
    void* nextBlock = testAllocator.Allocate();
    ASSERT_NE(nextBlock, nullptr);
    ASSERT_NE(block, nextBlock);
}

/**@brief Tests allocating all blocks succeeds and subsequent allocations fail */
TEST_F(PoolAllocatorTest, Allocate_AllBlocks)
{
    void* blocks[NUMBER_OF_BLOCKS];
    for (int i = 0; i < NUMBER_OF_BLOCKS; i++)
    {
        blocks[i] = testAllocator.Allocate();
        ASSERT_NE(blocks[i], nullptr) << "Allocation " << i << " failed";
        for (int j = 0; j < i; ++j)
        {
            ASSERT_NE(blocks[i], blocks[j]) << "Duplicate block at " << i << " and " << j;
        }
    }
    void* nullBlock = testAllocator.Allocate();
    ASSERT_EQ(nullBlock, nullptr);
}

/**@brief Tests freeing a single block allows reallocation */
TEST_F(PoolAllocatorTest, Free_SingleBlock)
{
    void* block = testAllocator.Allocate();
    testAllocator.Free(block);
    void* newBlock = testAllocator.Allocate();
    ASSERT_EQ(newBlock, block);
}

/**@brief Tests freeing all blocks allows reallocating all */
TEST_F(PoolAllocatorTest, Free_AllBlocks)
{
    void* firstAllocationBlocks[NUMBER_OF_BLOCKS];
    std::unordered_set<void*> firstSet;
    for (int i = 0; i < NUMBER_OF_BLOCKS; ++i)
    {
        firstAllocationBlocks[i] = testAllocator.Allocate();
        ASSERT_NE(firstAllocationBlocks[i], nullptr);
        firstSet.insert(firstAllocationBlocks[i]);
    }
    for (int i = 0; i < NUMBER_OF_BLOCKS; ++i)
    {
        testAllocator.Free(firstAllocationBlocks[i]);
    }
    void* secondAllocationBlocks[NUMBER_OF_BLOCKS];
    for (int i = 0; i < NUMBER_OF_BLOCKS; i++)
    {
        secondAllocationBlocks[i] = testAllocator.Allocate();
        ASSERT_NE(secondAllocationBlocks[i], nullptr);
        ASSERT_TRUE(firstSet.count(secondAllocationBlocks[i])) << "Pointer not from original allocation";
        for (int j = 0; j < i; j++)
        {
            ASSERT_NE(secondAllocationBlocks[i], secondAllocationBlocks[j]) << "Duplicate at " << i << " and " << j;
        }
    }
}

/**@brief Tests freeing an invalid pointer triggers an assertion */
TEST_F(PoolAllocatorTest, Free_InvalidPointer)
{
    int dummy;
    void* invalidPtr = &dummy;
    EXPECT_THROW(testAllocator.Free(invalidPtr), std::runtime_error) << "ASSERT failed. Pointer does not fall within this pool's memory block.";
}

/**@brief Tests that freeing a block and reallocating returns it in LIFO order */
TEST_F(PoolAllocatorTest, Allocate_AfterFree_Order)
{
    [[maybe_unused]] void* ptr0 = testAllocator.Allocate();
    void* ptr1 = testAllocator.Allocate();
    [[maybe_unused]] void* ptr2 = testAllocator.Allocate();

    testAllocator.Free(ptr1);

    void* afterFree = testAllocator.Allocate();
    ASSERT_EQ(afterFree, ptr1);
}

/**@brief Tests double-free triggers assertion */
TEST_F(PoolAllocatorTest, Free_DoubleFreeAssertion)
{
    void* ptr = testAllocator.Allocate();
    testAllocator.Free(ptr);

    EXPECT_THROW(testAllocator.Free(ptr),  std::runtime_error);

    [[maybe_unused]] void* ptr1 = testAllocator.Allocate();
    [[maybe_unused]] void* ptr2 = testAllocator.Allocate();
    void* ptr3 = testAllocator.Allocate();
    testAllocator.Free(ptr3);

    EXPECT_THROW(testAllocator.Free(ptr3),  std::runtime_error);
}