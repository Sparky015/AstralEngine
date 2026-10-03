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

    void GraphView::ShowGlobalGraphView(const SceneMetricsStorage& sceneMetricsStorage)
    {
        // Settings tab content
        MemoryGraphSelection graphSelection = ShowGraphSelectorPanel();
        m_AllocationListsPanel.Show(sceneMetricsStorage);
        size_t selectedDataPoint = m_AllocationListsPanel.GetSelectedPoint();

        ShowMemoryGraphPanel(sceneMetricsStorage, MemoryMetricsDataScope::GLOBAL, graphSelection, selectedDataPoint);
        ShowDataPointInfoPanel(sceneMetricsStorage, selectedDataPoint);
        AllocationStackTracePanel(sceneMetricsStorage, selectedDataPoint);
    }


    void GraphView::ShowAllocatorGraphsView(const SceneMetricsStorage& sceneMetricsStorage)
    {
        MemoryGraphSelection graphSelection = ShowGraphSelectorPanel();
        m_AllocationListsPanel.Show(sceneMetricsStorage);
        size_t selectedDataPoint = m_AllocationListsPanel.GetSelectedPoint();

        ShowMemoryGraphPanel(sceneMetricsStorage, MemoryMetricsDataScope::ALLOCATOR, graphSelection, selectedDataPoint);
        ShowDataPointInfoPanel(sceneMetricsStorage, selectedDataPoint);
        AllocationStackTracePanel(sceneMetricsStorage, selectedDataPoint);
    }


    void GraphView::ShowRegionGraphsView(const SceneMetricsStorage& sceneMetricsStorage)
    {
        MemoryGraphSelection graphSelection = ShowGraphSelectorPanel();
        m_AllocationListsPanel.Show(sceneMetricsStorage);
        size_t selectedDataPoint = m_AllocationListsPanel.GetSelectedPoint();

        ShowMemoryGraphPanel(sceneMetricsStorage, MemoryMetricsDataScope::REGION, graphSelection, selectedDataPoint);
        ShowDataPointInfoPanel(sceneMetricsStorage, selectedDataPoint);
        AllocationStackTracePanel(sceneMetricsStorage, selectedDataPoint);

    }

    void GraphView::ShowThreadGraphsView(const SceneMetricsStorage& sceneMetricsStorage)
    {
        MemoryGraphSelection graphSelection = ShowGraphSelectorPanel();
        m_AllocationListsPanel.Show(sceneMetricsStorage);
        size_t selectedDataPoint = m_AllocationListsPanel.GetSelectedPoint();

        ShowMemoryGraphPanel(sceneMetricsStorage, MemoryMetricsDataScope::THREAD, graphSelection, selectedDataPoint);
        ShowDataPointInfoPanel(sceneMetricsStorage, selectedDataPoint);
        AllocationStackTracePanel(sceneMetricsStorage, selectedDataPoint);

    }

}
