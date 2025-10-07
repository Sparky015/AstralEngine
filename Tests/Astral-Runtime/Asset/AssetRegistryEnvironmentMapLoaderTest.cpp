/**
* @file AssetRegistryEnvironmentMapLoaderTest.cpp
* @author Andrew Fagan
* @date 9/22/25
*/

#include "gtest/gtest.h"
#include "gtest/gtest-param-test.h"

#include "Renderer/RHI/RendererAPI.h"
#include "Asset/AssetRegistry.h"

namespace Astral {

class AssetRegistryEnvironmentMapLoaderTest : public ::testing::TestWithParam<const char*>
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

TEST_P(AssetRegistryEnvironmentMapLoaderTest, CreateAsset_LoadsEnvironmentMapsCorrectlyWithoutError)
{
    const char* path = GetParam();
    Ref<EnvironmentMap> environmentMap = TestAssetRegistry.GetOrCreateAsset<EnvironmentMap>(path);
    ASSERT_NE(environmentMap, nullptr);
    ASSERT_EQ(RendererAPI::GetContext().GetNumValidationErrorsAndWarnings(), 0);
}

INSTANTIATE_TEST_SUITE_P(
    EnvironmentMapLoaderTestPaths,
    AssetRegistryEnvironmentMapLoaderTest,
    ::testing::Values(
        "EnvironmentMaps/charolettenbrunn_park_2k.hdr",
        "EnvironmentMaps/little_paris_eiffel_tower_2k.hdr",
        "EnvironmentMaps/qwantani_dusk_2_puresky_2k.hdr"
    )
);


}