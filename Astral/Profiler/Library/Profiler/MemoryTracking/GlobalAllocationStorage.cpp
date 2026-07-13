/**
* @file GlobalAllocationStorage.cpp
* @author Andrew Fagan
* @date 1/7/2025
*/

#include "GlobalAllocationStorage.h"

#include "MemoryTracker.h"

namespace Astral {

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


    size_t GlobalAllocationStorage::GetAllocationEntryCount() const
    {
        return m_NumberOfEntries;
    }

}
