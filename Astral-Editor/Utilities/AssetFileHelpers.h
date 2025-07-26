/**
* @file AssetFileHelpers.h
* @author Andrew Fagan
* @date 7/23/25
*/

#pragma once

#include "Asset/AssetManager.h"
#include "Core/Engine.h"

#include "imgui.h"


namespace Astral {

    void GetAssetFilePath(AssetRegistry& registry, AssetID materialAssetID, char* inputBuffer, int inputBufferSize);
    void SelectFileFromDialog(std::string& outFilePath, std::string_view filterName, std::string_view filterSpec);
    void SelectFileFromDialog(std::filesystem::path& outFilePath, std::string_view filterName, std::string_view filterSpec);

    template <typename AssetType>
        requires std::is_base_of_v<Asset, AssetType>
    void AssetSelectionComponent(std::string_view selectionName, Ref<AssetType>& outAsset, std::string_view filterName, std::string_view filterSpec)
    {
        AssetRegistry& registry = Engine::Get().GetAssetManager().GetRegistry();

        AssetID textureAssetID = NullAssetID;
        if (outAsset)
        {
            textureAssetID = outAsset->GetAssetID();
        }
        static char inputBuffer[200];
        GetAssetFilePath(registry, textureAssetID, inputBuffer, sizeof(inputBuffer));


        ImGui::PushID(selectionName.data());
        ImGui::PushID(textureAssetID);


        ImGui::Text(selectionName.data());
        ImGui::SameLine();
        if (ImGui::InputText("##AssetSelectionInputText", inputBuffer, sizeof(inputBuffer), ImGuiInputTextFlags_EnterReturnsTrue))
        {
            Ref<AssetType> newAsset = registry.CreateAsset<AssetType>(inputBuffer);
            if (newAsset) { outAsset = newAsset; }
        }
        ImGui::SameLine();
        if (ImGui::Button("...##AssetSelectionButton"))
        {
            std::filesystem::path selectedFilePath;
            SelectFileFromDialog(selectedFilePath, filterName, filterSpec);
            registry.GetRelativePath(selectedFilePath);

            Ref<AssetType> newAsset = registry.CreateAsset<AssetType>(selectedFilePath.string());
            if (newAsset) { outAsset = newAsset; }
        }


        ImGui::PopID();
        ImGui::PopID();
    }


}
