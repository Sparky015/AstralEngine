//
// Created by Andrew Fagan on 1/7/25.
//

#pragma once

#include "Core/Events/EventListener.h"
#include "Core/Memory/PointerAllocationSizeMap.h"
#include "Renderer/RendererEvents.h"
#include <mutex>

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


    /**@class MemoryMetricsManager
     * @brief Tracks global allocations and frees and provides getters for memory usage stats.
     * @note This class is a singleton. This class is thread safe. */
    class MemoryMetricsManager
    {
    public:

        /**@brief Gets the singleton instance of MemoryMetricsManager */
        inline static MemoryMetricsManager& Get()
        {
            static MemoryMetricsManager instance = MemoryMetricsManager();
            return instance;
        }

        /**@brief Updates allocation metrics with allocation data
         * @param allocatedPointer The pointer that was allocated memory. Function does nothing if nullptr.
         * @param allocationSize The size of the memory block allocated to allocatedPointer. */
        void Allocate(void* allocatedPointer, size_t allocationSize);

        /**@brief Updates free metrics and tracked data for given pointer
         * @param pointerToBeFreed The pointer that is about to be freed. Must be called before the pointer
         * is freed and not after. Function does nothing if nullptr. */
        void Free(void* pointerToBeFreed);

        /**@brief Initializes the MemoryMetricsManager. Call before using MemoryMetricsManager */
        void Init();

        /**@brief Shuts down the MemoryMetricsManager. Call when done using MemoryMetricsManager */
        void Shutdown();

        /**@brief Retrieves the total allocated bytes over the course of the program */
        [[nodiscard]] uint64 GetTotalAllocatedBytes() const { return m_TotalAllocatedBytes; }

        /**@brief Retrieves the total freed bytes over the course of the program */
        [[nodiscard]] uint64 GetTotalFreedBytes() const { return m_TotalFreedBytes; }

        /**@brief Retrieves the total number of allocations over the course of the program */
        [[nodiscard]] uint32 GetTotalNumberOfAllocations() const { return m_TotalNumberOfAllocations; }

        /**@brief Retrieves the total number of frees over the course of the program */
        [[nodiscard]] uint32 GetTotalNumberOfFrees() const { return m_TotalNumberOfFrees; }

        /**@brief Retrieves the amount of allocations that were allocated in the previous frame but not freed before the end of the frame */
        [[nodiscard]] uint32 GetUnfreedAllocationsInFrame() const { return m_FrameAllocationData.NumberOfAllocations - m_FrameAllocationData.NumberOfFrees; }

        /**@brief Retrieves memory usage metrics for the previous frame */
        [[nodiscard]] const FrameAllocationData& GetFrameAllocationData() const { return m_FrameAllocationData; }

        MemoryMetricsManager(const MemoryMetricsManager&) = delete;
        MemoryMetricsManager& operator=(const MemoryMetricsManager&) = delete;
        MemoryMetricsManager(MemoryMetricsManager&&) = delete;
        MemoryMetricsManager& operator=(MemoryMetricsManager&&) = delete;

    private:
        MemoryMetricsManager() = default;
        ~MemoryMetricsManager() = default;

        std::recursive_mutex m_Mutex{};

        PointerAllocationSizeMap m_PointerAllocationSizeMap{};

        uint64 m_TotalAllocatedBytes{};
        uint64 m_TotalFreedBytes{};
        uint32 m_TotalNumberOfAllocations{};
        uint32 m_TotalNumberOfFrees{};

        FrameAllocationData m_FrameAllocationData{};

        Core::EventListener<NewFrameEvent> m_NewFrameEventListener{[this](NewFrameEvent)
        {
            m_FrameAllocationData = FrameAllocationData();
        }};
    };

}
