/**
* @file TextureTest.cpp
* @author Andrew Fagan
* @date 9/22/25
*/


#include "gtest/gtest.h"

#include "Renderer/RHI/RendererAPI.h"
#include "Renderer/RHI/Resources/Texture.h"

namespace Astral {

    class TextureTests : public ::testing::TestWithParam<std::tuple<ImageFormat, Vec2, uint32, uint32, SamplerFilter, SamplerAddressMode>>
    {
    public:

        void SetUp() override
        {
            RendererAPI::GetContext().ClearNumValidationErrorsAndWarnings();
        }

    };

    TEST_P(TextureTests, Contructor_CorrectlyCreatesTextureWithoutError)
    {
        auto& [imageFormat, dimensions, layerCount, mipMapCount, samplerFilter, samplerAddressMode] = GetParam();

        TextureCreateInfo testTextureCreateInfo = {
            .Format = imageFormat,
            .Layout = ImageLayout::GENERAL,
            .UsageFlags = IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            .Dimensions = dimensions,
            .ImageData = nullptr,
            .LayerCount = layerCount,
            .MipMapCount = mipMapCount,
            .GenerateMipMaps = false,
            .SamplerFilter = samplerFilter,
            .SamplerAddressMode = samplerAddressMode,
        };
        TextureHandle testTexture = RendererAPI::GetDevice().CreateTexture(testTextureCreateInfo);

        ASSERT_NE(testTexture, nullptr);
        ASSERT_NE(testTexture->GetNativeImage(), nullptr);
        ASSERT_NE(testTexture->GetNativeImageView(), nullptr);
        ASSERT_NE(testTexture->GetNativeSampler(), nullptr);
        ASSERT_EQ(RendererAPI::GetContext().GetNumValidationErrorsAndWarnings(), 0);
    }


    TEST_P(TextureTests, Texture3D_Contructor_CorrectlyCreatesTextureWithoutError)
    {
        auto& [imageFormat, dimensions, layerCount, mipMapCount, samplerFilter, samplerAddressMode] = GetParam();

        TextureCreateInfo testTextureCreateInfo = {
            .Format = imageFormat,
            .Layout = ImageLayout::COLOR_ATTACHMENT_OPTIMAL,
            .UsageFlags = IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            .Dimensions = dimensions,
            .ImageData = nullptr,
            .LayerCount = layerCount,
            .MipMapCount = mipMapCount,
            .GenerateMipMaps = false,
            .SamplerFilter = samplerFilter,
            .SamplerAddressMode = samplerAddressMode,
        };

        if (dimensions.x > 512)
        {
            testTextureCreateInfo.Dimensions = Vec2(512, 512);
        }

        TextureHandle testTexture = RendererAPI::GetDevice().Create3DTexture(testTextureCreateInfo);

        ASSERT_NE(testTexture, nullptr);
        ASSERT_NE(testTexture->GetNativeImage(), nullptr);
        ASSERT_NE(testTexture->GetNativeImageView(), nullptr);
        ASSERT_NE(testTexture->GetNativeSampler(), nullptr);
        ASSERT_EQ(RendererAPI::GetContext().GetNumValidationErrorsAndWarnings(), 0);
    }


    TEST_P(TextureTests, Texture2DArray_Contructor_CorrectlyCreatesTextureWithoutError)
    {
        auto& [imageFormat, dimensions, layerCount, mipMapCount, samplerFilter, samplerAddressMode] = GetParam();

        TextureCreateInfo testTextureCreateInfo = {
            .Format = imageFormat,
            .Layout = ImageLayout::GENERAL,
            .UsageFlags = IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            .Dimensions = dimensions,
            .ImageData = nullptr,
            .LayerCount = layerCount,
            .MipMapCount = mipMapCount,
            .GenerateMipMaps = false,
            .SamplerFilter = samplerFilter,
            .SamplerAddressMode = samplerAddressMode,
        };
        TextureHandle testTexture = RendererAPI::GetDevice().Create2DTextureArray(testTextureCreateInfo);

        ASSERT_NE(testTexture, nullptr);
        ASSERT_NE(testTexture->GetNativeImage(), nullptr);
        ASSERT_NE(testTexture->GetNativeImageView(), nullptr);
        ASSERT_NE(testTexture->GetNativeSampler(), nullptr);
        ASSERT_EQ(RendererAPI::GetContext().GetNumValidationErrorsAndWarnings(), 0);
    }


