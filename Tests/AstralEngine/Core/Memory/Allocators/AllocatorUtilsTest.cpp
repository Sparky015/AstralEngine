/**
* @file AllocatorUtilsTest.cpp
* @author Andrew Fagan
* @date 2/1/25
*/

#include <gtest/gtest.h>

#include "Core/Memory/Allocators/AllocatorUtils.h"

class AllocatorUtilsTest : public ::testing::Test
{
public:

};

/** @brief Tests if the IsAlignmentPowerOfTwo method correctly returns true only when the alignment is a power of two */
TEST_F(AllocatorUtilsTest, IsAlignmentPowerOfTwo_ReturnsTrueOnlyWhenPowerOfTwo)
{
    EXPECT_EQ(Core::AllocatorUtils::IsAlignmentPowerOfTwo(4), true);
    EXPECT_EQ(Core::AllocatorUtils::IsAlignmentPowerOfTwo(8), true);
    EXPECT_EQ(Core::AllocatorUtils::IsAlignmentPowerOfTwo(16), true);
    EXPECT_EQ(Core::AllocatorUtils::IsAlignmentPowerOfTwo(32), true);
    EXPECT_EQ(Core::AllocatorUtils::IsAlignmentPowerOfTwo(64), true);
    EXPECT_EQ(Core::AllocatorUtils::IsAlignmentPowerOfTwo(128), true);
    EXPECT_EQ(Core::AllocatorUtils::IsAlignmentPowerOfTwo(256), true);
    EXPECT_EQ(Core::AllocatorUtils::IsAlignmentPowerOfTwo(1), true);

    EXPECT_EQ(Core::AllocatorUtils::IsAlignmentPowerOfTwo(3), false);
    EXPECT_EQ(Core::AllocatorUtils::IsAlignmentPowerOfTwo(63), false);
    EXPECT_EQ(Core::AllocatorUtils::IsAlignmentPowerOfTwo(73), false);
    EXPECT_EQ(Core::AllocatorUtils::IsAlignmentPowerOfTwo(7), false);

    EXPECT_EQ(Core::AllocatorUtils::IsAlignmentPowerOfTwo(0), false);
}

/** @brief Tests if the DoesOverFlow method correctly returns true only
 *         the allocated number of bytes will cause an overflow past the given end address */
TEST_F(AllocatorUtilsTest, DoesCauseOverflow_ReturnsTrueWhenAllocationCausesOverflow)
{
    void* startAddress = nullptr;
    void* endAddress = (void*)0x00000001;
    EXPECT_EQ(Core::AllocatorUtils::DoesCauseOverflow(startAddress, 0, endAddress), false);
    EXPECT_EQ(Core::AllocatorUtils::DoesCauseOverflow(startAddress, 1, endAddress), false);
    EXPECT_EQ(Core::AllocatorUtils::DoesCauseOverflow(startAddress, 2, endAddress), true);

    startAddress = (void*)0x000001A2;
    endAddress = (void*)0x000001A8;
    EXPECT_EQ(Core::AllocatorUtils::DoesCauseOverflow(startAddress, 4, endAddress), false);
    EXPECT_EQ(Core::AllocatorUtils::DoesCauseOverflow(startAddress, 6, endAddress), false);
    EXPECT_EQ(Core::AllocatorUtils::DoesCauseOverflow(startAddress, 7, endAddress), true);
    EXPECT_EQ(Core::AllocatorUtils::DoesCauseOverflow(startAddress, 1242, endAddress), true);
}

/** @brief Tests if the RoundToNextAlignmentMultiple method correctly returns a rounded up
 *         number of the original memory size block to the next multiple of the given alignment. */
TEST_F(AllocatorUtilsTest, RoundToNextAlignmentMultiple_RoundsGivenNumberToTheNextMultipleOfGivenAlignment)
{
    EXPECT_EQ(Core::AllocatorUtils::RoundToNextAlignmentMultiple(8, 4), 8);
    EXPECT_EQ(Core::AllocatorUtils::RoundToNextAlignmentMultiple(5, 4), 8);

    EXPECT_EQ(Core::AllocatorUtils::RoundToNextAlignmentMultiple(14, 8), 16);
    EXPECT_EQ(Core::AllocatorUtils::RoundToNextAlignmentMultiple(65, 8), 72);

    EXPECT_EQ(Core::AllocatorUtils::RoundToNextAlignmentMultiple(15, 16), 16);
    EXPECT_EQ(Core::AllocatorUtils::RoundToNextAlignmentMultiple(17, 16), 32);
}

/** @brief Tests if the RoundToNextAlignmentMultiple returns zero when the given alignment is zero. */
TEST_F(AllocatorUtilsTest, RoundToNextAlignmentMultiple_ReturnsZeroWhenAlignmentZero)
{
    EXPECT_EQ(Core::AllocatorUtils::RoundToNextAlignmentMultiple(5, 0), 0);
    EXPECT_EQ(Core::AllocatorUtils::RoundToNextAlignmentMultiple(122, 0), 0);
    EXPECT_EQ(Core::AllocatorUtils::RoundToNextAlignmentMultiple(256, 0), 0);
}

/** @brief Tests if the power returned back from AlignedAlloc is correctly aligned to the given alignment */
TEST_F(AllocatorUtilsTest, AlignedAlloc_ReturnedPointerIsCorrectlyAligned)
{
    void* pointer = Core::AllocatorUtils::AlignedAlloc(64, 4);
    EXPECT_EQ((std::uintptr_t)pointer % 4, 0);
    Core::AllocatorUtils::FreeAlignedAlloc(pointer);

    pointer = Core::AllocatorUtils::AlignedAlloc(63, 8);
    EXPECT_EQ((std::uintptr_t)pointer % 8, 0);
    Core::AllocatorUtils::FreeAlignedAlloc(pointer);

    pointer = Core::AllocatorUtils::AlignedAlloc(8, 16);
    EXPECT_EQ((std::uintptr_t)pointer % 16, 0);
    Core::AllocatorUtils::FreeAlignedAlloc(pointer);

    pointer = Core::AllocatorUtils::AlignedAlloc(1, 64);
    EXPECT_EQ((std::uintptr_t)pointer % 64, 0);
    Core::AllocatorUtils::FreeAlignedAlloc(pointer);

    pointer = Core::AllocatorUtils::AlignedAlloc(16, 16);
    EXPECT_EQ((std::uintptr_t)pointer % 16, 0);
    Core::AllocatorUtils::FreeAlignedAlloc(pointer);
}

/** @brief Tests if the method resizes the memory block and if it resizes it to the right size. */
TEST_F(AllocatorUtilsTest, ResizeMemoryBlock_CorrectlyResizesToCorrectSize)
{
    const size_t originalBlockSize = 1024;
    void* initialBlockPtr = Core::AllocatorUtils::AllocMaxAlignedBlock(originalBlockSize);
    ASSERT_NE(nullptr, initialBlockPtr) << "Failed to allocate initial block of memory.";

    void* newBlockPtr = nullptr;
    size_t newBlockSize = 0;
    EXPECT_TRUE(Core::AllocatorUtils::ResizeMemoryBlock(initialBlockPtr, originalBlockSize, newBlockPtr, newBlockSize));

    EXPECT_NE(nullptr, newBlockPtr) << "Resized block pointer should not be null.";
    EXPECT_EQ(originalBlockSize * 2, newBlockSize) << "Resized block should be twice the original size.";

    Core::AllocatorUtils::FreeMaxAlignedBlock(newBlockPtr);
}