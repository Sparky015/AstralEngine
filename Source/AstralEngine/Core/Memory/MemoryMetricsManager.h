//
// Created by Andrew Fagan on 1/7/25.
//

#pragma once

#include "Core/Events/EventListener.h"
#include "Core/Memory/PointerAllocationSizeMap.h"
#include "Renderer/RendererEvents.h"
#include <mutex>

namespace Core {

    struct FrameAllocationData
    {
        uint32 AllocatedBytes{};
        uint32 FreedBytes{};
        uint32 NumberOfAllocations{};
        uint32 NumberOfFrees{};
    };

    class MemoryMetricsManager
    {
    public:

        inline static MemoryMetricsManager& Get()
        {
            static MemoryMetricsManager instance = MemoryMetricsManager();
            return instance;
        }

        void Allocate(void* pointer, size_t allocationSize);
        void Free(void* pointer);

        void Init();
        void Shutdown();

        [[nodiscard]] uint64 GetTotalAllocatedBytes() const { return m_TotalAllocatedBytes; }
        [[nodiscard]] uint64 GetTotalFreedBytes() const { return m_TotalFreedBytes; }
        [[nodiscard]] uint32 GetTotalNumberOfAllocations() const { return m_TotalNumberOfAllocations; }
        [[nodiscard]] uint32 GetTotalNumberOfFrees() const { return m_TotalNumberOfFrees; }
        [[nodiscard]] uint32 GetUnfreedAllocationsInFrame() const { return m_FrameAllocationData.NumberOfAllocations - m_FrameAllocationData.NumberOfFrees; }
        [[nodiscard]] const FrameAllocationData& GetFrameAllocationData() const { return m_FrameAllocationData; }


        MemoryMetricsManager(const MemoryMetricsManager&) = delete;
        MemoryMetricsManager& operator=(const MemoryMetricsManager&) = delete;
        MemoryMetricsManager(MemoryMetricsManager&&) = delete;
        MemoryMetricsManager& operator=(MemoryMetricsManager&&) = delete;

    private:
        MemoryMetricsManager() = default;
        ~MemoryMetricsManager() = default;

        // To protect against race conditions from the multiple threads spawned by GLFW. My new and delete redefines
        // get called on all threads, so this needs to be thread safe.
        std::recursive_mutex m_Mutex{};

        PointerAllocationSizeMap m_PointerAllocationSizeMap{};

        uint64 m_TotalAllocatedBytes{};
        uint64 m_TotalFreedBytes{};
        uint32 m_TotalNumberOfAllocations{};
        uint32 m_TotalNumberOfFrees{};

        FrameAllocationData m_FrameAllocationData{};

        Event::EventListener<NewFrameEvent> m_NewFrameListener{[this](NewFrameEvent)
        {
            m_FrameAllocationData = FrameAllocationData();
        }};
    };

}
