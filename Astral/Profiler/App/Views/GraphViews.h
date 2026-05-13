/**
* @file GraphViews.h
* @author Andrew Fagan
* @date 3/29/26
*/

#pragma once

#include "../ProfilerApp.h"

namespace Astral {

    void GlobalGraphView(const SceneMetricsStorage& sceneMetricsStorage);
    void AllocatorGraphsView(const SceneMetricsStorage& sceneMetricsStorage);
    void RegionGraphsView(const SceneMetricsStorage& sceneMetricsStorage);
    void ThreadGraphsView(const SceneMetricsStorage& sceneMetricsStorage);

}
