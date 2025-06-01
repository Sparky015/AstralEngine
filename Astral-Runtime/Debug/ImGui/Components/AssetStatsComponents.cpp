/**
* @file AssetStatsComponents.cpp
* @author Andrew Fagan
* @date 4/14/25
*/

#include "AssetStatsComponents.h"

#include "Asset/AssetManager.h"
#include "Asset/Asset.h"
#include "Core/Engine.h"

#include "imgui.h"

namespace Debug {

    void ShowNumberOfAssetsLoaded()
    {
        const Astral::AssetRegistryStats& registryStats = Astral::Engine::Get().GetAssetManager().GetRegistry().GetAssetRegistryStats();
        ImGui::Text("Number of Assets Loaded: %u", registryStats.NumberOfLoadedAssets);
    }


    void ShowTotalNumberOfAssetsLoaded()
    {
        const Astral::AssetRegistryStats& registryStats = Astral::Engine::Get().GetAssetManager().GetRegistry().GetAssetRegistryStats();
        ImGui::Text("Total Loads Made: %llu", registryStats.NumberOfLoadsMade);
    }


    void ShowTotalNumberOfAssetsUnloaded()
    {
        const Astral::AssetRegistryStats& registryStats = Astral::Engine::Get().GetAssetManager().GetRegistry().GetAssetRegistryStats();
        ImGui::Text("Total Unloads Made: %llu", registryStats.NumberOfUnloadsMade);
    }

    void ShowNumberOfAssetsLoadedByType()
    {
        const Astral::AssetRegistryStats& registryStats = Astral::Engine::Get().GetAssetManager().GetRegistry().GetAssetRegistryStats();

        if (ImGui::TreeNode("Number of Loaded Assets By Type"))
        {
            for (auto [assetType, numberOfLoadedAssets] : registryStats.LoadedAssetsByType)
            {
                ImGui::Text("%s: %u", Astral::AssetTypeToString(assetType), numberOfLoadedAssets);
            }
            ImGui::TreePop();
        }
    }
}

