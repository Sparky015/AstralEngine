/**
* @file AllocationsListPanel.h
* @author Andrew Fagan
* @date 2/17/25
*/

#pragma once

#include "Core/Math/Math.h"
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

        /**
         * @brief Sets the allocation list plot limits filter in criteria
         * @param plotLimits The plot limits to filter in based on
         */
        static void SetListPlotLimitsFilterIn(Vec2 plotLimits);

        /**
         * @brief Sets the allocation list thread ID filter in criteria
         * @param hashedThreadID The thread ID to filter in based on
         */
        static void SetListThreadIDFilterIn(size_t hashedThreadID);

        /**
         * @brief Sets the allocation list memory region filter in criteria
         * @param memoryRegion The memory region to filter in based on
         */
        static void SetListRegionFilterIn(MemoryRegion memoryRegion);

        /**
         * @brief Sets the allocation list allocator type filter in criteria
         * @param allocatorType The allocator type to filter in based on
         */
        static void SetListAllocatorFilterIn(MemoryTrackerAllocatorType allocatorType);

    private:

        /**
         * @brief A custom list box imgui method that supports handling filtered out data
         * @param label The imgui label for the list box
         * @param current_item The current item selected index
         * @param getter A getter method to handle indexing into the user data
         * @param user_data The user data to list information with
         * @param items_count The number of items in the user data
         * @param height_in_items The height in items of the list box
         */
        bool ImGuiCustomListBox(const char* label, int* current_item, const char* (* getter)(void* user_data, int idx), void* user_data, int items_count, int height_in_items);

        /**
         * @brief Checks if an allocation should be filtered out with the current set filters
         * @return True if the allocation should be filtered out, false otherwise
         */
        static bool ShouldFilterAllocationOut(const SceneMetricsStorage& storage, int storageIndex);

        /**
         * @brief A getter for the custom imgui list box that supports filtering out indices of data
         * @param data The data to retrieve an item name from
         * @param idx The index to retrieve data from
         * @return The name of the item in the list at the given index
         */
        static const char* AllocationDataArrayGetter(void* data, int idx);

        int m_SelectedPointIndex = -1;
        int m_SortedSelectedPointIndex = -1;
        std::vector<std::pair<AllocationDataSerializeable, int>> m_SortedAllocationData = {}; // Container to cache sorted allocation data with .second containing a index mapping to the original allocation data order

        static Vec2 m_FilterInPlotLimits;
        static size_t m_FilterInThreadID;
        static MemoryRegion m_FilterInMemoryRegion;
        static MemoryTrackerAllocatorType m_FilterInAllocatorType;
    };

}
