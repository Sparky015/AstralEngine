//
// Created by Andrew Fagan on 1/7/25.
//

#pragma once

#include "Core/Events/EventListener.h"
#include "Core/Memory/PointerAllocationSizeMap.h"
#include "Renderer/RendererEvents.h"

namespace Core {

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
        [[nodiscard]] uint32 GetUnfreedAllocationsInFrame() const { return m_TotalNumberOfAllocationsInFrame - m_TotalNumberOfFreesInFrame; }


        MemoryMetricsManager(const MemoryMetricsManager&) = delete;
        MemoryMetricsManager& operator=(const MemoryMetricsManager&) = delete;
        MemoryMetricsManager(MemoryMetricsManager&&) = delete;
        MemoryMetricsManager& operator=(MemoryMetricsManager&&) = delete;

    private:
        MemoryMetricsManager() = default;
        ~MemoryMetricsManager() = default;

        PointerAllocationSizeMap m_PointerAllocationSizeMap{};

        uint64 m_TotalAllocatedBytes{};
        uint64 m_TotalFreedBytes{};

        uint32 m_TotalNumberOfAllocations{};
        uint32 m_TotalNumberOfFrees{};

        uint32 m_TotalNumberOfAllocationsInFrame{};
        uint32 m_TotalNumberOfFreesInFrame{};

        Event::EventListener<NewFrameEvent> m_NewFrameListener{[this](NewFrameEvent) {
            this->m_TotalNumberOfAllocationsInFrame = 0;
            this->m_TotalNumberOfFreesInFrame = 0;
        }};
    };

}
