/**
* @file MemoryTracker.cpp
* @author Andrew Fagan
* @date 2/8/25
*/

#include "MemoryTracker.h"

#include "Core/Utilities/Loggers.h"

#include <ranges>
#include <algorithm>

namespace Astral {

    thread_local bool MemoryTracker::m_IsThreadRecursiveGuardEnabled = false;

    MemoryTracker& MemoryTracker::Get()
    {
        static MemoryTracker instance = MemoryTracker();
        return instance;
    }


    void MemoryTracker::Init()
    {
#ifdef ASTRAL_DISABLE_MEMORY_TRACKING
        return;
#endif
        m_MemoryMetrics.Init();
        m_SceneMetricsExporter.InitExportFile();

        m_IsThreadRecursiveGuardEnabled = true;
        m_DeferredOperationsProcessor = std::jthread([this](){ ProcessorWorkerThreadMain(); });
        m_IsThreadRecursiveGuardEnabled = false;

        m_IsTrackingEnabled = true;
    }


    void MemoryTracker::Shutdown()
    {
#ifdef ASTRAL_DISABLE_MEMORY_TRACKING
        return;
#endif

        m_ShouldStopProcessingOperations = true;
        m_ProcessorConditionalVariable.notify_one();

        m_MemoryMetrics.Shutdown();
        m_IsTrackingEnabled = false;
    }


    void MemoryTracker::BeginScene(const char* sceneName)
    {
#ifdef ASTRAL_DISABLE_MEMORY_TRACKING
        return;
#endif


        m_IsSceneStartEndSyncEnabled = true;
        ProcessDeferredOperationsBuffer();

        std::unique_lock lock(m_MemoryProfilerMutex);

        bool successFlag = m_SceneMetricsExporter.BeginScene(sceneName, m_MemoryMetrics);
        if (!successFlag) { AE_LOG("Memory profiling scene \"" << sceneName << "\" failed to start!") }

        m_IsSceneStartEndSyncEnabled = false;
    }


    bool MemoryTracker::IsSceneActive() const
    {
#ifdef ASTRAL_DISABLE_MEMORY_TRACKING
        return false;
#endif
        return m_SceneMetricsExporter.IsSceneActive();
    }


    void MemoryTracker::EndScene()
    {
#ifdef ASTRAL_DISABLE_MEMORY_TRACKING
        return;
#endif

        m_IsSceneStartEndSyncEnabled = true;
        ProcessDeferredOperationsBuffer();

        std::unique_lock lock(m_MemoryProfilerMutex);
        m_SceneMetricsExporter.EndScene();

        m_IsSceneStartEndSyncEnabled = false;
    }


    void MemoryTracker::AddAllocation(void* pointer, size_t size, MemoryRegion region, MemoryTrackerAllocatorType allocatorType)
    {
#ifdef ASTRAL_DISABLE_MEMORY_TRACKING
        return;
#endif
        if (!IsTrackingEnabled()) { return; }
        if (m_IsThreadRecursiveGuardEnabled) { return; }

        uint64 operationNumber = m_OperationCount.fetch_add(1);

        const AllocationData allocationData = {pointer, size, region, allocatorType, std::this_thread::get_id()};
        DeferOperationTracking({allocationData, operationNumber, true, nullptr});

        if (m_SceneMetricsExporter.IsSceneActive())
        {
            m_SceneMetricsExporter.CaptureRawStacktraceToBuffer();
            m_SceneMetricsExporter.SaveSceneProfilingTimeToBuffer();
        }
    }


    void MemoryTracker::RemoveAllocation(void* pointer)
    {
#ifdef ASTRAL_DISABLE_MEMORY_TRACKING
        return;
#endif
        if (!IsTrackingEnabled()) { return; }
        if (m_IsThreadRecursiveGuardEnabled) { return; }

        uint64 operationNumber = m_OperationCount.fetch_add(1);
        DeferOperationTracking({{}, operationNumber, false, pointer});

        if (m_SceneMetricsExporter.IsSceneActive())
        {
            m_SceneMetricsExporter.CaptureRawStacktraceToBuffer();
            m_SceneMetricsExporter.SaveSceneProfilingTimeToBuffer();
        }
    }


