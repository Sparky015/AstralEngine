/**
* @file Shader.h
* @author Andrew Fagan
* @date 12/4/2024
*/

#pragma once

#include "Asset/Asset.h"
#include "ShaderSource.h"
#include "Core/Math/Math.h"
#include "Renderer/RHI/Common/GraphicsSmartPointers.h"

namespace Astral {

    enum class ShaderResourceType
    {
        COMBINED_SAMPLED_IMAGE,
        SEPARATE_IMAGE,
        SEPARATE_SAMPLER,
        STORAGE_IMAGE,
        STORAGE_BUFFER,
        UNIFORM_BUFFER
    };

    struct ShaderResourceBindSlot
    {
        uint32 DescriptorSet;
        uint32 BindSlot;
        ShaderResourceType ResourceType;
        std::string Name;
    };

    struct ShaderResourceCounts
    {
        uint32 CombinedSampledImagesCount;
        uint32 SeparateImagesCount;
        uint32 SeparateSamplersCount;
        uint32 StorageImages;
        uint32 StorageBuffers;
        uint32 UniformBuffers;
        uint32 PushConstants;
        uint32 StageInputs;
        uint32 StageOutputs;
    };

    struct ShaderReflectionInfo
    {
        ShaderType ShaderType = ShaderType::NONE;
        Vec3 WorkgroupDimensions = Vec3{-1}; // All components will be -1 if shader type is not a compute shader
        std::vector<ShaderResourceBindSlot> DeclaredResources = {};
        ShaderResourceCounts ShaderResourceCounts = {};
        std::string ShaderFileName = "";
    };

    /**
     * @brief Defines a RHI shader interface
     */
    class Shader : public Asset
    {
    public:
        virtual ~Shader() = default;

        /**
         * @brief Gets the shader reflection information for the shader
         * @return The shader reflection information for the shader
         */
        virtual const ShaderReflectionInfo& GetShaderReflectionInfo() = 0;

        /**
         * @brief Gets the native shader object handle
         * @return The native shader object handle
         */
        virtual void* GetNativeHandle() = 0;

        /**
         * @brief Compiles a shader and creates a shader object from the given shader source
         * @param shaderSource The shader source to create the shader object with
         * @return A shader object for the given shader source
         */
        static GraphicsRef<Shader> CreateShader(const ShaderSource& shaderSource);

        /**
         * @brief Gets the type of asset this Asset object is
         * @return The type of asset this Asset object is
         */
        AssetType GetAssetType() override { return GetStaticAssetType(); }

        /**
         * @brief Gets the type of asset this class is
         * @return The type of asset this class is
         */
        static AssetType GetStaticAssetType() { return AssetType::Shader; }
    };

    using ShaderHandle = GraphicsRef<Shader>;

} // Renderer