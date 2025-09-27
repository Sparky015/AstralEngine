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

        static void InitDefaultMaterial();
        static void DestroyDefaultMaterial();

        static void LoadSceneAssets(const std::filesystem::path& sceneFilePath);
        static void SerializeScene(Scene& scene, const std::filesystem::path& filePath);


        static void SerializeEnvironmentSettings(const Scene& scene, aiMetadata& outMetaData);
        static void LoadEnvironmentSettings(Scene& scene, const aiMetadata& metaData);

        static void SerializeCameraSettings(const Camera& camera, aiMetadata& outMetaData);
        static void LoadCameraSettings(Camera& outCamera, const aiMetadata& metaData);

        struct Helpers
        {
            static void LoadAndBreakObjectIntoMuiltipleObjects(const std::filesystem::path& sceneFilePath, bool shouldSerializeObjects);
            static void ProcessSceneNode(const aiScene* scene, aiNode* node, const std::vector<Ref<Material>>& materials, const std::vector<Ref<Mesh>>& meshes, const
                                         std::unordered_map<std::string_view, aiLight*>& lightNameToLight);
        };

        static void SetDefaultMaterialShaderModel(ShaderModel shaderModel) { m_DefaultMaterialShaderModal = shaderModel; }
        static void SetDefaultMaterialTextureConvention(TextureConvention textureConvention) { m_DefaultMaterialTextureConvention = textureConvention; }
        static void SetDefaultMaterialBaseColor(TextureHandle texture) { m_DefaultMaterialBaseColor = texture; }
        static void SetDefaultMaterialNormals(TextureHandle texture) { m_DefaultMaterialNormals = texture; }
        static void SetDefaultMaterialRoughness(TextureHandle texture) { m_DefaultMaterialRoughness = texture; }
        static void SetDefaultMaterialMetallic(TextureHandle texture) { m_DefaultMaterialMetallic = texture; }
        static void SetDefaultMaterialEmission(TextureHandle texture) { m_DefaultMaterialEmission = texture; }
        static void SetDefaultCoordinateSystemOffset(Vec3 rotationOffset) { m_DefaultRotationOffset = rotationOffset; }
        static void SetDefaultIsNormalsDirectX(bool isNormalsDirectX) { m_DefaultIsNormalsDirectX = isNormalsDirectX; }
        static void SetDefaultScaleMultiplier(float scaleMultiplier) { m_ScaleMultiplier = scaleMultiplier; }
        static void SetDefaultFlipUVs(bool shouldFlipUVs) { m_DefaultFlipUVs = shouldFlipUVs; }

        static ShaderModel GetDefaultMaterialShaderModel() { return m_DefaultMaterialShaderModal; }
        static TextureConvention GetDefaultMaterialTextureConvention() { return m_DefaultMaterialTextureConvention; }
        static TextureHandle GetDefaultMaterialBaseColor() { return m_DefaultMaterialBaseColor; }
        static TextureHandle GetDefaultMaterialNormals() { return m_DefaultMaterialNormals; }
        static TextureHandle GetDefaultMaterialRoughness() { return m_DefaultMaterialRoughness; }
        static TextureHandle GetDefaultMaterialMetallic() { return m_DefaultMaterialMetallic; }
        static TextureHandle GetDefaultMaterialEmission() { return m_DefaultMaterialEmission; }
        static Vec3 GetDefaultCoordinateSystemOffset() { return m_DefaultRotationOffset; }
        static bool GetDefaultIsNormalsDirectX() { return m_DefaultIsNormalsDirectX; }
        static bool GetDefaultFlipUVs() { return m_DefaultFlipUVs; }
        static float GetDefaultScaleMultiplier() { return m_ScaleMultiplier; }


    private:

        static Ref<Material> LoadPBRMaterial(aiMaterial* material, std::unordered_map<std::filesystem::path, Ref<Texture>>& externalTextures, std::filesystem::
                                          path& sceneDir);
        static Ref<Material> LoadORMPackedPBRMaterial(aiMaterial* material, std::unordered_map<std::filesystem::path, Ref<Texture>>& externalTextures, std::filesystem::
                                          path& sceneDir);
        static Ref<Texture> GetTexture(aiString& filePath, std::unordered_map<std::filesystem::path, Ref<Texture>>& externalTextures, std::filesystem::
                                          path& sceneDir);


        static ShaderModel m_DefaultMaterialShaderModal;
        static TextureConvention m_DefaultMaterialTextureConvention;

        static Vec3 m_DefaultRotationOffset;

        static TextureHandle m_DefaultMaterialBaseColor;
        static TextureHandle m_DefaultMaterialNormals;
        static TextureHandle m_DefaultMaterialRoughness;
        static TextureHandle m_DefaultMaterialMetallic;
        static TextureHandle m_DefaultMaterialEmission;

        static bool m_DefaultIsNormalsDirectX;
        static bool m_DefaultFlipUVs;
        static float m_ScaleMultiplier;
    };


}
