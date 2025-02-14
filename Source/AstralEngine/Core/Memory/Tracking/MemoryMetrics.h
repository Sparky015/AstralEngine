//
// Created by Andrew Fagan on 1/7/25.
//

#pragma once


#include "AllocationData.h"
#include "Core/Events/EventListener.h"
#include "GlobalAllocationStorage.h"
#include "Renderer/RendererEvents.h"
#include <msgpack.hpp>

namespace Core {

    /**@struct FrameAllocationData
     * @brief Stores memory usage data for a frame.
     * Tracks number of allocations and frees as well as total allocated and freed bytes */
    struct FrameAllocationData
    {
        FrameAllocationData() : AllocatedBytes(0), NumberOfAllocations(0) {}
        uint32 AllocatedBytes;
        uint32 NumberOfAllocations;

        MSGPACK_DEFINE(AllocatedBytes, NumberOfAllocations);
    };


    /**@class MemoryMetrics
     * @brief Stores memory usage stats for the engine.
     * @note This class is NOT thread safe. */
    class MemoryMetrics
    {
    public:


        MemoryMetrics();
        ~MemoryMetrics() = default;

        using AllocatorTypeMap = std::unordered_map<AllocatorType, size_t, std::hash<AllocatorType>, std::equal_to<>, NoTrackingAllocator<std::pair<const AllocatorType, size_t>>>;
        using MemoryRegionMap = std::unordered_map<MemoryRegion, size_t, std::hash<MemoryRegion>, std::equal_to<>, NoTrackingAllocator<std::pair<const MemoryRegion, size_t>>>;
        using ThreadMap = std::unordered_map<size_t, size_t, std::hash<size_t>, std::equal_to<>, NoTrackingAllocator<std::pair<const size_t, size_t>>>;

        /**@brief Initializes necessary components for MemoryMetrics. Call before using MemoryMetrics */
        void Init();

        /**@brief Shuts down necessary components for MemoryMetrics. Call when done using MemoryMetrics */
        void Shutdown();

        /**@brief Marks a new allocation and adds the allocation data to the memory metrics
         * @param allocationData The detail information of the allocation being added. */
        void TrackAllocation(const AllocationData& allocationData);

        /**@brief Marks a new deallocation and removes the allocation data from the memory metrics
         * @param allocationData The detailed information of the allocation being removed. */
        void TrackDeallocation(const AllocationData& allocationData);

        /**@brief Retrieves the peak allocated bytes over the course of the program
         * @return The peak memory usage of the program */
        [[nodiscard]] uint64 GetPeakMemoryUsage() const { return m_PeakMemoryUsage; }

        /**@brief Retrieves the total allocated bytes over the course of the program
         * @return The total memory usage of the program */
        [[nodiscard]] uint64 GetTotalMemoryUsage() const { return m_TotalMemoryUsage; }

        /**@brief Retrieves the current active allocations (Number of allocations that were allocated but not freed yet)
         * @return The current active allocation count */
        [[nodiscard]] uint64 GetTotalActiveAllocations() const { return m_TotalActiveAllocations; }

        /**@brief Retrieves the current total allocations made
            * @return The current total allocation count */
        [[nodiscard]] uint64 GetTotalAllocations() const { return m_TotalAllocations; }

        /**@brief Retrieves memory usage metrics for the current frame
         * @return The memory usage metrics of the current frame */
        [[nodiscard]] const FrameAllocationData& GetFrameAllocationData() const { return m_FrameAllocationData; }

        /**@brief Retrieves memory usage metrics for the allocator type.
         * @return The memory usage metrics of the allocator type */
        [[nodiscard]] size_t GetAllocatorTypeUsage(AllocatorType allocatorType) const;

        /**@brief Retrieves memory usage metrics for the allocator type.
         * @return The memory usage metrics of the allocator type */
        [[nodiscard]] size_t GetAllocatorTypePeakUsage(AllocatorType allocatorType) const;


        /**@brief Retrieves memory usage metrics for the allocator type.
         * @return The memory usage metrics of the allocator type */
        [[nodiscard]] size_t GetMemoryRegionUsage(MemoryRegion memoryRegion) const;


