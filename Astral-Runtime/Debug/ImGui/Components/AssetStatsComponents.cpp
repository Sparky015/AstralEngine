/**
* @file AssetStatsComponents.cpp
* @author Andrew Fagan
* @date 4/14/25
*/

#include "AssetStatsComponents.h"

#include "imgui.h"
#include "Asset/AssetManager.h"

namespace Debug {

    void ShowNumberOfAssetsLoaded()
    {
        const Astral::AssetRegistryStats& registryStats =  Astral::g_AssetManager.GetRegistry().GetAssetRegistryStats();
        ImGui::Text("Number of Assets Loaded: %u", registryStats.NumberOfLoadedAssets);
    }


    void ShowTotalNumberOfAssetsLoaded()
    {
        const Astral::AssetRegistryStats& registryStats =  Astral::g_AssetManager.GetRegistry().GetAssetRegistryStats();
        ImGui::Text("Total Loads Made: %llu", registryStats.NumberOfLoadsMade);
    }


    void ShowTotalNumberOfAssetsUnloaded()
    {
        const Astral::AssetRegistryStats& registryStats =  Astral::g_AssetManager.GetRegistry().GetAssetRegistryStats();
        ImGui::Text("Total Unloads Made: %llu", registryStats.NumberOfUnloadsMade);
    }

}

