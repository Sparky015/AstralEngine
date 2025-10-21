/**
* @file MenuBarComponent.cpp
* @author Andrew Fagan
* @date 6/21/2025
*/

#include "MenuBarComponent.h"

#include "ExternalSceneBreakdownConfigComponent.h"
#include "Asset/AssetManager.h"
#include "Core/Engine.h"
#include "Debug/Utilities/Loggers.h"
#include "nfd.hpp"

#include "imgui.h"
#include "Scenes/SceneManager.h"
#include "Asset/Loaders/SceneLoader.h"

namespace Astral {

    bool MenuBarComponent::m_ShowModal = false;
    bool MenuBarComponent::m_LoadExternalScene = false;
    bool MenuBarComponent::m_ShouldSerializeObjects = false;
    std::string MenuBarComponent::m_ExternalSceneFilePath = "";

    void MenuBarComponent::Show()
    {
        PROFILE_SCOPE("MenuBarComponent::Show")

        m_ShowModal = false;

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
                    AE_LOG("Open File Dialog Canceled")
                }
                else if (result == NFD_ERROR)
                {
                    AE_WARN("NFD Error: " << NFD_GetError())
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
                        AE_LOG("Save File Dialog Canceled");
                    }
                    else if (result == NFD_ERROR)
                    {
                        AE_WARN("NFD Error: " << NFD_GetError())
                    }
                }
            }

            if (ImGui::MenuItem("Load and Condition 3D File with Multiple Objects"))
            {
                nfdu8char_t* outPath;
                nfdu8filteritem_t filters[1] = { { "3D Object", "fbx,gltf,obj" }};
                nfdopendialogu8args_t args = {0};
                args.filterList = filters;
                args.filterCount = 1;
                // args.defaultPath = registry.GetAssetDirectoryPath().string().c_str();

                nfdresult_t result = NFD_OpenDialogU8_With(&outPath, &args);

                if (result == NFD_OKAY)
                {
                    m_ExternalSceneFilePath = outPath;
                    NFD_FreePathU8(outPath);
                    m_ShowModal = true;
                    m_ShouldSerializeObjects = true;
                }
                else if (result == NFD_CANCEL)
                {
                    AE_LOG("Open File Dialog Canceled")
                }
                else if (result == NFD_ERROR)
                {
                    AE_WARN("NFD Error: " << NFD_GetError())
                }
            }

            if (ImGui::MenuItem("View 3D File with Multiple Objects"))
            {
                nfdu8char_t* outPath;
                nfdu8filteritem_t filters[1] = { { "3D Object", "fbx,gltf,obj" }};
                nfdopendialogu8args_t args = {0};
                args.filterList = filters;
                args.filterCount = 1;
                // args.defaultPath = registry.GetAssetDirectoryPath().string().c_str();

                nfdresult_t result = NFD_OpenDialogU8_With(&outPath, &args);

                if (result == NFD_OKAY)
                {
                    m_ExternalSceneFilePath = outPath;
                    NFD_FreePathU8(outPath);
                    m_ShowModal = true;
                    m_ShouldSerializeObjects = false;
                }
                else if (result == NFD_CANCEL)
                {
                    AE_LOG("Open File Dialog Canceled")
                }
                else if (result == NFD_ERROR)
                {
                    AE_WARN("NFD Error: " << NFD_GetError())
                }
            }

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();



        if (m_ShowModal)
        {
            ImGui::OpenPopup("3D Object Import##ExternalSceneBreakdownConfigComponent");
        }

        ExternalSceneBreakdownConfigComponent::Show();

        if (m_LoadExternalScene)
        {
            SceneLoader::Helpers::LoadAndBreakObjectIntoMuiltipleObjects(m_ExternalSceneFilePath, m_ShouldSerializeObjects);
            m_ExternalSceneFilePath = "";
            m_LoadExternalScene = false;
        }

    }

}
