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

            AssetID environmentMapAssetID = NullAssetID;
            if (scene.EnvironmentMap)
            {
                environmentMapAssetID = scene.EnvironmentMap->GetAssetID();
            }
            static char environmentMapInputBuffer[200];
            GetAssetFilePath(registry, environmentMapAssetID, environmentMapInputBuffer, sizeof(environmentMapInputBuffer));


            ImGui::Text("Environment Map: ");
            ImGui::SameLine();
            if (ImGui::InputText("##EnvironmentCubemapFilePath", environmentMapInputBuffer, sizeof(environmentMapInputBuffer), ImGuiInputTextFlags_EnterReturnsTrue))
            {
                Ref<Texture> newEnvironmentMap = registry.CreateAsset<Texture>(environmentMapInputBuffer);

                if (newEnvironmentMap != nullptr)
                {
                    if (scene.EnvironmentMap) { scene.DecrementResourceRef(registry.GetFilePathFromAssetID(scene.EnvironmentMap->GetAssetID())); }
                    scene.IncrementResourceRef(registry.GetFilePathFromAssetID(newEnvironmentMap->GetAssetID()));
                    scene.EnvironmentMap = newEnvironmentMap;
                }
            }
            ImGui::SameLine();
            if (ImGui::Button("...##SelectEnvironmentMapFilePath"))
            {
                std::filesystem::path selectedFilePath;
                SelectFileFromDialog(selectedFilePath, "Environment Map", "hdr");
                registry.GetRelativePath(selectedFilePath);
                Ref<Texture> newEnvironmentMap = registry.CreateAsset<Texture>(selectedFilePath);

                if (newEnvironmentMap != nullptr)
                {
                    if (scene.EnvironmentMap) { scene.DecrementResourceRef(registry.GetFilePathFromAssetID(scene.EnvironmentMap->GetAssetID())); }
                    scene.IncrementResourceRef(registry.GetFilePathFromAssetID(newEnvironmentMap->GetAssetID()));
                    scene.EnvironmentMap = newEnvironmentMap;
                }
            }

        ImGui::End();
    }

}

