/**
* @file MemoryTracker.h
* @author Andrew Fagan
* @date 2/8/25
*/

#pragma once

#include "AllocationData.h"
#include "Core/Threading/Locks/ReaderBiasedRWLock.h"
#include "GlobalAllocationStorage.h"
#include "MemoryMetrics.h"
#include "Serialization/SceneMetricsExporter.h"

#include <shared_mutex>

namespace Astral {

    /**
     * @class MemoryTracker
     * @brief Tracks the memory usage of the engine globally
     * @note This class is a singleton. This class is thread safe.
     */
    class MemoryTracker
    {
    public:

        /**
         * @brief Gets the singleton instance of the MemoryTracker
         */
        static MemoryTracker& Get();

        /**
         * @brief Initializes the MemoryTracker.
         */
        void Init();

        /**
         * @brief Shuts down the MemoryTracker
         */
        void Shutdown();

        /**
         * @brief Begins a scene that records snapshots of the memory metrics to a file
         */
        void BeginScene(const char* sceneName);

        /**
         * @brief Tells whether a memory profiling scene is currently active or not.
         */
        [[nodiscard]] bool IsSceneActive() const;

        /**
         * @brief Ends the scene that records snapshots of the memory metrics to a file
         */
        void EndScene();

        /**
         * @brief Adds an allocated pointer to tracking and updates the metrics
         */
        void AddAllocation(void* pointer, size_t size, MemoryRegion region, MemoryTrackerAllocatorType allocatorType);

        /**
         * @brief Removes an allocated pointer from tracking and updates the metrics
         * @param pointer The pointer to the allocated block being freed
         */
        void RemoveAllocation(void* pointer);

        /**
         * @brief Enables memory tracking
         */
        void EnableTracking();

        /**
         * @brief Disables memory tracking
         */
        void DisableTracking();

        /**
         * @brief Checks if memory tracking is enabled
         * @return True if memory tracking is enabled, false otherwise
         */
        bool IsTrackingEnabled();

        /**
         * @brief Enables the thread recursive guard
         * @note This protects the memory profiler from tracking allocations made internally by the profiler
         */
        void EnableThreadRecursiveGuard();

        /**
         * @brief Disables the thread recursive guard
         */
        void DisableThreadRecursiveGuard();

        /**
         * @brief Checks if the thread recursive guard is enabled
         * @return True if the thread recursive guard is enabled, false otherwise
         */
        bool IsThreadRecursiveGuardEnabled();

        /**
         * @brief Processes the deferred operations buffer to provide the most up to date memory stats
         * @note This should only be called for to update the memory tracker to the most up to date memory stats. The deferred operations
         * buffer will be processed automatically after a certain amount of allocations are made.
         */
        void ProcessDeferredOperationsBuffer();

        /**
         * @brief Retrieves the peak allocated bytes over the course of the program
         * @return The peak memory usage of the program
         */
        [[nodiscard]] size_t GetPeakMemoryUsage() const;

        /**
         * @brief Retrieves the total allocated bytes over the course of the program
         * @return The total memory usage of the program
         */
        [[nodiscard]] size_t GetTotalMemoryUsage() const;

        /**
         * @brief Retrieves the current active allocations (Number of allocations that were allocated but not freed yet)
         * @return The current active allocation count
         */
        [[nodiscard]] size_t GetTotalActiveAllocations() const;

        /**
         * @brief Retrieves the current total allocations made
         * @return The current total allocation count
         */
        [[nodiscard]] size_t GetTotalAllocations() const;

        /**
         * @brief Retrieves memory usage metrics for the current frame
         * @return The memory usage metrics of the current frame
         */
        [[nodiscard]] FrameAllocationData GetFrameAllocationData() const;

        /**
         * @brief Gets the memory usage of the given allocator type
         * @param allocatorType The allocator type to get the memory usage of
         * @return The memory usage of the given allocator type
         */
        [[nodiscard]] size_t GetAllocatorTypeUsage(MemoryTrackerAllocatorType allocatorType) const;

        /**
         * @brief Gets the peak memory usage of the given allocator type
         * @param allocatorType The allocator type to get the peak memory usage of
         * @return The peak memory usage of the given allocator type
         */
        [[nodiscard]] size_t GetAllocatorTypePeakUsage(MemoryTrackerAllocatorType allocatorType) const;

        /**
         * @brief Gets the memory usage of the given memory region
         * @param memoryRegion The memory region to get the memory usage of
         * @return The memory usage of the given memory region
         */
        [[nodiscard]] size_t GetMemoryRegionUsage(MemoryRegion memoryRegion) const;

        /**
         * @brief Gets the peak memory usage of the given memory region
         * @param memoryRegion The memory region to get the peak memory usage of
         * @return The peak memory usage of the given memory region
         */
        [[nodiscard]] size_t GetMemoryRegionPeakUsage(MemoryRegion memoryRegion) const;

        /**
         * @brief Gets the memory usage of the given thread
         * @param threadID The thread ID of the thread to get the memory usage of
         * @return The memory usage of the given thread
         */
        [[nodiscard]] size_t GetThreadUsage(std::thread::id threadID) const;

        /**
         * @brief Gets the peak memory usage of the given thread
         * @param threadID The thread ID of the thread to get the peak memory usage of
         * @return The peak memory usage of the given thread
         */
        [[nodiscard]] size_t GetThreadPeakUsage(std::thread::id threadID) const;

