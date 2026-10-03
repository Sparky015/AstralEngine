//
// Created by Andrew Fagan on 1/7/25.
//

#pragma once


#include "AllocationData.h"
#include "Core/Events/EventListener.h"
#include "GlobalAllocationStorage.h"
#include "msgpack.hpp"

namespace Astral {

    /**
     * @struct FrameAllocationData
     * @brief Stores memory usage data for a frame.
     * Tracks number of allocations and frees as well as total allocated and freed bytes
     */
    struct FrameAllocationData
    {
        FrameAllocationData() : AllocatedBytes(0), NumberOfAllocations(0) {}
        uint32 AllocatedBytes;
        uint32 NumberOfAllocations;

        MSGPACK_DEFINE(AllocatedBytes, NumberOfAllocations);
    };


    /**
     * @class MemoryMetrics
     * @brief Stores memory usage stats for the engine.
     * @note This class is NOT thread safe.
     */
    class MemoryMetrics
    {
    public:
        MemoryMetrics();
        ~MemoryMetrics() = default;

        using AllocatorTypeMap = std::unordered_map<MemoryTrackerAllocatorType, size_t, std::hash<MemoryTrackerAllocatorType>, std::equal_to<>, NoTrackingSTLAllocator<std::pair<const MemoryTrackerAllocatorType, size_t>>>;
        using MemoryRegionMap = std::unordered_map<MemoryRegion, size_t, std::hash<MemoryRegion>, std::equal_to<>, NoTrackingSTLAllocator<std::pair<const MemoryRegion, size_t>>>;
        using ThreadMap = std::unordered_map<size_t, size_t, std::hash<size_t>, std::equal_to<>, NoTrackingSTLAllocator<std::pair<const size_t, size_t>>>;

        /**
         * @brief Initializes necessary components for MemoryMetrics. Call before using MemoryMetrics
         */
        void Init();

        /**
         * @brief Shuts down necessary components for MemoryMetrics. Call when done using MemoryMetrics
         */
        void Shutdown();

        /**
         * @brief Marks a new allocation and adds the allocation data to the memory metrics
         * @param allocationData The detail information of the allocation being added.
         */
        void TrackAllocation(const AllocationData& allocationData);

        /**
         * @brief Marks a new deallocation and removes the allocation data from the memory metrics
         * @param allocationData The detailed information of the allocation being removed.
         */
        void TrackDeallocation(const AllocationData& allocationData);

        /**
         * @brief Retrieves the peak allocated bytes over the course of the program
         * @return The peak memory usage of the program
         */
        [[nodiscard]] size_t GetPeakMemoryUsage() const;

        /**
         * @brief Retrieves the total allocated bytes over the course of the program
         * @return The total memory usage of the program
         */
        [[nodiscard]] size_t GetTotalMemoryUsage() const;

        /**
         * @brief Retrieves the current active allocations (Number of allocations that were allocated but not freed yet)
         * @return The current active allocation count
         */
        [[nodiscard]] size_t GetTotalActiveAllocations() const;

        /**
         * @brief Retrieves the current total allocations made
         * @return The current total allocation count
         */
        [[nodiscard]] size_t GetTotalAllocations() const;

        /**
         * @brief Retrieves memory usage metrics for the current frame
         * @return The memory usage metrics of the current frame
         */
        [[nodiscard]] const FrameAllocationData& GetFrameAllocationData() const;

        /**
         * @brief Gets the memory usage of the given allocator type
         * @param allocatorType The allocator type to get the memory usage of
         * @return The memory usage of the given allocator type
         */
        [[nodiscard]] size_t GetAllocatorTypeUsage(MemoryTrackerAllocatorType allocatorType) const;

        /**
         * @brief Gets the peak memory usage of the given allocator type
         * @param allocatorType The allocator type to get the peak memory usage of
         * @return The peak memory usage of the given allocator type
         */
        [[nodiscard]] size_t GetAllocatorTypePeakUsage(MemoryTrackerAllocatorType allocatorType) const;

        /**
         * @brief Gets the memory usage of the given memory region
         * @param memoryRegion The memory region to get the memory usage of
         * @return The memory usage of the given memory region
         */
        [[nodiscard]] size_t GetMemoryRegionUsage(MemoryRegion memoryRegion) const;

        /**
         * @brief Gets the peak memory usage of the given memory region
         * @param memoryRegion The memory region to get the peak memory usage of
         * @return The peak memory usage of the given memory region
         */
        [[nodiscard]] size_t GetMemoryRegionPeakUsage(MemoryRegion memoryRegion) const;

        /**
         * @brief Gets the memory usage of the given thread
         * @param threadID The thread ID of the thread to get the memory usage of
         * @return The memory usage of the given thread
         */
        [[nodiscard]] size_t GetThreadUsage(std::thread::id threadID) const;

        /**
         * @brief Gets the peak memory usage of the given thread
         * @param threadID The thread ID of the thread to get the peak memory usage of
         * @return The peak memory usage of the given thread
         */
        [[nodiscard]] size_t GetThreadPeakUsage(std::thread::id threadID) const;

        /**
         * @brief Gets the active allocation count of the given thread
         * @param threadID The thread ID of the thread to get the active allocation count of
         * @return The active allocation count of the given thread
         */
        [[nodiscard]] size_t GetThreadActiveAllocations(std::thread::id threadID) const;

        /**
         * @brief Gets the total allocation count of the given thread
         * @param threadID The thread ID of the thread to get the total allocation count of
         * @return The total allocation count of the given thread
         */
        [[nodiscard]] size_t GetThreadTotalAllocations(std::thread::id threadID) const;

