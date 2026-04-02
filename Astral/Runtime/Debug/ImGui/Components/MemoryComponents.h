//
// Created by Andrew Fagan on 1/7/25.
//

#pragma once

namespace Astral {

    void GlobalMemoryUsage();
    void PeakMemoryUsage();
    void GlobalTotalAllocationsMade();
    void GlobalActiveAllocations();

    void AllocationsInCurrentFrame();

    void MemoryUsageByAllocator();
    void PeakMemoryUsageByAllocator();
    void TotalAllocationsMadeByAllocator();
    void ActiveAllocationsByAllocator();

    void MemoryUsageByRegion();
    void PeakMemoryUsageByRegion();
    void TotalAllocationsMadeByRegion();
    void ActiveAllocationsByRegion();

    void MemoryUsageByThread();
    void PeakMemoryUsageByThread();
    void TotalAllocationsMadeByThread();
    void ActiveAllocationsByThread();

    void ManageMemoryProfilingScene();

}