/**
* @file GraphViews.h
* @author Andrew Fagan
* @date 3/29/26
*/

#pragma once

#include "Profiler/App/Panels/AllocationsListPanel.h"

namespace Astral {

    class GraphView
    {
    public:

        void ShowGlobalGraphView(const SceneMetricsStorage& sceneMetricsStorage);
        void ShowAllocatorGraphsView(const SceneMetricsStorage& sceneMetricsStorage);
        void ShowRegionGraphsView(const SceneMetricsStorage& sceneMetricsStorage);
        void ShowThreadGraphsView(const SceneMetricsStorage& sceneMetricsStorage);

    private:

        AllocationsListPanel m_AllocationListsPanel = {};
    };



}