        /**@brief Retrieves memory usage metrics for the allocator type.
         * @return The memory usage metrics of the allocator type */
        [[nodiscard]] size_t GetMemoryRegionPeakUsage(MemoryRegion memoryRegion) const;



        /**@brief Retrieves memory usage metrics for the allocator type.
         * @return The memory usage metrics of the allocator type */
        [[nodiscard]] size_t GetThreadUsage(std::thread::id threadID) const;

        /**@brief Retrieves memory usage metrics for the allocator type.
         * @return The memory usage metrics of the allocator type */
        [[nodiscard]] size_t GetThreadPeakUsage(std::thread::id threadID) const;

        /**@brief Retrieves memory usage metrics for the allocator type.
         * @return The memory usage metrics of the allocator type */
        [[nodiscard]] size_t GetThreadActiveAllocations(const std::thread::id threadID) const;

        /**@brief Retrieves memory usage metrics for the allocator type.
         * @return The memory usage metrics of the allocator type */
        [[nodiscard]] size_t GetThreadTotalAllocations(const std::thread::id threadID) const;


        [[nodiscard]] const AllocatorTypeMap& GetMemoryUsageByAllocatorIterable() const { return m_MemoryUsageByAllocator; }

        [[nodiscard]] const AllocatorTypeMap& GetPeakMemoryUsageByAllocatorIterable() const { return m_PeakMemoryUsageByAllocator; }

        [[nodiscard]] const AllocatorTypeMap& GetActiveAllocationsByAllocatorIterable() const { return m_ActiveAllocationsByAllocator; }

        [[nodiscard]] const AllocatorTypeMap& GetTotalAllocationsByAllocatorIterable() const { return m_TotalAllocationsByAllocator; }

        [[nodiscard]] const MemoryRegionMap& GetMemoryUsageByRegionIterable() const { return m_MemoryUsageByRegion; }

        [[nodiscard]] const MemoryRegionMap& GetPeakMemoryUsageByRegionIterable() const { return m_PeakMemoryUsageByRegion; }

        [[nodiscard]] const MemoryRegionMap& GetActiveAllocationsByRegionIterable() const { return m_ActiveAllocationsByRegion; }

        [[nodiscard]] const MemoryRegionMap& GetTotalAllocationsByRegionIterable() const { return m_TotalAllocationsByRegion; }

        [[nodiscard]] const ThreadMap& GetMemoryUsageByThreadIterable() const { return m_MemoryUsageByThread; }

        [[nodiscard]] const ThreadMap& GetPeakMemoryUsageByThreadIterable() const { return m_PeakMemoryUsageByThread; }

        [[nodiscard]] const ThreadMap& GetActiveAllocationsByThreadIterable() const { return m_ActiveAllocationsByThread; }

        [[nodiscard]] const ThreadMap& GetTotalAllocationsByThreadIterable() const { return m_TotalAllocationsByThread; }


        // There is no need for moving or copying this class.
        MemoryMetrics(const MemoryMetrics&) = delete;
        MemoryMetrics& operator=(const MemoryMetrics&) = delete;
        MemoryMetrics(MemoryMetrics&&) noexcept = delete;
        MemoryMetrics& operator=(MemoryMetrics&&) noexcept = delete;

    private:
        // TODO: Switch from a hashmap to an array for memory regions and allocator types because the size is known at compile time
        // and the enum values translate to indices easily

        // Underlying type of std::thread::id is size_t. Cast std::thread::id to size_t for serialization purposes

        [[nodiscard]] size_t GetThreadIDHash(const std::thread::id& id) const { return std::hash<std::thread::id>{}(id); }

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

        FrameAllocationData m_FrameAllocationData;
        Core::EventListener<NewFrameEvent> m_NewFrameEventListener;

    public:
        MSGPACK_DEFINE(m_PeakMemoryUsage, m_TotalMemoryUsage, m_TotalActiveAllocations, m_TotalAllocations,
                       m_MemoryUsageByAllocator, m_MemoryUsageByRegion, m_MemoryUsageByThread,
                       m_ActiveAllocationsByAllocator, m_ActiveAllocationsByRegion, m_ActiveAllocationsByThread,
                       m_TotalAllocationsByAllocator, m_TotalAllocationsByRegion, m_TotalAllocationsByThread,
                       m_FrameAllocationData);
    };

}