        /**
         * @brief Gets an iterable data structure of the memory usages by allocator type
         * @return An iterable data structure of the memory usages by allocator type
         */
        [[nodiscard]] const AllocatorTypeMap& GetMemoryUsageByAllocatorIterable() const;

        /**
        * @brief Gets an iterable data structure of the peak memory usages by allocator type
        * @return An iterable data structure of the peak memory usages by allocator type
        */
        [[nodiscard]] const AllocatorTypeMap& GetPeakMemoryUsageByAllocatorIterable() const;

        /**
        * @brief Gets an iterable data structure of the active allocations by allocator type
        * @return An iterable data structure of the active allocations by allocator type
        */
        [[nodiscard]] const AllocatorTypeMap& GetActiveAllocationsByAllocatorIterable() const;

        /**
        * @brief Gets an iterable data structure of the total allocations by allocator type
        * @return An iterable data structure of the total allocations by allocator type
        */
        [[nodiscard]] const AllocatorTypeMap& GetTotalAllocationsByAllocatorIterable() const;

        /**
        * @brief Gets an iterable data structure of the memory usage by memory region
        * @return An iterable data structure of the memory usage by memory region
        */
        [[nodiscard]] const MemoryRegionMap& GetMemoryUsageByRegionIterable() const;

        /**
        * @brief Gets an iterable data structure of the peak memory usage by memory region
        * @return An iterable data structure of the peak memory usage by memory region
        */
        [[nodiscard]] const MemoryRegionMap& GetPeakMemoryUsageByRegionIterable() const;

        /**
        * @brief Gets an iterable data structure of the active allocations by memory region
        * @return An iterable data structure of the active allocations by memory region
        */
        [[nodiscard]] const MemoryRegionMap& GetActiveAllocationsByRegionIterable() const;

        /**
        * @brief Gets an iterable data structure of the total allocations by memory region
        * @return An iterable data structure of the total allocations by memory region
        */
        [[nodiscard]] const MemoryRegionMap& GetTotalAllocationsByRegionIterable() const;

        /**
        * @brief Gets an iterable data structure of the memory usage by thread
        * @return An iterable data structure of the memory usage by thread
        */
        [[nodiscard]] const ThreadMap& GetMemoryUsageByThreadIterable() const;

        /**
        * @brief Gets an iterable data structure of the peak memory usage by thread
        * @return An iterable data structure of the peak memory usage by thread
        */
        [[nodiscard]] const ThreadMap& GetPeakMemoryUsageByThreadIterable() const;

        /**
        * @brief Gets an iterable data structure of the active allocations by thread
        * @return An iterable data structure of the active allocations by thread
        */
        [[nodiscard]] const ThreadMap& GetActiveAllocationsByThreadIterable() const;

        /**
        * @brief Gets an iterable data structure of the total allocations by thread
        * @return An iterable data structure of the total allocations by thread
        */
        [[nodiscard]] const ThreadMap& GetTotalAllocationsByThreadIterable() const;

        /**
         * @brief Calculates the thread ID hash of the given thread ID
         * @param id The thread ID to hash
         * @return The hash of the given thread ID
         */
        [[nodiscard]] size_t GetThreadIDHash(const std::thread::id& id) const;


        MemoryMetrics(const MemoryMetrics&) = delete;
        MemoryMetrics& operator=(const MemoryMetrics&) = delete;
        MemoryMetrics(MemoryMetrics&&) noexcept = default;
        MemoryMetrics& operator=(MemoryMetrics&&) noexcept = default;

    private:

        // TODO: Switch from a hashmap to an array for memory regions and allocator types because the size is known at compile time and the enum values translate to indices easily
        // TODO: Maybe use some sort of static atomic + thread_local flag to track thread ID indices in an array to avoid hashing

        uint64 m_PeakMemoryUsage;
        uint64 m_TotalMemoryUsage;
        uint32 m_TotalActiveAllocations;
        size_t m_TotalAllocations;

        AllocatorTypeMap m_MemoryUsageByAllocator;
        AllocatorTypeMap m_PeakMemoryUsageByAllocator;
        AllocatorTypeMap m_ActiveAllocationsByAllocator;
        AllocatorTypeMap m_TotalAllocationsByAllocator;

        MemoryRegionMap m_MemoryUsageByRegion;
        MemoryRegionMap m_PeakMemoryUsageByRegion;
        MemoryRegionMap m_ActiveAllocationsByRegion;
        MemoryRegionMap m_TotalAllocationsByRegion;

        ThreadMap m_MemoryUsageByThread;
        ThreadMap m_PeakMemoryUsageByThread;
        ThreadMap m_ActiveAllocationsByThread;
        ThreadMap m_TotalAllocationsByThread;

        FrameAllocationData m_OldFrameAllocationData;
        FrameAllocationData m_FrameAllocationData;
        // Astral::EventListener<NewFrameEvent> m_NewFrameEventListener;

    public:
        MSGPACK_DEFINE(m_PeakMemoryUsage, m_TotalMemoryUsage, m_TotalActiveAllocations, m_TotalAllocations,
                       m_MemoryUsageByAllocator, m_MemoryUsageByRegion, m_MemoryUsageByThread,
                       m_PeakMemoryUsageByAllocator, m_PeakMemoryUsageByRegion, m_PeakMemoryUsageByThread,
                       m_ActiveAllocationsByAllocator, m_ActiveAllocationsByRegion, m_ActiveAllocationsByThread,
                       m_TotalAllocationsByAllocator, m_TotalAllocationsByRegion, m_TotalAllocationsByThread,
                       m_FrameAllocationData);
    };

}
