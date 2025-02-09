//
// Created by Andrew Fagan on 1/7/25.
//

#pragma once


#include "Core/Events/EventListener.h"
#include "Renderer/RendererEvents.h"

#include "GlobalAllocationStorage.h"
#include "MemoryTracker.h"


namespace Core {

    /**@struct FrameAllocationData
     * @brief Stores memory usage data for a frame.
     * Tracks number of allocations and frees as well as total allocated and freed bytes */
    struct FrameAllocationData
    {
        uint32 AllocatedBytes{};
        uint32 NumberOfAllocations{};
    };


    /**@class MemoryMetrics
     * @brief Stores memory usage stats for the engine.
     * @note This class is NOT thread safe. */
    class MemoryMetrics
    {
    public:
        MemoryMetrics() = default;
        ~MemoryMetrics() = default;

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

        /**@brief Retrieves memory usage metrics for the current frame
         * @return The memory usage metrics of the current frame */
        [[nodiscard]] const FrameAllocationData& GetFrameAllocationData() const { return m_FrameAllocationData; }

        /**@brief Retrieves memory usage metrics for the allocator type.
         * @return The memory usage metrics of the allocator type */
        [[nodiscard]] size_t GetAllocatorTypeUsage(AllocatorType allocatorType) const;

        /**@brief Retrieves memory usage metrics for the allocator type.
         * @return The memory usage metrics of the allocator type */
        [[nodiscard]] size_t GetMemoryRegionUsage(MemoryRegion memoryRegion) const;

        /**@brief Retrieves memory usage metrics for the allocator type.
         * @return The memory usage metrics of the allocator type */
        [[nodiscard]] size_t GetThreadUsage(std::thread::id threadID) const;

        /**@brief Retrieves memory usage metrics for the allocator type.
         * @return The memory usage metrics of the allocator type */
        [[nodiscard]] size_t GetAllocatorTypePeakUsage(AllocatorType allocatorType) const;

        /**@brief Retrieves memory usage metrics for the allocator type.
         * @return The memory usage metrics of the allocator type */
        [[nodiscard]] size_t GetMemoryRegionPeakUsage(MemoryRegion memoryRegion) const;

        /**@brief Retrieves memory usage metrics for the allocator type.
         * @return The memory usage metrics of the allocator type */
        [[nodiscard]] size_t GetThreadPeakUsage(std::thread::id threadID) const;

        // There is no need for moving or copying this class.
        MemoryMetrics(const MemoryMetrics&) = delete;
        MemoryMetrics& operator=(const MemoryMetrics&) = delete;
        MemoryMetrics(MemoryMetrics&&) noexcept = delete;
        MemoryMetrics& operator=(MemoryMetrics&&) noexcept = delete;

    private:

        uint64 m_PeakMemoryUsage;
        uint64 m_TotalMemoryUsage;
        uint32 m_TotalActiveAllocations;

        std::unordered_map<const AllocatorType, size_t, NoTrackingAllocator<std::pair<const AllocatorType, size_t>>> m_MemoryUsageByAllocator;
        std::unordered_map<MemoryRegion, size_t, NoTrackingAllocator<std::pair<MemoryRegion, size_t>>> m_MemoryUsageByRegion;
        std::unordered_map<std::thread::id, size_t, NoTrackingAllocator<std::pair<std::thread::id, size_t>>> m_MemoryUsageByThread;

        std::unordered_map<const AllocatorType, size_t, NoTrackingAllocator<std::pair<const AllocatorType, size_t>>> m_PeakMemoryUsageByAllocator;
        std::unordered_map<MemoryRegion, size_t, NoTrackingAllocator<std::pair<MemoryRegion, size_t>>> m_PeakMemoryUsageByRegion;
        std::unordered_map<std::thread::id, size_t, NoTrackingAllocator<std::pair<std::thread::id, size_t>>> m_PeakMemoryUsageByThread;

        FrameAllocationData m_FrameAllocationData;
        Core::EventListener<NewFrameEvent> m_NewFrameEventListener{[this](NewFrameEvent)
        {
            m_FrameAllocationData = FrameAllocationData();
        }};
    };

}
