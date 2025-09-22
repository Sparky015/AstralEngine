/**
* @file AssetRegistryShaderLoaderTest.cpp
* @author Andrew Fagan
* @date 9/22/25
*/

#include "gtest/gtest.h"
#include "gtest/gtest-param-test.h"

#include "Renderer/RHI/RendererAPI.h"
#include "Asset/AssetRegistry.h"

namespace Astral {

class AssetRegistryShaderLoaderTest : public ::testing::TestWithParam<const char*>
{
public:

    void SetUp() override
    {
        TestAssetRegistry.Init();
        TestAssetRegistry.SetAssetDirectoryPath(ASTRAL_TEST_ASSETS_DIRECTORY);
        RendererAPI::GetContext().ClearNumValidationErrorsAndWarnings();
    }

    AssetRegistry TestAssetRegistry;

};

TEST_P(AssetRegistryShaderLoaderTest, CreateAsset_LoadsShadersCorrectlyWithoutError)
{
    const char* path = GetParam();
    Ref<Shader> shader = TestAssetRegistry.CreateAsset<Shader>(path);
    ASSERT_NE(shader, nullptr);
    ASSERT_EQ(RendererAPI::GetContext().GetNumValidationErrorsAndWarnings(), 0);
}

INSTANTIATE_TEST_SUITE_P(
    ShaderLoaderTestPaths,
    AssetRegistryShaderLoaderTest,
    ::testing::Values(
        "Shaders/ComputeIrradianceMap.comp",
        "Shaders/DeferredLightingPass.frag",
        "Shaders/EnvironmentMap.frag",
        "Shaders/ShadowMap.vert"
    )
);


}