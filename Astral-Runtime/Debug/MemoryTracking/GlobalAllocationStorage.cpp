/**
* @file GlobalAllocationStorage.cpp
* @author Andrew Fagan
* @date 1/7/2025
*/

#include "Debug/Utilities/Asserts.h"
#include "GlobalAllocationStorage.h"

#include <iostream>

namespace Core {

    GlobalAllocationStorage::~GlobalAllocationStorage()
    {
        // Output all the pointers still being stored as leaked.
        std::cout << "Number of leaked pointers: " << m_NumberOfEntries << "\n";
        for (auto [pointer, allocationData] : m_Storage)
        {
            if (allocationData.threadID == std::this_thread::get_id())
            {
                std::cout << "Leaked pointer " << pointer << " of size " << allocationData.size << "\n";
            }
        }
        // std::cout << "Number of leaked pointers: " << m_NumberOfEntries << "\n";

    }


    void GlobalAllocationStorage::AddPointer(const AllocationData& allocationData)
    {
        [[unlikely]] if (!allocationData.pointer) { return; }
        [[unlikely]] if (IsPointerStored(allocationData.pointer)) { return; }
        m_Storage[allocationData.pointer] = allocationData;
        m_NumberOfEntries++;
    }


    void GlobalAllocationStorage::FreePointer(void* pointer)
    {
        [[unlikely]] if (!pointer) { return; }
        [[unlikely]] if (!IsPointerStored(pointer)) { return; }

        m_Storage.erase(pointer);
        m_NumberOfEntries--;
    }


    bool GlobalAllocationStorage::IsPointerStored(void* pointer) const
    {
        return m_Storage.contains(pointer);
    }


    const AllocationData& GlobalAllocationStorage::GetPointerData(const void* pointer) const
    {
        return m_Storage.at(pointer);
    }

}
