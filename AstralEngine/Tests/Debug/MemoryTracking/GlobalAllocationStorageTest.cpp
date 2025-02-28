/**
* @file GlobalAllocationStorageTest.cpp
* @author Andrew Fagan
* @date 2/10/25
*/

#include <gtest/gtest.h>

#include "Debug/MemoryTracking/GlobalAllocationStorage.h"


class GlobalAllocationStorageTest : public ::testing::Test
{
public:
    Core::GlobalAllocationStorage m_Storage;
};


/**@brief  */
TEST_F(GlobalAllocationStorageTest, AddPointer_DoesNothingWhenPointerIsNullptr)
{
    Core::AllocationData allocationData1 = {nullptr, 4};
    EXPECT_NO_THROW(m_Storage.AddPointer(allocationData1));
    EXPECT_FALSE(m_Storage.IsPointerStored(nullptr));
}

/**@brief  */
TEST_F(GlobalAllocationStorageTest, AddPointer_DoesNothingOnDoubleAdds)
{
    int a = 1;
    Core::AllocationData allocationData1 = {&a, 4};
    Core::AllocationData allocationData2 = {&a, 9};

    m_Storage.AddPointer(allocationData1);
    m_Storage.AddPointer(allocationData2);
    EXPECT_EQ(m_Storage.GetPointerData(&a).size, 4);

    m_Storage.FreePointer(allocationData1.pointer);
}


/**@brief  */
TEST_F(GlobalAllocationStorageTest, FreePointer_DoesNothingWhenGivenNullptr)
{
    int a = 1;
    int b = 2;
    Core::AllocationData allocationData1 = {&a, 4};
    Core::AllocationData allocationData2 = {&b, 4};

    m_Storage.AddPointer(allocationData1);
    m_Storage.AddPointer(allocationData2);

    size_t initialEntryCount = m_Storage.GetAllocationEntryCount();

    EXPECT_NO_THROW(m_Storage.FreePointer(nullptr));
    EXPECT_FALSE(m_Storage.IsPointerStored(nullptr));

    EXPECT_EQ(initialEntryCount, m_Storage.GetAllocationEntryCount());

    m_Storage.FreePointer(allocationData1.pointer);
    m_Storage.FreePointer(allocationData2.pointer);
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


/**@brief Tests if the  */
TEST_F(GlobalAllocationStorageTest, GetPointerData_ThrowsIfPointerHasNoEntry)
{
    int a = 5;
    int b = 10;
    Core::AllocationData allocationData1 = {&a, 4};
    Core::AllocationData allocationData2 = {&b, 4};

    EXPECT_ANY_THROW(m_Storage.GetPointerData(allocationData1.pointer));
    EXPECT_ANY_THROW(m_Storage.GetPointerData(allocationData2.pointer));

    m_Storage.AddPointer(allocationData1);
    m_Storage.AddPointer(allocationData2);

    EXPECT_NO_THROW(m_Storage.GetPointerData(allocationData1.pointer));
    EXPECT_NO_THROW(m_Storage.GetPointerData(allocationData2.pointer));

    m_Storage.FreePointer(allocationData1.pointer);
    m_Storage.FreePointer(allocationData2.pointer);
}