    void MemoryTracker::EnableTracking()
    {
        m_IsTrackingEnabled = true;
    }


    void MemoryTracker::DisableTracking()
    {
        m_IsTrackingEnabled = false;
    }


    bool MemoryTracker::IsTrackingEnabled()
    {
        return m_IsTrackingEnabled;
    }


    void MemoryTracker::EnableThreadRecursiveGuard()
    {
        m_IsThreadRecursiveGuardEnabled = true;
    }


    void MemoryTracker::DisableThreadRecursiveGuard()
    {
        m_IsThreadRecursiveGuardEnabled = false;
    }


    bool MemoryTracker::IsThreadRecursiveGuardEnabled()
    {
        return m_IsThreadRecursiveGuardEnabled;
    }


    void MemoryTracker::ProcessDeferredOperationsBuffer()
    {
        m_ShouldProcessOperations = true;
        m_ProcessorConditionalVariable.notify_one();
        while (m_ShouldProcessOperations) { std::this_thread::sleep_for(std::chrono::microseconds(100)); } // Wait while the deferred processing finishes
    }


    size_t MemoryTracker::GetPeakMemoryUsage() const
    {
        std::shared_lock readerLock(m_MemoryMetricsRWLock);
        return m_MemoryMetrics.GetPeakMemoryUsage();
    }


    size_t MemoryTracker::GetTotalMemoryUsage() const
    {
        std::shared_lock readerLock{m_MemoryMetricsRWLock};
        return m_MemoryMetrics.GetTotalMemoryUsage();
    }


    size_t MemoryTracker::GetTotalActiveAllocations() const
    {
        std::shared_lock readerLock(m_MemoryMetricsRWLock);
        return m_MemoryMetrics.GetTotalActiveAllocations();
    }


    size_t MemoryTracker::GetTotalAllocations() const
    {
        std::shared_lock readerLock(m_MemoryMetricsRWLock);
        return m_MemoryMetrics.GetTotalAllocations();
    }


    FrameAllocationData MemoryTracker::GetFrameAllocationData() const
    {
        std::shared_lock readerLock(m_MemoryMetricsRWLock);
        return m_MemoryMetrics.GetFrameAllocationData();
    }


    size_t MemoryTracker::GetAllocatorTypeUsage(MemoryTrackerAllocatorType allocatorType) const
    {
        std::shared_lock readerLock(m_MemoryMetricsRWLock);
        return m_MemoryMetrics.GetAllocatorTypeUsage(allocatorType);
    }


    size_t MemoryTracker::GetAllocatorTypePeakUsage(MemoryTrackerAllocatorType allocatorType) const
    {
        std::shared_lock readerLock(m_MemoryMetricsRWLock);
        return m_MemoryMetrics.GetAllocatorTypePeakUsage(allocatorType);
    }


    size_t MemoryTracker::GetMemoryRegionUsage(MemoryRegion memoryRegion) const
    {
        std::shared_lock readerLock(m_MemoryMetricsRWLock);
        return m_MemoryMetrics.GetMemoryRegionUsage(memoryRegion);
    }


    size_t MemoryTracker::GetMemoryRegionPeakUsage(MemoryRegion memoryRegion) const
    {
        std::shared_lock readerLock(m_MemoryMetricsRWLock);
        return m_MemoryMetrics.GetMemoryRegionPeakUsage(memoryRegion);
    }


    size_t MemoryTracker::GetThreadUsage(std::thread::id threadID) const
    {
        std::shared_lock readerLock(m_MemoryMetricsRWLock);
        return m_MemoryMetrics.GetThreadUsage(threadID);
    }


    size_t MemoryTracker::GetThreadPeakUsage(std::thread::id threadID) const
    {
        std::shared_lock readerLock(m_MemoryMetricsRWLock);
        return m_MemoryMetrics.GetThreadPeakUsage(threadID);
    }


    size_t MemoryTracker::GetThreadActiveAllocations(const std::thread::id threadID) const
    {
        std::shared_lock readerLock(m_MemoryMetricsRWLock);
        return m_MemoryMetrics.GetThreadActiveAllocations(threadID);
    }


