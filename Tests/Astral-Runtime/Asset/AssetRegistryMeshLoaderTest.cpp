/**
* @file AssetRegistryMeshLoaderTest.cpp
* @author Andrew Fagan
* @date 9/22/25
*/

#include "gtest/gtest.h"
#include "gtest/gtest-param-test.h"

#include "Renderer/RHI/RendererAPI.h"
#include "Asset/AssetRegistry.h"

namespace Astral {

class AssetRegistryMeshLoaderTest : public ::testing::TestWithParam<const char*>
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

TEST_P(AssetRegistryMeshLoaderTest, CreateAsset_LoadsMeshesCorrectlyWithoutError)
{
    const char* path = GetParam();
    Ref<Mesh> mesh = TestAssetRegistry.CreateAsset<Mesh>(path);
    ASSERT_NE(mesh, nullptr);
    ASSERT_EQ(RendererAPI::GetContext().GetNumValidationErrorsAndWarnings(), 0);
}

INSTANTIATE_TEST_SUITE_P(
    MeshLoaderTestPaths,
    AssetRegistryMeshLoaderTest,
    ::testing::Values(
        "Meshes/Cube.obj",
        "Meshes/Quad.obj",
        "Models/Cerberus/Cerberus_LP.FBX",
        "Models/WaterBendingGauntlet/WaterBendingGauntlet.fbx"
    )
);


}