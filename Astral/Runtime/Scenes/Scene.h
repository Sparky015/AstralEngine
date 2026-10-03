/**
* @file Scene.h
* @author Andrew Fagan
* @date 6/20/2025
*/


#pragma once

#include "ECS/ECS.h"
#include "Renderer/Cameras/Camera.h"
#include "Renderer/Common/EnvironmentMap.h"

namespace Astral {

    using SceneResourceID = uint16;
    static constexpr SceneResourceID NullSceneResourceID = -1;

    struct SceneResource
    {
        uint16 ReferenceCount;
        SceneResourceID SceneResourceID;
    };

    struct Scene
    {
        Scene() = default;
        ~Scene() = default;

        std::unordered_map<std::string, SceneResource> ExternalResourceFiles;
        Camera PrimaryCamera = Camera(CameraType::PERSPECTIVE, 1.0, 800.0f);
        ECS ECS;
        Ref<EnvironmentMap> EnvironmentMap;
        float AmbientLightConstant = 1.0f;
        float Exposure = 0.0f;
        float EnvironmentMapBlur = 0.0f;

        void IncrementResourceRef(const std::string& filePath);
        void IncrementResourceRef(const std::filesystem::path& filePath);
        void IncrementResourceRef(const char* filePath);
        void DecrementResourceRef(const std::string& filePath);
        void DecrementResourceRef(const std::filesystem::path& filePath);
        void DecrementResourceRef(const char* filePath);

        SceneResourceID NextSceneResourceID = 0;

        Scene(const Scene&) = default;
        Scene& operator=(const Scene&) = delete;
        Scene(Scene&&) noexcept = delete;
        Scene& operator=(Scene&&) noexcept = delete;
    };


    inline void Scene::IncrementResourceRef(const std::string& filePath)
    {
        ASSERT(filePath != "", "Can't increment the scene resource ref when file path is empty!")
        if (!ExternalResourceFiles.contains(filePath))
        {
            ExternalResourceFiles[filePath].SceneResourceID = NextSceneResourceID;
            NextSceneResourceID++;
        }

        ExternalResourceFiles[filePath].ReferenceCount++;
    }

    inline void Scene::IncrementResourceRef(const std::filesystem::path& filePath)
    {
        IncrementResourceRef(filePath.string());
    }

    inline void Scene::IncrementResourceRef(const char* filePath)
    {
        IncrementResourceRef(std::string(filePath));
    }

    inline void Scene::DecrementResourceRef(const std::string& filePath)
    {
        if (filePath == "") { return; }
        // ASSERT(ExternalResourceFiles.contains(filePath), "Trying to decrement ref of scene resource that was never added to the scene!") // TODO: Add back if this resource ref set up is being used
        ExternalResourceFiles[filePath].ReferenceCount--;
    }

    inline void Scene::DecrementResourceRef(const std::filesystem::path& filePath)
    {
        DecrementResourceRef(filePath.string());
    }

    inline void Scene::DecrementResourceRef(const char* filePath)
    {
        DecrementResourceRef(std::string(filePath));
    }

}
