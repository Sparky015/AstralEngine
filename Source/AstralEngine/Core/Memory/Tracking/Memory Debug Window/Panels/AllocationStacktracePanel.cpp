/**
* @file AllocationStacktracePanel.cpp
* @author Andrew Fagan
* @date 2/16/25
*/

#include "AllocationStacktracePanel.h"

#include "imgui.h"

namespace Core {

    void AllocationStackTracePanel(const SceneMetricsStorage& storage, size_t dataPointAddress)
    {
        ImGui::Begin("Allocation Stacktrace");

        if (&dataPointAddress == nullptr)
        {
            ImGui::Text("No Data Point Selected");
        }
        else
        {
            ImGui::Text("Stacktrace Here");
        }

        ImGui::End();
    }
}