    size_t MemoryTracker::GetThreadTotalAllocations(const std::thread::id threadID) const
    {
        std::shared_lock readerLock(m_MemoryMetricsRWLock);
        return m_MemoryMetrics.GetThreadTotalAllocations(threadID);
    }

    MemoryMetrics::AllocatorTypeMap MemoryTracker::GetMemoryUsageByAllocatorIterable() const
    {
        std::shared_lock readerLock(m_MemoryMetricsRWLock);
        return m_MemoryMetrics.GetMemoryUsageByAllocatorIterable();
    }

    MemoryMetrics::AllocatorTypeMap MemoryTracker::GetPeakMemoryUsageByAllocatorIterable() const
    {
        std::shared_lock readerLock(m_MemoryMetricsRWLock);
        return m_MemoryMetrics.GetPeakMemoryUsageByAllocatorIterable();
    }

    MemoryMetrics::AllocatorTypeMap MemoryTracker::GetActiveAllocationsByAllocatorIterable() const
    {
        std::shared_lock readerLock(m_MemoryMetricsRWLock);
        return m_MemoryMetrics.GetActiveAllocationsByAllocatorIterable();
    }

    MemoryMetrics::AllocatorTypeMap MemoryTracker::GetTotalAllocationsByAllocatorIterable() const
    {
        std::shared_lock readerLock(m_MemoryMetricsRWLock);
        return m_MemoryMetrics.GetTotalAllocationsByAllocatorIterable();
    }

    MemoryMetrics::MemoryRegionMap MemoryTracker::GetMemoryUsageByRegionIterable() const
    {
        std::shared_lock readerLock(m_MemoryMetricsRWLock);
        return m_MemoryMetrics.GetMemoryUsageByRegionIterable();
    }

    MemoryMetrics::MemoryRegionMap MemoryTracker::GetPeakMemoryUsageByRegionIterable() const
    {
        std::shared_lock readerLock(m_MemoryMetricsRWLock);
        return m_MemoryMetrics.GetPeakMemoryUsageByRegionIterable();
    }

    MemoryMetrics::MemoryRegionMap MemoryTracker::GetActiveAllocationsByRegionIterable() const
    {
        std::shared_lock readerLock(m_MemoryMetricsRWLock);
        return m_MemoryMetrics.GetActiveAllocationsByRegionIterable();
    }

    MemoryMetrics::MemoryRegionMap MemoryTracker::GetTotalAllocationsByRegionIterable() const
    {
        std::shared_lock readerLock(m_MemoryMetricsRWLock);
        return m_MemoryMetrics.GetTotalAllocationsByRegionIterable();
    }

    MemoryMetrics::ThreadMap MemoryTracker::GetMemoryUsageByThreadIterable() const
    {
        std::shared_lock readerLock(m_MemoryMetricsRWLock);
        return m_MemoryMetrics.GetMemoryUsageByThreadIterable();
    }

    MemoryMetrics::ThreadMap MemoryTracker::GetPeakMemoryUsageByThreadIterable() const
    {
        std::shared_lock readerLock(m_MemoryMetricsRWLock);
        return m_MemoryMetrics.GetPeakMemoryUsageByThreadIterable();
    }

    MemoryMetrics::ThreadMap MemoryTracker::GetActiveAllocationsByThreadIterable() const
    {
        std::shared_lock readerLock(m_MemoryMetricsRWLock);
        return m_MemoryMetrics.GetActiveAllocationsByThreadIterable();
    }

    MemoryMetrics::ThreadMap MemoryTracker::GetTotalAllocationsByThreadIterable() const
    {
        std::shared_lock readerLock(m_MemoryMetricsRWLock);
        return m_MemoryMetrics.GetTotalAllocationsByThreadIterable();
    }


    MemoryTracker::MemoryTracker() :
        m_IsTrackingEnabled(false)
    {
    }

    MemoryTracker::~MemoryTracker()
    {
        m_SceneMetricsExporter.EndScene();
        m_IsTrackingEnabled = false;
    }


