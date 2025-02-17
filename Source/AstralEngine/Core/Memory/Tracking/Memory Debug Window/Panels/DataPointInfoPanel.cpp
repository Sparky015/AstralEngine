/**
* @file DataPointInfoPanel.cpp
* @author Andrew Fagan
* @date 2/16/25
*/

#include "DataPointInfoPanel.h"

#include "imgui.h"

namespace Core {

    size_t* GetSelectedDataPointAddress()
    {
        return nullptr;
    }

    void ShowDataPointInfoPanel(const SceneMetricsStorage& storage, size_t* selectedPointAddress)
    {
        ImGui::Begin("Data Point Info");

        if (selectedPointAddress == nullptr)
        {
            ImGui::Text("No Data Point Selected");
        }
        else
        {
            ImGui::Text("Point Data Here");
        }

        ImGui::End();
    }


}
