/**
* @file AssetRegistryCubeLUTLoaderTest.cpp
* @author Andrew Fagan
* @date 9/22/25
*/

#include "gtest/gtest.h"
#include "gtest/gtest-param-test.h"

#include "Renderer/RHI/RendererAPI.h"
#include "Asset/AssetRegistry.h"
#include "Renderer/Common/CubeLUT.h"

namespace Astral {

class AssetRegistryCubeLUTLoaderTest : public ::testing::TestWithParam<const char*>
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

TEST_P(AssetRegistryCubeLUTLoaderTest, CreateAsset_LoadsCubeLUTCorrectlyWithoutError)
{
    const char* path = GetParam();
    Ref<CubeLUT> cubeLUT = TestAssetRegistry.CreateAsset<CubeLUT>(path);
    ASSERT_NE(cubeLUT, nullptr);
    ASSERT_EQ(RendererAPI::GetContext().GetNumValidationErrorsAndWarnings(), 0);
}

INSTANTIATE_TEST_SUITE_P(
    CubeLUTLoaderTestPaths,
    AssetRegistryCubeLUTLoaderTest,
    ::testing::Values(
        "LUTs/ACEScg_to_sRGB_RRT_ODT.cube"
    )
);


}
