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


namespace Astral {

    class SceneLoader
    {
    public:

        static void LoadSceneAssets(const std::filesystem::path& sceneFilePath);
        static void SerializeScene(Scene& scene, const std::filesystem::path& filePath);


        struct Helpers
        {
            static void LoadAndBreakObjectIntoMuiltipleObjects(const std::filesystem::path& sceneFilePath, bool shouldSerializeObjects);
            static void ProcessSceneNode(const aiScene* scene, const aiNode* node, const std::vector<Ref<Material>>& materials, const std::vector<Ref<Mesh>>& meshes);
        };

    private:

        static Ref<Material> LoadMaterial(aiMaterial* material, std::unordered_map<std::filesystem::path, Ref<Texture>>& externalTextures, std::filesystem::
                                          path& sceneDir);
        static Ref<Texture> GetTexture(aiString& filePath, std::unordered_map<std::filesystem::path, Ref<Texture>>& externalTextures, std::filesystem::
                                          path& sceneDir);
    };





}
