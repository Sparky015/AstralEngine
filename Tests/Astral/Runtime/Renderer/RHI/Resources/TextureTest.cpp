/**
* @file TextureTest.cpp
* @author Andrew Fagan
* @date 9/22/25
*/


#include "gtest/gtest.h"

#include "Renderer/RHI/RendererAPI.h"
#include "Renderer/RHI/Resources/Texture.h"

namespace Astral {

    class TextureTests : public ::testing::Test
    {
    public:

        void SetUp() override
        {
            RendererAPI::GetContext().ClearNumValidationErrorsAndWarnings();
        }

    };

    TEST_F(TextureTests, Contructor_CorrectlyCreatesTextureWithoutError)
    {
        TextureCreateInfo testTextureCreateInfo = {
            .Format = ImageFormat::R8G8B8A8_UNORM,
            .Layout = ImageLayout::GENERAL,
            .UsageFlags = IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            .Dimensions = Vec2(20, 20),
            .ImageData = nullptr,
            .LayerCount = 1,
            .MipMapCount = 1,
        };
        TextureHandle testTexture = RendererAPI::GetDevice().CreateTexture(testTextureCreateInfo);

        ASSERT_EQ(RendererAPI::GetContext().GetNumValidationErrorsAndWarnings(), 0);
    }


    TEST_F(TextureTests, Texture3D_Contructor_CorrectlyCreatesTextureWithoutError)
    {

        TextureCreateInfo testTextureCreateInfo = {
            .Format = ImageFormat::R8G8B8A8_UNORM,
            .Layout = ImageLayout::COLOR_ATTACHMENT_OPTIMAL,
            .UsageFlags = IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            .Dimensions = Vec2(20, 20),
            .ImageData = nullptr,
            .LayerCount = 1,
            .MipMapCount = 1,
        };
        TextureHandle testTexture = RendererAPI::GetDevice().Create3DTexture(testTextureCreateInfo);

        ASSERT_EQ(RendererAPI::GetContext().GetNumValidationErrorsAndWarnings(), 0);
    }


    TEST_F(TextureTests, Texture2DArray_Contructor_CorrectlyCreatesTextureWithoutError)
    {
        TextureCreateInfo testTextureCreateInfo = {
            .Format = ImageFormat::R16G16B16A16_SFLOAT,
            .Layout = ImageLayout::GENERAL,
            .UsageFlags = IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            .Dimensions = Vec2(40, 40),
            .ImageData = nullptr,
            .LayerCount = 10,
            .MipMapCount = 1,
        };
        TextureHandle testTexture = RendererAPI::GetDevice().Create2DTextureArray(testTextureCreateInfo);

        ASSERT_EQ(RendererAPI::GetContext().GetNumValidationErrorsAndWarnings(), 0);
    }


    TEST_F(TextureTests, TextureCubemap_Contructor_CorrectlyCreatesTextureWithoutError)
    {
        TextureCreateInfo testTextureCreateInfo = {
            .Format = ImageFormat::R8G8B8A8_UNORM,
            .Layout = ImageLayout::GENERAL,
            .UsageFlags = IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            .Dimensions = Vec2(20, 20),
            .ImageData = nullptr,
            .LayerCount = 1,
            .MipMapCount = 1,
        };
        TextureHandle testTexture = RendererAPI::GetDevice().CreateCubemap(testTextureCreateInfo);

        ASSERT_EQ(RendererAPI::GetContext().GetNumValidationErrorsAndWarnings(), 0);
    }


    TEST_F(TextureTests, Texture1D_Contructor_CorrectlyCreatesTextureWithoutError)
    {
        TextureCreateInfo testTextureCreateInfo = {
            .Format = ImageFormat::R8_UNORM,
            .Layout = ImageLayout::SHADER_READ_ONLY_OPTIMAL,
            .UsageFlags = IMAGE_USAGE_SAMPLED_BIT,
            .Dimensions = Vec2(50, 0),
            .ImageData = nullptr,
            .LayerCount = 1,
            .MipMapCount = 1,
        };
        TextureHandle testTexture = RendererAPI::GetDevice().Create1DTexture(testTextureCreateInfo);

        ASSERT_EQ(RendererAPI::GetContext().GetNumValidationErrorsAndWarnings(), 0);
    }


    TEST_F(TextureTests, ContructorWithMipMaps_CorrectlyCreatesTextureWithoutError)
    {
        TextureCreateInfo testTextureCreateInfo = {
            .Format = ImageFormat::R8_UNORM,
            .Layout = ImageLayout::SHADER_READ_ONLY_OPTIMAL,
            .UsageFlags = IMAGE_USAGE_SAMPLED_BIT,
            .Dimensions = Vec2(1024, 1024),
            .ImageData = nullptr,
            .LayerCount = 1,
            .MipMapCount = Texture::CalculateMipMapLevels(1024, 1024),
            .GenerateMipMaps = true
        };
        TextureHandle testTexture = RendererAPI::GetDevice().CreateTexture(testTextureCreateInfo);

        ASSERT_EQ(RendererAPI::GetContext().GetNumValidationErrorsAndWarnings(), 0);
    }


    TEST_F(TextureTests, ContructorWithEditedSampler_CorrectlyCreatesTextureWithoutError)
    {
        TextureCreateInfo testTextureCreateInfo = {
            .Format = ImageFormat::R8_UNORM,
            .Layout = ImageLayout::SHADER_READ_ONLY_OPTIMAL,
            .UsageFlags = IMAGE_USAGE_SAMPLED_BIT,
            .Dimensions = Vec2(1024, 1024),
            .ImageData = nullptr,
            .LayerCount = 1,
            .MipMapCount = 1,
            .SamplerFilter = SamplerFilter::NEAREST,
            .SamplerAddressMode = SamplerAddressMode::CLAMP_TO_EDGE,
        };
        TextureHandle testTexture = RendererAPI::GetDevice().CreateTexture(testTextureCreateInfo);

        ASSERT_EQ(RendererAPI::GetContext().GetNumValidationErrorsAndWarnings(), 0);
    }


}