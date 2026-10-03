//
// Created by Andrew Fagan on 11/27/24.
//

#pragma once

#include "Core/SystemManager.h"

#include "../ECS/ECS.h"
#include "Scenes/Scene.h"

namespace Astral {

    class SceneManager : public SystemManager
    {
    public:
        SceneManager();
        ~SceneManager() override;

        void Init() override;
        void Update() override;
        void Shutdown() override;

        void CreateEmptyScene();
        void LoadScene(const std::filesystem::path& filePath);
        void SaveActiveScene(const std::string& filePath);
        void UnloadScene();
        bool IsSceneActive() { return m_ActiveSceneExists; }

        Scene& GetActiveScene()
        {
            ASSERT(m_ActiveSceneExists, "Can't get active scene! No scene is currently set active!")
            return *m_ActiveScene;
        }

        ECS& GetECS()
        {
            ASSERT(m_ActiveSceneExists, "Can't get ECS! No scene is currently set active!")
            return m_ActiveScene->ECS;
        }

        SceneManager(const SceneManager&) = delete;
        SceneManager& operator=(const SceneManager&) = delete;
        SceneManager(SceneManager&&) = delete;
        SceneManager& operator=(SceneManager&&) = delete;

    private:

        std::unordered_map<std::filesystem::path, ScopedPtr<Scene>> m_Scenes;
        Scene* m_ActiveScene;
        bool m_ActiveSceneExists;
    };

} // ECS
