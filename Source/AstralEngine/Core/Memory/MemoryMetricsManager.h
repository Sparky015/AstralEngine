//
// Created by Andrew Fagan on 1/7/25.
//

#pragma once
#include "Core/Events/EventListener.h"
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


        void Allocate(size_t allocationSize);
        void Free(size_t freeSize);

        void Init();
        void Shutdown();

        [[nodiscard]] uint64 GetTotalAllocatedBytes() const { return m_TotalAllocatedBytes; }
        [[nodiscard]] uint64 GetTotalFreedBytes() const { return m_TotalFreedBytes; }
        [[nodiscard]] uint32 GetTotalNumberOfAllocations() const { return m_TotalNumberOfAllocations; }
        [[nodiscard]] uint32 GetTotalNumberOfFrees() const { return m_TotalNumberOfFrees; }
        [[nodiscard]] uint32 GetAliveAllocationsInFrame() const { return m_TotalNumberOfAllocationsPerFrame - m_TotalNumberOfFreesPerFrame; }


        MemoryMetricsManager(const MemoryMetricsManager&) = delete;
        MemoryMetricsManager& operator=(const MemoryMetricsManager&) = delete;
        MemoryMetricsManager(MemoryMetricsManager&&) = delete;
        MemoryMetricsManager& operator=(MemoryMetricsManager&&) = delete;

    private:
        MemoryMetricsManager() = default;
        ~MemoryMetricsManager() = default;

        uint64 m_TotalAllocatedBytes{};
        uint64 m_TotalFreedBytes{};
        uint32 m_TotalNumberOfAllocations{};
        uint32 m_TotalNumberOfFrees{};
        uint32 m_TotalNumberOfAllocationsPerFrame{};
        uint32 m_TotalNumberOfFreesPerFrame{};

        Event::EventListener<NewFrameEvent> m_NewFrameListener{[this](NewFrameEvent) {
            this->m_TotalNumberOfAllocationsPerFrame = 0;
            this->m_TotalNumberOfFreesPerFrame = 0;
        }};
    };

}
