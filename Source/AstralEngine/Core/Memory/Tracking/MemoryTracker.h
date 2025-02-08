/**
* @file MemoryTracker.h
* @author Andrew Fagan
* @date 2/8/25
*/

#pragma once

#include "GlobalAllocationStorage.h"
#include "MemoryMetrics.h"
#include "TrackingSceneManager.h"

#include <mutex>

namespace Core {

    enum class MemoryRegions : uint8
    {
        RENDERER,
        WINDOW,
        ASSETS,
        ECS,
        DEBUG,
        CORE,
    };

    enum class AllocatorType : uint8
    {
        STACK,
        RING,
        FRAME,
        LINEAR,
        POOL,
        DOUBLE_BUFFERED,

        ALIGNED_ALLOCATOR,
        NEW_OPERATOR,
    };

    /**
     * @class MemoryTracker
     * @brief
     * @note This class is a singleton. This class is thread safe.
     */
    class MemoryTracker
    {
    public:

        inline static MemoryTracker& Get()
        {
            static MemoryTracker instance = MemoryTracker();
            return instance;
        }

        void Init();
        void Shutdown();

        void BeginScene() const;
        void EndScene() const;

        void AddAllocation(void* pointer, size_t size); //, MemoryRegions region, AllocatorType allocatorType);

        void RemoveAllocation(void* pointer);

        constexpr const MemoryMetrics& GetMemoryMetrics() const { return m_MemoryMetrics; }


        MemoryTracker(const MemoryTracker&) = delete;
        MemoryTracker& operator=(const MemoryTracker&) = delete;
        MemoryTracker(MemoryTracker&&) = delete;
        MemoryTracker& operator=(MemoryTracker&&) = delete;

    private:
        MemoryTracker();
        ~MemoryTracker();

        std::recursive_mutex m_Mutex;

        GlobalAllocationStorage m_GlobalAllocationStorage;
        TrackingSceneManager m_TrackingSceneManager;
        MemoryMetrics m_MemoryMetrics;

    };

}