/**
* @file SceneMetricsGraphs.h
* @author Andrew Fagan
* @date 2/9/2025
*/

#pragma once

#include "Core/Memory/Tracking/SceneMetricsStorage.h"

namespace Core {

    /**@brief  */
    void ThreadUsageGraph(const SceneMetricsStorage& sceneMetricsStorage);

    /**@brief  */
    void TotalMemoryUsageGraph(const SceneMetricsStorage& sceneMetricsStorage);

    /**@brief  */
    void ActiveAllocationGraph(const SceneMetricsStorage& sceneMetricsStorage);

    /**@brief  */
    void AllocatorTypeUsageGraph(const SceneMetricsStorage& sceneMetricsStorage);

    /**@brief  */
    void MemoryRegionUsage(const SceneMetricsStorage& sceneMetricsStorage);

    /**@brief  */
    void NewAllocationsInFrameGraph(const SceneMetricsStorage& sceneMetricsStorage);

}
