/**
* @file MemoryTracker.h
* @author Andrew Fagan
* @date 2/8/25
*/

#pragma once

#include "AllocationData.h"
#include "GlobalAllocationStorage.h"
#include "MemoryMetrics.h"
#include "SceneMetricsExporter.h"

#include <mutex>

namespace Core {

    /**
     * @class MemoryTracker
     * @brief Tracks the memory usage of the engine globally
     * @note This class is a singleton. This class is thread safe.
     */
    class MemoryTracker
    {
    public:

        /**@brief Gets the singleton instance of the MemoryTracker */
        inline static MemoryTracker& Get()
        {
            static MemoryTracker instance = MemoryTracker();
            return instance;
        }

        /**@brief Initializes the necessary components of the MemoryTracker.
         * @warning Call this before using the MemoryTracker */
        void Init();

        /**@brief Shuts down the MemoryTracker and its necessary components
         * @note This will record all the pointers that have not been freed yet which means those pointers are memory leaks. */
        void Shutdown();

        /**@brief Begins a scene that records snapshots of the memory metrics to a file */
        void BeginScene(const char* sceneName);

        /**@brief Tells whether a memory profiling scene is currently active or not. */
        [[nodiscard]] bool IsSceneActive() const;

        /**@brief Ends the scene that records snapshots of the memory metrics to a file */
        void EndScene();

        /**@brief Adds an allocated pointer to tracking and updates the metrics */
        void AddAllocation(void* pointer, size_t size, MemoryRegion region, AllocatorType allocatorType);

        /**@brief Removes an allocated pointer from tracking and updates the metrics
         * @param pointer The pointer to the allocated block being freed */
        void RemoveAllocation(void* pointer);

        void EnableTracking() { m_IsTrackingEnabled = true; }

        void DisableTracking() { m_IsTrackingEnabled = false; }


        /**@brief Gets the memory metrics of the engine
         * @return The memory metrics of the engine */
        [[nodiscard]] constexpr const MemoryMetrics& GetMemoryMetrics() const { return m_MemoryMetrics; }


        MemoryTracker(const MemoryTracker&) = delete;
        MemoryTracker& operator=(const MemoryTracker&) = delete;
        MemoryTracker(MemoryTracker&&) = delete;
        MemoryTracker& operator=(MemoryTracker&&) = delete;

    private:
        MemoryTracker() = default;
        ~MemoryTracker() = default;

        mutable std::recursive_mutex m_Mutex;

        GlobalAllocationStorage m_GlobalAllocationStorage;
        SceneMetricsExporter m_SceneMetricsExporter;
        MemoryMetrics m_MemoryMetrics;

        bool m_IsTrackingEnabled;
    };

}