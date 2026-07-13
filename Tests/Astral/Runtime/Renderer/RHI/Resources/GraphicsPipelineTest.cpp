/**
* @file GraphicsPipelineTest.cpp
* @author Andrew Fagan
* @date 6/20/26
*/


#include "gtest/gtest.h"

#include "Renderer/RHI/RendererAPI.h"
#include "Renderer/RHI/Resources/PipelineState.h"

namespace Astral {

class GraphicsPipelineTest : public ::testing::TestWithParam<std::pair<const char*, ShaderType>>
{
protected:

    void SetUp() override
    {
        TestAssetRegistry.Init();
        TestAssetRegistry.SetAssetDirectoryPath(ASTRAL_TEST_ASSETS_DIRECTORY);
        RendererAPI::GetContext().ClearNumValidationErrorsAndWarnings();
    }

    AssetRegistry TestAssetRegistry;

};

TEST_P(GraphicsPipelineTest, ShaderSource_Constructor_CorrectlyLoadsShaderSourceFromDisk)
{
    std::pair<const char*, ShaderType> params = GetParam();
    const char* path = params.first;
    ShaderType shaderType = params.second;
    ShaderSource shaderSource = ShaderSource(std::string(ASTRAL_TEST_ASSETS_DIRECTORY) + '/' + path);

    std::filesystem::path shaderSourceFilePath = std::filesystem::path(path);
    ASSERT_EQ(shaderSourceFilePath.filename(), shaderSource.GetFileName());
    ASSERT_EQ(shaderSource.GetShaderType(), shaderType);
    ASSERT_NE(shaderSource.GetShaderCode().size(), 0);
    ASSERT_EQ(RendererAPI::GetContext().GetNumValidationErrorsAndWarnings(), 0);
}


INSTANTIATE_TEST_SUITE_P(
    ShaderLoaderTestPaths,
    GraphicsPipelineTest,
    ::testing::Values(
        std::pair{"Shaders/Test_ComputeIrradianceMap.comp", ShaderType::COMPUTE_SHADER},
        std::pair{"Shaders/Test_DeferredLightingPass.frag", ShaderType::FRAGMENT_SHADER},
        std::pair{"Shaders/Test_EnvironmentMap.frag", ShaderType::FRAGMENT_SHADER},
        std::pair{"Shaders/Test_ShadowMap.vert", ShaderType::VERTEX_SHADER}
    )
);


}