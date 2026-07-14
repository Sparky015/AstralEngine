/**
* @file GraphViews.cpp
* @author Andrew Fagan
* @date 3/29/26
*/

#include "GraphViews.h"

#include "../Panels/AllocationStacktracePanel.h"
#include "../Panels/AllocationsListPanel.h"
#include "../Panels/DataPointInfoPanel.h"
#include "../Panels/GraphSelectorPanel.h"
#include "../Panels/MemoryGraphPanel.h"

namespace Astral {

    void GlobalGraphView(const SceneMetricsStorage& sceneMetricsStorage)
    {
        // Settings tab content
        MemoryGraphSelection graphSelection = ShowGraphSelectorPanel();
        size_t selectedDataPoint = AllocationsListPanel(sceneMetricsStorage);

        ShowMemoryGraphPanel(sceneMetricsStorage, MemoryMetricsDataScope::GLOBAL, graphSelection, selectedDataPoint);
        ShowDataPointInfoPanel(sceneMetricsStorage, selectedDataPoint);
        AllocationStackTracePanel(sceneMetricsStorage, selectedDataPoint);
    }


    void AllocatorGraphsView(const SceneMetricsStorage& sceneMetricsStorage)
    {
        MemoryGraphSelection graphSelection = ShowGraphSelectorPanel();
        size_t selectedDataPoint = AllocationsListPanel(sceneMetricsStorage);

        ShowMemoryGraphPanel(sceneMetricsStorage, MemoryMetricsDataScope::ALLOCATOR, graphSelection, selectedDataPoint);
        ShowDataPointInfoPanel(sceneMetricsStorage, selectedDataPoint);
        AllocationStackTracePanel(sceneMetricsStorage, selectedDataPoint);
    }


    void RegionGraphsView(const SceneMetricsStorage& sceneMetricsStorage)
    {
        MemoryGraphSelection graphSelection = ShowGraphSelectorPanel();
        size_t selectedDataPoint = AllocationsListPanel(sceneMetricsStorage);

        ShowMemoryGraphPanel(sceneMetricsStorage, MemoryMetricsDataScope::REGION, graphSelection, selectedDataPoint);
        ShowDataPointInfoPanel(sceneMetricsStorage, selectedDataPoint);
        AllocationStackTracePanel(sceneMetricsStorage, selectedDataPoint);

    }

    void ThreadGraphsView(const SceneMetricsStorage& sceneMetricsStorage)
    {
        MemoryGraphSelection graphSelection = ShowGraphSelectorPanel();
        size_t selectedDataPoint = AllocationsListPanel(sceneMetricsStorage);

        ShowMemoryGraphPanel(sceneMetricsStorage, MemoryMetricsDataScope::THREAD, graphSelection, selectedDataPoint);
        ShowDataPointInfoPanel(sceneMetricsStorage, selectedDataPoint);
        AllocationStackTracePanel(sceneMetricsStorage, selectedDataPoint);

    }

}
