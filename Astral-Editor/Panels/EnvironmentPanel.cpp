/**
* @file EnvironmentPanel.cpp
* @author Andrew Fagan
* @date 7/23/25
*/

#include "EnvironmentPanel.h"

#include "Core/Engine.h"
#include "ECS/SceneManager.h"
#include "Scenes/Scene.h"
#include "Utilities/AssetFileHelpers.h"
#include "Asset/AssetManager.h"

#include "imgui.h"

namespace Astral {

    void EnvironmentPanel()
    {
        Scene& scene = Engine::Get().GetSceneManager().GetActiveScene();

        ImGui::Begin("Environment##EditorEnvironmentPanel");

        AssetRegistry& registry = Engine::Get().GetAssetManager().GetRegistry();

        AssetSelectionComponent("Environment Map: ", scene.EnvironmentMap, "Environment Map", "hdr");

        ImGui::Text("Ambient Light: ");
        ImGui::SameLine();
        ImGui::InputFloat("##AmbientLightInput", &scene.AmbientLightConstant, .05, .1);

        ImGui::Text("Exposure: ");
        ImGui::SameLine();
        ImGui::InputFloat("##ExposureInput", &scene.Exposure, .05, .1);

        ImGui::End();
    }

}

