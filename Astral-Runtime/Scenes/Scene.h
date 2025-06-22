/**
* @file Scene.h
* @author Andrew Fagan
* @date 6/20/2025
*/


#pragma once

#include "ECS/ECS.h"

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
        ECS SceneECS;

        void IncrementResourceRef(const std::string& filePath);
        void IncrementResourceRef(const std::filesystem::path& filePath);
        void DecrementResourceRef(const std::string& filePath);
        void DecrementResourceRef(const std::filesystem::path& filePath);

        SceneResourceID NextSceneResourceID = 0;
    };


    inline void Scene::IncrementResourceRef(const std::string& filePath)
    {
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

    inline void Scene::DecrementResourceRef(const std::string& filePath)
    {
        ASSERT(ExternalResourceFiles.contains(filePath), "Trying to decrement ref of scene resource that was never added to the scene!")
        ExternalResourceFiles[filePath].ReferenceCount--;
    }

    inline void Scene::DecrementResourceRef(const std::filesystem::path& filePath)
    {
        DecrementResourceRef(filePath.string());
    }

}
