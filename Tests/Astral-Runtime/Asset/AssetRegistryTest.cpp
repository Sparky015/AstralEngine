/**
* @file AssetRegistryTest.cpp
* @author Andrew Fagan
* @date 9/22/25
*/

#include "gtest/gtest.h"

#include "Renderer/RHI/RendererAPI.h"
#include "Asset/AssetRegistry.h"

namespace Astral {

    class AssetRegistryTest : public ::testing::Test
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

    TEST_F(AssetRegistryTest, CreateAsset_CreatesAssetCorrectlyWithoutError)
    {
        Ref<Texture> texture = TestAssetRegistry.GetOrCreateAsset<Texture>("Textures/PNG/SolidWhite.png");
        ASSERT_NE(texture, nullptr);
        ASSERT_EQ(RendererAPI::GetContext().GetNumValidationErrorsAndWarnings(), 0);
    }

    TEST_F(AssetRegistryTest, RegisterRuntimeAsset_RegistersAssetAndStores)
    {
        Ref<Asset> textureAsset = TestAssetRegistry.LoadAsset(AssetType::Texture, std::string(ASTRAL_TEST_ASSETS_DIRECTORY) + "Textures/PNG/SolidWhite.png");
        ASSERT_NE(textureAsset, nullptr);

        Ref<Texture> cachedTexture = TestAssetRegistry.GetAsset<Texture>("Temp://SolidWhiteTexture");
        ASSERT_EQ(cachedTexture, nullptr);

        TestAssetRegistry.RegisterRuntimeAsset<Texture>(std::dynamic_pointer_cast<Texture>(textureAsset), "SolidWhiteTexture");

        cachedTexture = TestAssetRegistry.GetAsset<Texture>("Temp://SolidWhiteTexture");
        ASSERT_NE(cachedTexture, nullptr);

        ASSERT_EQ(cachedTexture, std::dynamic_pointer_cast<Texture>(textureAsset));

        ASSERT_EQ(RendererAPI::GetContext().GetNumValidationErrorsAndWarnings(), 0);
    }

}