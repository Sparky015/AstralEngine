/**
* @file GlobalAllocationStorage.h
* @author Andrew Fagan
* @date 1/7/2025
*/
#pragma once

#include "AllocationData.h"
#include "Core/Memory/Allocators/NoTrackingAllocator.h"
#include <unordered_map>

namespace Core {

    /**@class GlobalAllocationStorage
     * @brief Stores the size of memory blocks allocated to pointers */
    class GlobalAllocationStorage
    {
    public:
        GlobalAllocationStorage() = default;
        ~GlobalAllocationStorage();

        /**@brief Stores a mapping of the pointer to the allocation size of the pointer */
        void AddPointer(const AllocationData& allocationData);

        /**@brief Removes the pointer's entry in the allocation size mapping
         * @param pointer The pointer that is being freed. Removes entry of pointer only if it was already being tracked.
         * Does nothing when given nullptr. */
        void FreePointer(void* pointer);

        /**@brief Checks if a pointer has a allocation data entry in storage currently
         * @param pointer The pointer being checked if it is in storage
         * @return True if the pointer is in storage and false if the pointer is not in storage. */
        bool IsPointerStored(void* pointer) const;

        /**@brief Gets the allocated memory block size given to a pointer
         * @param pointer The pointer to the allocated memory block.
         * @return The size of the allocated memory block given to pointer.
         * Returns 0 if
         * - pointer is nullptr.
         * - pointer was not previously registered with AddPointer method. */
        const AllocationData& GetPointerData(const void* pointer) const;

        /**@brief Gets the number of allocation entries currently in storage
         * @return The number of allocation entries currently in storage */
        [[nodiscard]] size_t GetAllocationEntryCount() const { return m_NumberOfEntries; }

    private:

        size_t m_NumberOfEntries{};
        std::unordered_map<const void*, AllocationData, std::hash<const void*>, std::equal_to<const void*>, NoTrackingAllocator<std::pair<const void* const, AllocationData>>> m_Storage;
    };

}
