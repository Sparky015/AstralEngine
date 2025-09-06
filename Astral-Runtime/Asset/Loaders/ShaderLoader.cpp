/**
* @file ShaderLoader.cpp
* @author Andrew Fagan
* @date 6/1/25
*/

#include "ShaderLoader.h"

#include "Debug/Instrumentation/ScopeProfiler.h"
#include "Renderer/RHI/RendererAPI.h"
#include "Renderer/RHI/Resources/Shader.h"

namespace Astral {

    Ref<Asset> ShaderLoader::LoadAsset(const std::filesystem::path& filePath)
    {
        PROFILE_SCOPE("ShaderLoader::LoadAsset")
        ShaderSource shaderSource = ShaderSource(filePath);
        ShaderHandle shaderHandle = Shader::CreateShader(shaderSource);
        RendererAPI::NameObject(shaderHandle, filePath.filename().string().data());
        return shaderHandle;
    }

}
