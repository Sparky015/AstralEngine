/**
* @file ToolBar.cpp
* @author Andrew Fagan
* @date 6/21/2025
*/

#include "MenuBar.h"

#include "Asset/AssetManager.h"
#include "Core/Engine.h"
#include "Debug/Utilities/Loggers.h"
#include "nfd.hpp"

#include "imgui.h"
#include "ECS/SceneManager.h"

namespace Astral {

    void MenuBarComponent()
    {
        ImGui::BeginMainMenuBar();

        Astral::SceneManager& sceneManager = Astral::Engine::Get().GetSceneManager();
        Astral::AssetRegistry& registry = Astral::Engine::Get().GetAssetManager().GetRegistry();


        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Create Empty Scene"))
            {
                sceneManager.CreateEmptyScene();
            }

            if (ImGui::MenuItem("Load Scene"))
            {

                // registry.LoadScene("Scenes/Sponza/sponza.obj");

                // registry.LoadScene("Scenes/San_Miguel/san-miguel-low-poly.obj");

                // registry.LoadScene("Scenes/Amazon_Lumberyard_Bistro/Interior/interior.obj");
                // registry.LoadScene("Scenes/Amazon_Lumberyard_Bistro/Exterior/exterior.obj");

                // registry.LoadScene("Scenes/Toolbox/Toolbox.fbx");

                nfdu8char_t* outPath;
                nfdu8filteritem_t filters[1] = { { "Astral Scene", "aescene" }};
                nfdopendialogu8args_t args = {0};
                args.filterList = filters;
                args.filterCount = 1;
                // args.defaultPath = registry.GetAssetDirectoryPath().string().c_str();

                nfdresult_t result = NFD_OpenDialogU8_With(&outPath, &args);

                if (result == NFD_OKAY)
                {
                    std::string outFilePath = std::string(outPath);
                    NFD_FreePathU8(outPath);
                    sceneManager.LoadScene(outFilePath);
                }
                else if (result == NFD_CANCEL)
                {
                    LOG("Open File Dialog Canceled")
                }
                else if (result == NFD_ERROR)
                {
                    WARN("NFD Error: " << NFD_GetError())
                }

            }

            if (ImGui::MenuItem("Save Scene"))
            {
                if (sceneManager.IsSceneActive())
                {
                    nfdu8char_t* outPath;
                    nfdu8filteritem_t filters[1] = { { "Astral Scene", "aescene" }};
                    nfdsavedialogu8args_t args = {0};
                    args.filterList = filters;
                    args.filterCount = 1;
                    // args.defaultPath = registry.GetAssetDirectoryPath().string().c_str();

                    // Use the Save dialog instead of Open
                    nfdresult_t result = NFD_SaveDialogU8_With(&outPath, &args);

                    if (result == NFD_OKAY)
                    {
                        std::string outFilePath = std::string(outPath);
                        NFD_FreePathU8(outPath);
                        sceneManager.SaveActiveScene(outFilePath);
                    }
                    else if (result == NFD_CANCEL)
                    {
                        LOG("Save File Dialog Canceled");
                    }
                    else if (result == NFD_ERROR)
                    {
                        WARN("NFD Error: " << NFD_GetError())
                    }
                }
            }


            ImGui::EndMenu();
        }



        ImGui::EndMainMenuBar();
    }

}
