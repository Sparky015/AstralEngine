/**
* @file GlobalAllocationStorage.h
* @author Andrew Fagan
* @date 1/7/2025
*/
#pragma once

#include "AllocationData.h"
#include "NoTrackingSTLAllocator.h"
#include <unordered_map>

namespace Astral {

    /**
     * @class GlobalAllocationStorage
     * @brief Stores the allocation data of allocated pointers
     */
    class GlobalAllocationStorage
    {
    public:
        GlobalAllocationStorage() = default;
        ~GlobalAllocationStorage() = default;

        /**
         * @brief Stores a mapping of the pointer to the allocation data of the pointer
         * @param allocationData The allocation data for a pointer
         * @warning This method will do nothing if the pointer is already being stored.
         */
        void AddPointer(const AllocationData& allocationData);

        /**
         * @brief Removes the pointer's entry in the allocation storage
         * @param pointer The pointer that is being freed.
         * @note Does nothing when given nullptr. Does nothing if the pointer is not stored.
         */
        void FreePointer(void* pointer);

        /**
         * @brief Checks if a pointer has an allocation data entry in storage currently
         * @param pointer The pointer being checked if it is in storage
         * @return True if the pointer is in storage and false if the pointer is not in storage.
         */
        bool IsPointerStored(void* pointer) const;

        /**
         * @brief Gets the allocated memory block size given to a pointer
         * @param pointer The pointer to the allocated memory block.
         * @return The size of the allocated memory block given to a pointer.
         * @warning Aborts when the pointer is not in the storage
         */
        const AllocationData& GetPointerData(const void* pointer) const;

        /**
         * @brief Gets the number of allocation entries currently in storage
         * @return The number of allocation entries currently in storage
         */
        [[nodiscard]] size_t GetAllocationEntryCount() const;

    private:

        using AllocationStorage = std::unordered_map<const void*, AllocationData, std::hash<const void*>, std::equal_to<const void*>, NoTrackingSTLAllocator<std::pair<const void* const, AllocationData>>>;

        AllocationStorage m_Storage;
        size_t m_NumberOfEntries{};
    };

}
