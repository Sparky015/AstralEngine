/**
* @file GlobalAllocationStorage.cpp
* @author Andrew Fagan
* @date 1/7/2025
*/

#include "GlobalAllocationStorage.h"

#include <iostream>

#include "MemoryTracker.h"

namespace Astral {

    GlobalAllocationStorage::~GlobalAllocationStorage()
    {
        // Print out all the remaining pointers in storage. They are considered leaked pointers.
        // std::cout << "Number of leaked pointers: " << m_Storage.size() << "\n";
        // for (auto [pointer, allocationData] : m_Storage)
        // {
        //     if (allocationData.threadID == std::this_thread::get_id())
        //     {
        //         std::cout << "Leaked pointer " << pointer << " of size " << allocationData.size << " on thread " << std::hash<std::thread::id>{}(allocationData.threadID) << "\n";
        //     }
        // }
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
