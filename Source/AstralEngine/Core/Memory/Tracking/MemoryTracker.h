/**
* @file MemoryTracker.h
* @author Andrew Fagan
* @date 2/8/25
*/

#pragma once

#include "GlobalAllocationStorage.h"
#include "MemoryMetrics.h"
#include "TrackingSceneManager.h"

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

    class MemoryTracker
    {
    public:
        MemoryTracker();

        void BeginScene();
        void EndScene();

        void AddAllocation(void* pointer, size_t size, MemoryRegions region, AllocatorType allocatorType);

        void RemoveAllocation(void* pointer);

        // Get memory metrics

    private:

        GlobalAllocationStorage m_GlobalAllocationStorage;
        TrackingSceneManager m_TrackingSceneManager;
        MemoryMetrics m_MemoryMetrics;
    };

}