    void MemoryTracker::DeferOperationTracking(const DeferredTrackingOperation& operation)
    {
        while (m_IsSceneStartEndSyncEnabled)
        {
            // spin until sync stops
        }

        std::shared_lock readOperationsBufferLock{m_ThreadOperationBuffersRWMutex};

        m_IsThreadRecursiveGuardEnabled = true; // To avoid this thread triggering allocations that the memory tracker picks up

        if (!m_ThreadOperationBuffers.contains(std::this_thread::get_id()))
        {
            readOperationsBufferLock.unlock();
            std::unique_lock writeLock{m_ThreadOperationBuffersRWMutex};
            m_ThreadOperationBuffers[std::this_thread::get_id()] = {};
            writeLock.unlock();
            readOperationsBufferLock.lock();
        }

        std::vector<DeferredTrackingOperation>& threadOperationBuffer = m_ThreadOperationBuffers.at(std::this_thread::get_id());
        threadOperationBuffer.push_back(operation);


        if (threadOperationBuffer.size() >= 1000)
        {
            m_ShouldProcessOperations = true;
            m_ProcessorConditionalVariable.notify_one();
            readOperationsBufferLock.unlock(); // Unlock after threadOperationBuffer read
        }
        else
        {
            readOperationsBufferLock.unlock(); // Unlock after threadOperationBuffer read
        }

        m_IsThreadRecursiveGuardEnabled = false;
    }


    void MemoryTracker::ProcessDeferredOperations()
    {
        m_IsThreadRecursiveGuardEnabled = true;

        {
            std::vector<DeferredTrackingOperation> inOrderOperations = {};

            std::unique_lock threadOperationsWriteLock{m_ThreadOperationBuffersRWMutex};

            for (auto& [threadID, deferredBuffer] : m_ThreadOperationBuffers)
            {
                for (DeferredTrackingOperation& operation : deferredBuffer)
                {
                    inOrderOperations.push_back(operation);
                }

                deferredBuffer.clear();
            }

            threadOperationsWriteLock.unlock();

            std::ranges::sort(inOrderOperations, {}, &DeferredTrackingOperation::OperationNumber);

            // m_OperationCount = 0;

            for (size_t i = 0; i < inOrderOperations.size(); i++)
            {
                DeferredTrackingOperation& operation = inOrderOperations[i];

                if (operation.IsAddingAllocation)
                {
                    if (m_SceneMetricsExporter.IsSceneActive())
                    {
                        m_SceneMetricsExporter.SaveOperationDataToBuffer(operation.AllocationData, false);
                    }

                    m_GlobalAllocationStorage.AddPointer(operation.AllocationData);

                    std::unique_lock memoryMetricsWriteLock(m_MemoryMetricsRWLock);
                    m_MemoryMetrics.TrackAllocation(operation.AllocationData);
                }
                else
                {
                    if (!m_GlobalAllocationStorage.IsPointerStored(operation.Pointer))
                    {
                        if (m_SceneMetricsExporter.IsSceneActive())
                        {
                            AllocationData emptyData = {};
                            emptyData.pointer = nullptr;
                            m_SceneMetricsExporter.SaveOperationDataToBuffer(emptyData, true);
                        }

                        continue;
                    }
                    const AllocationData& allocationData = m_GlobalAllocationStorage.GetPointerData(operation.Pointer);

                    if (m_SceneMetricsExporter.IsSceneActive())
                    {
                        m_SceneMetricsExporter.SaveOperationDataToBuffer(allocationData, true);
                    }

                    std::unique_lock memoryMetricsWriteLock(m_MemoryMetricsRWLock);
                    m_MemoryMetrics.TrackDeallocation(allocationData);
                    memoryMetricsWriteLock.unlock();

                    m_GlobalAllocationStorage.FreePointer(operation.Pointer);
                }
            }

        }

        m_IsThreadRecursiveGuardEnabled = false;
    }


    void MemoryTracker::ProcessorWorkerThreadMain()
    {
        while (!m_ShouldStopProcessingOperations)
        {
            std::unique_lock writeLock{m_ProcessorMutex};
            m_ProcessorConditionalVariable.wait(writeLock, [this](){ return m_ShouldProcessOperations || m_ShouldStopProcessingOperations; });

            if (m_ShouldStopProcessingOperations) { return;}

            ProcessDeferredOperations();

            m_ShouldProcessOperations = false;
        }
    }

}


