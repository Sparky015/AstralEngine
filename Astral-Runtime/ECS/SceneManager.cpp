//
// Created by Andrew Fagan on 11/27/24.
//

#include "SceneManager.h"

#include "ECS.h"
#include "Debug/Instrumentation/ScopeProfiler.h"
#include "Debug/Utilities/Loggers.h"
#include "Systems/RenderingSystem.h"

namespace Astral {

    SceneManager::SceneManager() :
        m_ActiveScene(nullptr),
        m_ActiveSceneExists(true)
    {
        TRACE("Constructing Scene Manager!")
    }


    SceneManager::~SceneManager()
    {
        TRACE("Destroying Scene Manager!")
    }


    void SceneManager::Init()
    {
        PROFILE_SCOPE("Scene Manager Initialization");
        TRACE("Initializing Scene Manager!")
        Scene scene{};
        scene.ECS = ECS();
        scene.ECS.Init();

        m_Scenes["Engine:\\BaseScene.fbx"] = CreateScopedPtr<Scene>(scene);
        m_ActiveScene = m_Scenes["Engine:\\BaseScene.fbx"].get();;
        m_ActiveSceneExists = true;

        m_UpdateListener.StartListening();
    }


    void SceneManager::Update()
    {
        PROFILE_SCOPE("Scene ECS Systems");
        RenderingSystem::RenderEntities();
    }


    void SceneManager::Shutdown()
    {
        PROFILE_SCOPE("Scene Manager Shutdown");
        TRACE("Shutting down Scene Manager!")
        m_UpdateListener.StopListening();
        GetECS().Shutdown();
        m_Scenes.clear();
    }


    void SceneManager::CreateEmptyScene()
    {
        Scene scene{};
        scene.ECS = ECS();
        scene.ECS.Init();

        m_Scenes[""] = CreateScopedPtr<Scene>(scene);
        m_ActiveScene = m_Scenes[""].get();
        m_ActiveSceneExists = true;
    }


    void SceneManager::LoadScene(const std::filesystem::path& filePath)
    {
        // if (m_Scenes.contains(filePath)) // TODO: Add way to switch to different scene without loading it from a file
        // {
        //     m_ActiveScene = m_Scenes[filePath].get();
        //     m_ActiveSceneExists = true;
        //     return;
        // }

        Astral::AssetRegistry& registry = Astral::Engine::Get().GetAssetManager().GetRegistry();
        Scene scene{};
        scene.ECS = ECS();
        scene.ECS.Init();

        m_Scenes[filePath] = CreateScopedPtr<Scene>(scene);
        m_ActiveScene = m_Scenes[filePath].get();
        m_ActiveSceneExists = true;

        registry.LoadScene(filePath);
    }


    void SceneManager::SaveActiveScene(const std::string& filePath)
    {
        Astral::AssetRegistry& registry = Astral::Engine::Get().GetAssetManager().GetRegistry();
        registry.SerializeScene(*m_ActiveScene, filePath);
    }


    void SceneManager::UnloadScene()
    {

    }

} // ECS