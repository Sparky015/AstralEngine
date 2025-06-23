/**
* @file SceneLoader.h
* @author Andrew Fagan
* @date 6/20/2025
*/


#pragma once

#include <filesystem>

#include "assimp/scene.h"
#include "Core/SmartPointers.h"
#include "Renderer/Common/Material.h"
#include "Renderer/Common/Mesh.h"
#include "Scenes/Scene.h"


namespace Astral::SceneLoader {

    void LoadSceneAssets(const std::filesystem::path& sceneFilePath);
    void SerializeScene(Scene& scene, const std::filesystem::path& filePath);

    namespace Helpers {

        void ProcessSceneNode(const aiScene* scene, const aiNode* node, const std::vector<Ref<Material>>& materials, const std::vector<Ref<Mesh>>& meshes);

    }

}
