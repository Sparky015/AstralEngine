/**
* @file ProfilerViewConfig.h
* @author Andrew Fagan 
* @date 10/26/26
*/

#pragma once

#include "Core/Math/Math.h"
#include "Profiler/MemoryTracking/AllocationData.h"

namespace Astral {

    class ProfilerViewConfig
    {
    public:

        /**
         * @brief Sets the plot limits filter to the given plot limits
         * @param plotLimits The plot limits to filter in based on
         */
        void SetPlotLimitsFilterIn(Vec2 plotLimits);

        /**
         * @brief Gets the plot limits filter
         * @return The plot limits filter
         */
        Vec2 GetPlotLimitsFilter();

        /**
         * @brief Sets the thread ID filter to the given thread ID
         * @param hashedThreadID The thread ID to filter in based on
         */
        void SetThreadIDFilterIn(size_t hashedThreadID);

        /**
         * @brief Gets the thread ID filter
         * @return The thread ID filter
         */
        size_t GetThreadIDFilter();

        /**
         * @brief Sets the memory region filter to the given memory region
         * @param memoryRegion The memory region to filter in based on
         */
        void SetRegionFilterIn(MemoryRegion memoryRegion);

        /**
         * @brief Gets the memory region filter
         * @return The memory region filter
         */
        MemoryRegion GetRegionFilter();

        /**
         * @brief Sets the allocator type filter in
         * @param allocatorType The allocator type to filter in based on
         */
        void SetAllocatorFilterIn(MemoryTrackerAllocatorType allocatorType);

        /**
         * @brief Gets the allocator filter
         * @return The allocator filter
         */
        MemoryTrackerAllocatorType GetAllocatorFilter();

    private:

        Vec2 m_FilterInPlotLimits = {};
        size_t m_FilterInThreadID = 0;
        MemoryRegion m_FilterInMemoryRegion = MemoryRegion::MEMORY_REGION_END;
        MemoryTrackerAllocatorType m_FilterInAllocatorType = MemoryTrackerAllocatorType::ALLOCATOR_TYPE_END;
    };

}
