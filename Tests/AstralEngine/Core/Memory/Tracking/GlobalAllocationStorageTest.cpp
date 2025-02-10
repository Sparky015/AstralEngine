/**
* @file GlobalAllocationStorageTest.cpp
* @author Andrew Fagan
* @date 2/10/25
*/

#include <gtest/gtest.h>

#include "Core/Memory/Tracking/GlobalAllocationStorage.h"


class GlobalAllocationStorageTest : public ::testing::Test
{
public:
    Core::GlobalAllocationStorage m_Storage;
};


/**@brief  */
TEST_F(GlobalAllocationStorageTest, AddPointer_)
{

}


/**@brief  */
TEST_F(GlobalAllocationStorageTest, FreePointer_)
{

}


/**@brief Tests if the IsPointerStored method is accurately returning the correct value after AddPointer and FreePointer  */
TEST_F(GlobalAllocationStorageTest, IsPointerStored_ReturnsTrueWhenPointerIsStored)
{
    void* pointer = std::malloc(4);
    Core::AllocationData allocationData = {pointer, 4, Core::MemoryRegion::UNKNOWN, Core::AllocatorType::NEW_OPERATOR};

    EXPECT_FALSE(m_Storage.IsPointerStored(pointer));

    m_Storage.AddPointer(allocationData);
    EXPECT_FALSE(m_Storage.IsPointerStored(nullptr));
    EXPECT_FALSE(m_Storage.IsPointerStored((void*)0x22335567));
    EXPECT_TRUE(m_Storage.IsPointerStored(pointer));
    m_Storage.FreePointer(pointer);
    std::free(pointer);

    EXPECT_FALSE(m_Storage.IsPointerStored(pointer));
}


/**@brief  */
TEST_F(GlobalAllocationStorageTest, GetPointerData_)
{

}