/**
* @file SceneMetricsGraphComponents.h
* @author Andrew Fagan
* @date 2/9/2025
*/

#pragma once

#include "Core/Memory/Tracking/SceneMetricsStorage.h"

namespace Core {


    /**@brief  */
    void GlobalMemoryUsageGraph(const SceneMetricsStorage& storage);

    /**@brief  */
    void GlobalPeakMemoryUsageGraph(const SceneMetricsStorage& storage);

    /**@brief  */
    void GlobalActiveAllocationGraph(const SceneMetricsStorage& storage);

    /**@brief  */
    void GlobalTotalAllocationGraph(const SceneMetricsStorage& storage);

    /**@brief  */
    void AllocatorTypeUsageGraph(const SceneMetricsStorage& storage);

    /**@brief  */
    void MemoryRegionUsage(const SceneMetricsStorage& storage);

    /**@brief  */
    void ThreadUsageGraph(const SceneMetricsStorage& storage);

    /**@brief  */
    void NewAllocationsInFrameGraph(const SceneMetricsStorage& storage);

}
