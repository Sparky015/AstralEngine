/**
* @file SceneMetricsGraphs.h
* @author Andrew Fagan
* @date 2/9/2025
*/

#pragma once

#include "SceneMetricsStorage.h"

#include <string_view>

namespace Core {

    /**@brief  */
    std::string_view SelectFile();

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
