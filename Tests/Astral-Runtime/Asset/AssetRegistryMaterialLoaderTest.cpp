/**
* @file AssetRegistryMaterialLoaderTest.cpp
* @author Andrew Fagan
* @date 9/22/25
*/


#include "gtest/gtest.h"
#include "gtest/gtest-param-test.h"

#include "Renderer/RHI/RendererAPI.h"
#include "Asset/AssetRegistry.h"

namespace Astral {

class AssetRegistryMaterialLoaderTest : public ::testing::TestWithParam<const char*>
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

TEST_P(AssetRegistryMaterialLoaderTest, CreateAsset_LoadsMaterialsCorrectlyWithoutError)
{
    const char* path = GetParam();
    Ref<Material> material = TestAssetRegistry.CreateAsset<Material>(path);
    ASSERT_NE(material, nullptr);
    ASSERT_EQ(RendererAPI::GetContext().GetNumValidationErrorsAndWarnings(), 0);
}

INSTANTIATE_TEST_SUITE_P(
    MaterialLoaderTestPaths,
    AssetRegistryMaterialLoaderTest,
    ::testing::Values(
        "Materials/MissingMaterial.astmat",
        "Models/WaterBendingGauntlet/WaterBendingGauntlet.astmat",
        "Models/WaterBendingGauntlet/WaterBendingGauntlet_PBR.astmat",
        "Models/Cerberus/Cerberus.astmat"
    )
);


}