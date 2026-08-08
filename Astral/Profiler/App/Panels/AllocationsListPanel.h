/**
* @file AllocationsListPanel.h
* @author Andrew Fagan
* @date 2/17/25
*/

#pragma once

#include "Profiler/MemoryTracking/Serialization/SceneMetricsStorage.h"

namespace Astral {

    class AllocationsListPanel
    {
    public:

        /**
         * @brief Renders the Allocations List ImGui panel
         * @param storage The scene metrics to use for the allocations list
         */
        void Show(const SceneMetricsStorage& storage);

        /**
         * @brief Gets the selected point index in the allocations list
         * @return The selected point index in the allocations list or -1 if no point was selected
         */
        int GetSelectedPoint();

    private:

        int m_SelectedPointIndex = -1;
    };

}
