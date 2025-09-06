/**
* @file AllocationStacktracePanel.cpp
* @author Andrew Fagan
* @date 2/16/25
*/

#include "AllocationStacktracePanel.h"

#include "imgui.h"

namespace Astral {

    void AllocationStackTracePanel(const SceneMetricsStorage& storage, size_t dataPointAddress)
    {
        ImGui::Begin("Allocation Stacktrace");

        if (dataPointAddress == std::numeric_limits<size_t>::max())
        {
            ImGui::Text("No Data Point Selected");
        }
        else
        {
            ImGui::Text("%s", storage.GetStacktraceStorage()[dataPointAddress].c_str());
        }

        ImGui::End();
    }
}
