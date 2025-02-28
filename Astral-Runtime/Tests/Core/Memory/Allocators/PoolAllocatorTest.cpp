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
    Core::PoolAllocator testAllocator{1024, NUMBER_OF_BLOCKS};
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
    for (auto& firstAllocationBlock : firstAllocationBlocks)
    {
        firstAllocationBlock = testAllocator.Allocate();
        ASSERT_NE(firstAllocationBlock, nullptr);
        firstSet.insert(firstAllocationBlock);
    }
    for (auto& firstAllocationBlock : firstAllocationBlocks)
    {
        testAllocator.Free(firstAllocationBlock);
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
    int num;
    void* invalidPtr = &num;
    EXPECT_THROW(testAllocator.Free(invalidPtr), std::runtime_error) << "ASSERT failed. Pointer does not fall within this pool's memory block.";
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

/**@brief Tests that we can detect availability of blocks correctly */
TEST_F(PoolAllocatorTest, CanAllocateMoreBlocks_ReturnsTrueWhenNotFull)
{
    // Initially, all blocks should be available
    ASSERT_TRUE(testAllocator.CanAllocateMoreBlocks())
        << "PoolAllocator should have blocks available initially.";

    // Allocate all blocks
    for (int i = 0; i < NUMBER_OF_BLOCKS; ++i)
    {
        void* block = testAllocator.Allocate();
        ASSERT_NE(block, nullptr);
    }

    // Now no blocks should be available
    ASSERT_FALSE(testAllocator.CanAllocateMoreBlocks())
        << "PoolAllocator should have no blocks available after allocating all.";


    void* ptr = testAllocator.Allocate();
    ASSERT_EQ(ptr, nullptr);

    // Allocate again to confirm there's nothing left
    ptr = testAllocator.Allocate();
    ASSERT_EQ(ptr, nullptr);
}

/**@brief Tests that partially freeing a few blocks allows for reallocation */
TEST_F(PoolAllocatorTest, Free_OrderDoesNotMatter)
{
    // Allocate all blocks
    void* blocks[NUMBER_OF_BLOCKS];
    for (int i = 0; i < NUMBER_OF_BLOCKS; ++i)
    {
        blocks[i] = testAllocator.Allocate();
        ASSERT_NE(blocks[i], nullptr) << "Allocation " << i << " failed";
    }

    // Free a few blocks in the middle
    testAllocator.Free(blocks[3]);
    testAllocator.Free(blocks[5]);
    testAllocator.Free(blocks[7]);

    // Re-allocate those freed positions
    void* blockA = testAllocator.Allocate();
    ASSERT_NE(blockA, nullptr) << "Re-allocation after partial free should succeed for index 3.";
    void* blockB = testAllocator.Allocate();
    ASSERT_NE(blockB, nullptr) << "Re-allocation after partial free should succeed for index 5.";
    void* blockC = testAllocator.Allocate();
    ASSERT_NE(blockC, nullptr) << "Re-allocation after partial free should succeed for index 7.";

    // Attempting another allocation should fail, as all slots are in use again.
    void* nullBlock = testAllocator.Allocate();
    ASSERT_EQ(nullBlock, nullptr) << "No more blocks should be available after re-allocating the freed blocks.";
}

TEST_F(PoolAllocatorTest, MoveConstructorTest)
{
    Core::PoolAllocator original(64, 4);
    [[maybe_unused]] void* ptr = original.Allocate();

    Core::PoolAllocator moved(std::move(original));

    EXPECT_EQ(original.GetNumberOfBlocks(), moved.GetNumberOfBlocks());
    EXPECT_EQ(original.GetIndividualBlockSize(), moved.GetIndividualBlockSize());
    EXPECT_TRUE(moved.CanAllocateMoreBlocks() != original.CanAllocateMoreBlocks());
}


TEST_F(PoolAllocatorTest, MoveAssignmentTest)
{
    Core::PoolAllocator allocator1(64, 4);
    Core::PoolAllocator allocator2(128, 2);

    [[maybe_unused]] void* lhsPtr = allocator1.Allocate();

    allocator2 = std::move(allocator1);

    EXPECT_EQ(allocator2.GetNumberOfBlocks(), 4);
    EXPECT_EQ(allocator2.GetIndividualBlockSize(), 64);
    EXPECT_FALSE(allocator2 == allocator1);
}