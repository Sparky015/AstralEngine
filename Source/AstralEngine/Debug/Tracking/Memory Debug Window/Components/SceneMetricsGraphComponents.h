/**
* @file SceneMetricsGraphComponents.h
* @author Andrew Fagan
* @date 2/9/2025
*/

#pragma once

#include "Debug/Tracking/SceneMetricsStorage.h"

namespace Core {


    /**@brief  */
    void GlobalMemoryUsageGraph(const SceneMetricsStorage& storage, size_t selectedPointIndex);

    /**@brief  */
    void GlobalPeakMemoryUsageGraph(const SceneMetricsStorage& storage, size_t selectedPointIndex);

    /**@brief  */
    void GlobalActiveAllocationGraph(const SceneMetricsStorage& storage, size_t selectedPointIndex);

    /**@brief  */
    void GlobalTotalAllocationGraph(const SceneMetricsStorage& storage, size_t selectedPointIndex);

    /**@brief  */
    void AllocatorMemoryUsageGraph(const SceneMetricsStorage& storage, size_t selectedPointIndex);

    /**@brief  */
    void AllocatorPeakMemoryUsageGraph(const SceneMetricsStorage& storage, size_t selectedPointIndex);

    /**@brief  */
    void AllocatorActiveAllocationsUsageGraph(const SceneMetricsStorage& storage, size_t selectedPointIndex);

    /**@brief  */
    void AllocatorTotalAllocationsUsageGraph(const SceneMetricsStorage& storage, size_t selectedPointIndex);

    /**@brief  */
    void RegionMemoryUsageGraph(const SceneMetricsStorage& storage, size_t selectedPointIndex);

    /**@brief  */
    void RegionPeakMemoryUsageGraph(const SceneMetricsStorage& storage, size_t selectedPointIndex);

    /**@brief  */
    void RegionActiveAllocationsUsageGraph(const SceneMetricsStorage& storage, size_t selectedPointIndex);

    /**@brief  */
    void RegionTotalAllocationsUsageGraph(const SceneMetricsStorage& storage, size_t selectedPointIndex);

    /**@brief  */
    void ThreadMemoryUsageGraph(const SceneMetricsStorage& storage, size_t selectedPointIndex);

    /**@brief  */
    void ThreadPeakMemoryUsageGraph(const SceneMetricsStorage& storage, size_t selectedPointIndex);

    /**@brief  */
    void ThreadActiveAllocationsUsageGraph(const SceneMetricsStorage& storage, size_t selectedPointIndex);

    /**@brief  */
    void ThreadTotalAllocationsUsageGraph(const SceneMetricsStorage& storage, size_t selectedPointIndex);


    /**@brief  */
    void NewAllocationsInFrameGraph(const SceneMetricsStorage& storage, size_t selectedPointIndex);

}
