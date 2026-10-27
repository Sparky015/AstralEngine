/**
* @file ViewConfigSelector.h
* @author Andrew Fagan 
* @date 10/26/26
*/

#pragma once

#include "Profiler/MemoryTracking/Serialization/SceneMetricsStorage.h"

namespace Astral {

    class ViewConfigSelector
    {
    public:

        /**
         * @brief Renders the View Config Selector ImGui panel
         * @param storage The scene metrics storage currently loaded
         */
        static void Show(const SceneMetricsStorage& storage);

    private:

        /**
         * @brief Renders the ImGui thread filter selector
         * @param storage The scene metrics storage currently loaded
         */
        static void ShowThreadFilter(const SceneMetricsStorage& storage);

        /**
         * @brief Renders the ImGui allocator filter selector
         * @param storage The scene metrics storage currently loaded
         */
        static void ShowAllocatorFilter(const SceneMetricsStorage& storage);

        /**
         * @brief Renders the ImGui memory region filter selector
         * @param storage The scene metrics storage currently loaded
         */
        static void ShowMemoryRegionFilter(const SceneMetricsStorage& storage);
    };

}
