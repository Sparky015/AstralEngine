//
// Created by Andrew Fagan on 1/7/25.
//

#pragma once


#include "Core/Events/EventListener.h"
#include "Renderer/RendererEvents.h"

#include "GlobalAllocationStorage.h"


namespace Core {

    /**@struct FrameAllocationData
     * @brief Stores memory usage data for a frame.
     * Tracks number of allocations and frees as well as total allocated and freed bytes */
    struct FrameAllocationData
    {
        uint32 AllocatedBytes{};
        uint32 FreedBytes{};
        uint32 NumberOfAllocations{};
        uint32 NumberOfFrees{};
    };


    /**@class MemoryMetrics
     * @brief Stores memory usage stats for the engine.
     * @note This class is NOT thread safe. */
    class MemoryMetrics
    {
    public:
        MemoryMetrics() = default;
        ~MemoryMetrics() = default;

        /**@brief Initializes the MemoryMetrics. Call before using MemoryMetrics */
        void Init();

        /**@brief Shuts down the MemoryMetrics. Call when done using MemoryMetrics */
        void Shutdown();

        /**@brief Updates memory metrics relating to allocations with the allocationSize
         * @param allocationSize The size of the memory block being allocated */
        void TrackAllocation(size_t allocationSize);

        /**@brief Updates memory metrics relating to deallocations with the deallocationSize
         * @param deallocationSize The size of the memory block being deallocated
         * is freed and not after. Function does nothing if nullptr. */
        void TrackDeallocation(size_t deallocationSize);

        /**@brief Retrieves the total allocated bytes over the course of the program
         * @return The total memory usage of the program */
        [[nodiscard]] uint64 GetTotalMemoryUsage() const { return m_TotalMemoryUsage; }

        /**@brief Retrieves the peak allocated bytes over the course of the program
         * @return The peak memory usage of the program */
        [[nodiscard]] uint64 GetPeakMemoryUsage() const { return m_PeakMemoryUsage; }

        /**@brief Retrieves memory usage metrics for the current frame
         * @return The memory usage metrics of the current frame */
        [[nodiscard]] const FrameAllocationData& GetFrameAllocationData() const { return m_FrameAllocationData; }

    private:

        uint64 m_PeakMemoryUsage{};
        uint64 m_TotalMemoryUsage{};
        uint32 m_TotalActiveAllocations{};


        FrameAllocationData m_FrameAllocationData{};

        Core::EventListener<NewFrameEvent> m_NewFrameEventListener{[this](NewFrameEvent)
        {
            m_FrameAllocationData = FrameAllocationData();
        }};
    };

}