    TEST_P(TextureTests, TextureCubemap_Contructor_CorrectlyCreatesTextureWithoutError)
    {
        auto& [imageFormat, dimensions, layerCount, mipMapCount, samplerFilter, samplerAddressMode] = GetParam();

        TextureCreateInfo testTextureCreateInfo = {
            .Format = imageFormat,
            .Layout = ImageLayout::GENERAL,
            .UsageFlags = IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            .Dimensions = dimensions,
            .ImageData = nullptr,
            .LayerCount = layerCount,
            .MipMapCount = mipMapCount,
            .GenerateMipMaps = false,
            .SamplerFilter = samplerFilter,
            .SamplerAddressMode = samplerAddressMode,
        };
        TextureHandle testTexture = RendererAPI::GetDevice().CreateCubemap(testTextureCreateInfo);

        ASSERT_NE(testTexture, nullptr);
        ASSERT_NE(testTexture->GetNativeImage(), nullptr);
        ASSERT_NE(testTexture->GetNativeImageView(), nullptr);
        ASSERT_NE(testTexture->GetNativeSampler(), nullptr);
        ASSERT_EQ(RendererAPI::GetContext().GetNumValidationErrorsAndWarnings(), 0);
    }


    TEST_P(TextureTests, Texture1D_Contructor_CorrectlyCreatesTextureWithoutError)
    {
        auto& [imageFormat, dimensions, layerCount, mipMapCount, samplerFilter, samplerAddressMode] = GetParam();

        TextureCreateInfo testTextureCreateInfo = {
            .Format = imageFormat,
            .Layout = ImageLayout::SHADER_READ_ONLY_OPTIMAL,
            .UsageFlags = IMAGE_USAGE_SAMPLED_BIT,
            .Dimensions = dimensions,
            .ImageData = nullptr,
            .LayerCount = layerCount,
            .MipMapCount = mipMapCount,
            .GenerateMipMaps = false,
            .SamplerFilter = samplerFilter,
            .SamplerAddressMode = samplerAddressMode,
        };
        TextureHandle testTexture = RendererAPI::GetDevice().Create1DTexture(testTextureCreateInfo);

        ASSERT_NE(testTexture, nullptr);
        ASSERT_NE(testTexture->GetNativeImage(), nullptr);
        ASSERT_NE(testTexture->GetNativeImageView(), nullptr);
        ASSERT_NE(testTexture->GetNativeSampler(), nullptr);
        ASSERT_EQ(RendererAPI::GetContext().GetNumValidationErrorsAndWarnings(), 0);
    }


    TEST_P(TextureTests, ContructorWithMipMaps_CorrectlyCreatesTextureWithoutError)
    {
        auto& [imageFormat, dimensions, layerCount, mipMapCount, samplerFilter, samplerAddressMode] = GetParam();

        TextureCreateInfo testTextureCreateInfo = {
            .Format = imageFormat,
            .Layout = ImageLayout::SHADER_READ_ONLY_OPTIMAL,
            .UsageFlags = IMAGE_USAGE_SAMPLED_BIT,
            .Dimensions = dimensions,
            .ImageData = nullptr,
            .LayerCount = layerCount,
            .MipMapCount = Texture::CalculateMipMapLevels(dimensions.x, dimensions.y),
            .GenerateMipMaps = true,
            .SamplerFilter = samplerFilter,
            .SamplerAddressMode = samplerAddressMode,
        };
        TextureHandle testTexture = RendererAPI::GetDevice().CreateTexture(testTextureCreateInfo);

        ASSERT_NE(testTexture, nullptr);
        ASSERT_NE(testTexture->GetNativeImage(), nullptr);
        ASSERT_NE(testTexture->GetNativeImageView(), nullptr);
        ASSERT_NE(testTexture->GetNativeSampler(), nullptr);
        ASSERT_EQ(RendererAPI::GetContext().GetNumValidationErrorsAndWarnings(), 0);
    }


    INSTANTIATE_TEST_SUITE_P(
        TextureTestsParams,
        TextureTests,
        ::testing::Combine(
            ::testing::Values(ImageFormat::R8_UNORM, ImageFormat::R8G8B8A8_UNORM, ImageFormat::R16G16B16A16_SFLOAT), // Image Format
            ::testing::Values(Vec2(1, 1), Vec2(20, 20), Vec2(512, 512), Vec2(1024, 1024)),                           // Image Dimensions
            ::testing::Values(1, 2, 5),                                                                              // Layer Count
            ::testing::Values(1, 3, 5),                                                                              // Mip Map Count
            ::testing::Values(SamplerFilter::NEAREST, SamplerFilter::LINEAR),                                        // Sampler Filter
            ::testing::Values(SamplerAddressMode::REPEAT, SamplerAddressMode::CLAMP_TO_EDGE,                         // Sampler Address Mode
                              SamplerAddressMode::CLAMP_TO_BORDER)
        )
    );

}