/**
* @file GlobalAllocationStorage.cpp
* @author Andrew Fagan
* @date 1/7/2025
*/

#include "Debug/Utilities/Asserts.h"
#include "GlobalAllocationStorage.h"

#include <iostream>

#include "MemoryTracker.h"

namespace Core {

    GlobalAllocationStorage::GlobalAllocationStorage()
    {
        std::atexit(GlobalAllocationStorage::PrintLeakedPointers);
    }

    void GlobalAllocationStorage::AddPointer(const AllocationData& allocationData)
    {
        [[unlikely]] if (!allocationData.pointer) { return; }
        [[unlikely]] if (IsPointerStored(allocationData.pointer)) { return; }
        GetStorage()[allocationData.pointer] = allocationData;
        m_NumberOfEntries++;
    }


    void GlobalAllocationStorage::FreePointer(void* pointer)
    {
        [[unlikely]] if (!pointer) { return; }
        [[unlikely]] if (!IsPointerStored(pointer)) { return; }

        GetStorage().erase(pointer);
        m_NumberOfEntries--;
    }


    bool GlobalAllocationStorage::IsPointerStored(void* pointer) const
    {
        return GetStorage().contains(pointer);
    }


    const AllocationData& GlobalAllocationStorage::GetPointerData(const void* pointer) const
    {
        return GetStorage().at(pointer);
    }


    void GlobalAllocationStorage::PrintLeakedPointers()
    {
        std::cout << "Number of leaked pointers: " << GlobalAllocationStorage::GetStorage().size() << "\n";
        for (auto [pointer, allocationData] : GlobalAllocationStorage::GetStorage())
        {
            if (allocationData.threadID == std::this_thread::get_id())
            {
                std::cout << "Leaked pointer " << pointer << " of size " << allocationData.size << "\n";
            }
        }

    }

}