        /**
         * @brief Gets the active allocation count of the given thread
         * @param threadID The thread ID of the thread to get the active allocation count of
         * @return The active allocation count of the given thread
         */
        [[nodiscard]] size_t GetThreadActiveAllocations(std::thread::id threadID) const;

        /**
         * @brief Gets the total allocation count of the given thread
         * @param threadID The thread ID of the thread to get the total allocation count of
         * @return The total allocation count of the given thread
         */
        [[nodiscard]] size_t GetThreadTotalAllocations(std::thread::id threadID) const;

        /**
         * @brief Gets an iterable data structure of the memory usages by allocator type
         * @return An iterable data structure of the memory usages by allocator type
         */
        [[nodiscard]] MemoryMetrics::AllocatorTypeMap GetMemoryUsageByAllocatorIterable() const;

        /**
        * @brief Gets an iterable data structure of the peak memory usages by allocator type
        * @return An iterable data structure of the peak memory usages by allocator type
        */
        [[nodiscard]] MemoryMetrics::AllocatorTypeMap GetPeakMemoryUsageByAllocatorIterable() const;

        /**
        * @brief Gets an iterable data structure of the active allocations by allocator type
        * @return An iterable data structure of the active allocations by allocator type
        */
        [[nodiscard]] MemoryMetrics::AllocatorTypeMap GetActiveAllocationsByAllocatorIterable() const;

        /**
        * @brief Gets an iterable data structure of the total allocations by allocator type
        * @return An iterable data structure of the total allocations by allocator type
        */
        [[nodiscard]] MemoryMetrics::AllocatorTypeMap GetTotalAllocationsByAllocatorIterable() const;

        /**
        * @brief Gets an iterable data structure of the memory usage by memory region
        * @return An iterable data structure of the memory usage by memory region
        */
        [[nodiscard]] MemoryMetrics::MemoryRegionMap GetMemoryUsageByRegionIterable() const;

        /**
        * @brief Gets an iterable data structure of the peak memory usage by memory region
        * @return An iterable data structure of the peak memory usage by memory region
        */
        [[nodiscard]] MemoryMetrics::MemoryRegionMap GetPeakMemoryUsageByRegionIterable() const;

        /**
        * @brief Gets an iterable data structure of the active allocations by memory region
        * @return An iterable data structure of the active allocations by memory region
        */
        [[nodiscard]] MemoryMetrics::MemoryRegionMap GetActiveAllocationsByRegionIterable() const;

        /**
        * @brief Gets an iterable data structure of the total allocations by memory region
        * @return An iterable data structure of the total allocations by memory region
        */
        [[nodiscard]] MemoryMetrics::MemoryRegionMap GetTotalAllocationsByRegionIterable() const;

        /**
        * @brief Gets an iterable data structure of the memory usage by thread
        * @return An iterable data structure of the memory usage by thread
        */
        [[nodiscard]] MemoryMetrics::ThreadMap GetMemoryUsageByThreadIterable() const;

        /**
        * @brief Gets an iterable data structure of the peak memory usage by thread
        * @return An iterable data structure of the peak memory usage by thread
        */
        [[nodiscard]] MemoryMetrics::ThreadMap GetPeakMemoryUsageByThreadIterable() const;

        /**
        * @brief Gets an iterable data structure of the active allocations by thread
        * @return An iterable data structure of the active allocations by thread
        */
        [[nodiscard]] MemoryMetrics::ThreadMap GetActiveAllocationsByThreadIterable() const;

        /**
        * @brief Gets an iterable data structure of the total allocations by thread
        * @return An iterable data structure of the total allocations by thread
        */
        [[nodiscard]] MemoryMetrics::ThreadMap GetTotalAllocationsByThreadIterable() const;


        MemoryTracker(const MemoryTracker&) = delete;
        MemoryTracker& operator=(const MemoryTracker&) = delete;
        MemoryTracker(MemoryTracker&&) = delete;
        MemoryTracker& operator=(MemoryTracker&&) = delete;

    private:
        MemoryTracker();
        ~MemoryTracker();

        struct DeferredTrackingOperation
        {
            AllocationData AllocationData; // Used only for allocation operations
            uint64 OperationNumber;
            bool IsAddingAllocation;
            void* Pointer; // Used only for free operations
        };

        void DeferOperationTracking(const DeferredTrackingOperation& operation);
        void ProcessDeferredOperations();
        void ProcessorWorkerThreadMain();

        mutable std::shared_mutex m_MemoryProfilerMutex;

        GlobalAllocationStorage m_GlobalAllocationStorage;
        SceneMetricsExporter m_SceneMetricsExporter;
        MemoryMetrics m_MemoryMetrics;
        mutable ReaderBiasedRWLock m_MemoryMetricsRWLock;
        std::atomic<bool> m_IsTrackingEnabled;

        ReaderBiasedRWLock m_ThreadOperationBuffersRWMutex;
        std::unordered_map<std::thread::id, std::vector<DeferredTrackingOperation>> m_ThreadOperationBuffers;
        std::atomic<uint64> m_OperationCount;
        static thread_local bool m_IsThreadRecursiveGuardEnabled;

        std::jthread m_DeferredOperationsProcessor;
        std::mutex m_ProcessorMutex;
        std::condition_variable m_ProcessorConditionalVariable;
        std::atomic<bool> m_ShouldProcessOperations;
        std::atomic<bool> m_ShouldStopProcessingOperations;
        std::atomic<bool> m_IsSceneStartEndSyncEnabled;
    };

}
