/**
* @file DescriptorSetTest.cpp
* @author Andrew Fagan
* @date 9/21/25
*/


#include "gtest/gtest.h"

#include "Renderer/RHI/RendererAPI.h"
#include "Renderer/RHI/Resources/DescriptorSet.h"

namespace Astral {

    class DescriptorSetTests : public ::testing::Test
    {
    public:

        void SetUp() override
        {
            RendererAPI::GetContext().ClearNumValidationErrorsAndWarnings();
        }

    };

    TEST_F(DescriptorSetTests, BuildSet_CorrectlyCreatesDescriptorSetWithoutError)
    {
        DescriptorSetHandle descriptorSet = RendererAPI::GetDevice().CreateDescriptorSet();

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

        float testData[4] = {1.0f, 0.0, 0.0f, 1.0f};
        BufferHandle testUniformBuffer = RendererAPI::GetDevice().CreateUniformBuffer(&testData, sizeof(testData));


        descriptorSet->BeginBuildingSet();
        descriptorSet->AddDescriptorImageSampler(testTexture, ShaderStage::ALL);
        descriptorSet->AddDescriptorUniformBuffer(testUniformBuffer, ShaderStage::ALL);
        descriptorSet->EndBuildingSet();

        ASSERT_EQ(RendererAPI::GetContext().GetNumValidationErrorsAndWarnings(), 0);
    }

    TEST_F(DescriptorSetTests, BuildSet_2_CorrectlyCreatesDescriptorSetWithoutError)
    {
        DescriptorSetHandle descriptorSet = RendererAPI::GetDevice().CreateDescriptorSet();


        float testData[4] = {1.0f, 0.0, 0.0f, 1.0f};
        BufferHandle testUniformBuffer = RendererAPI::GetDevice().CreateUniformBuffer(&testData, sizeof(testData));

        float testData2[3] = {1.0f, 1.0, 0.0f};
        BufferHandle testUniformBuffer2 = RendererAPI::GetDevice().CreateUniformBuffer(&testData2, sizeof(testData2));

        uint32 testData3[8] = {1, 0, 3, 1, 4, 1, 7, 3};
        BufferHandle testStorageBuffer = RendererAPI::GetDevice().CreateStorageBuffer(&testData3, sizeof(testData3));


        descriptorSet->BeginBuildingSet();
        descriptorSet->AddDescriptorUniformBuffer(testUniformBuffer, ShaderStage::ALL);
        descriptorSet->AddDescriptorStorageBuffer(testStorageBuffer, ShaderStage::ALL);
        descriptorSet->AddDescriptorUniformBuffer(testUniformBuffer2, ShaderStage::ALL);
        descriptorSet->EndBuildingSet();

        ASSERT_EQ(RendererAPI::GetContext().GetNumValidationErrorsAndWarnings(), 0);
    }


    TEST_F(DescriptorSetTests, BuildSet_3_CorrectlyCreatesDescriptorSetWithoutError)
    {
        DescriptorSetHandle descriptorSet = RendererAPI::GetDevice().CreateDescriptorSet();


        TextureCreateInfo testTextureCreateInfo = {
            .Format = ImageFormat::R8G8B8A8_UNORM,
            .Layout = ImageLayout::GENERAL,
            .UsageFlags = IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            .Dimensions = Vec2(40, 40),
            .ImageData = nullptr,
            .LayerCount = 1,
            .MipMapCount = 1,
        };
        TextureHandle testTexture = RendererAPI::GetDevice().CreateTexture(testTextureCreateInfo);


        descriptorSet->BeginBuildingSet();
        descriptorSet->AddDescriptorImageSampler(testTexture, ShaderStage::ALL);
        descriptorSet->AddDescriptorImageSampler(testTexture, ShaderStage::ALL);
        descriptorSet->AddDescriptorImageSampler(testTexture, ShaderStage::ALL);
        descriptorSet->AddDescriptorImageSampler(testTexture, ShaderStage::ALL);
        descriptorSet->AddDescriptorImageSampler(testTexture, ShaderStage::ALL);
        descriptorSet->AddDescriptorImageSampler(testTexture, ShaderStage::ALL);
        descriptorSet->AddDescriptorImageSampler(testTexture, ShaderStage::ALL);
        descriptorSet->EndBuildingSet();

        ASSERT_EQ(RendererAPI::GetContext().GetNumValidationErrorsAndWarnings(), 0);
    }


    TEST_F(DescriptorSetTests, UpdateUniformBinding_CorrectlyUpdatesBindingWithoutError)
    {
        DescriptorSetHandle descriptorSet = RendererAPI::GetDevice().CreateDescriptorSet();


        TextureCreateInfo testTextureCreateInfo = {
            .Format = ImageFormat::R8G8B8A8_UNORM,
            .Layout = ImageLayout::GENERAL,
            .UsageFlags = IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            .Dimensions = Vec2(40, 40),
            .ImageData = nullptr,
            .LayerCount = 1,
            .MipMapCount = 1,
        };
        TextureHandle testTexture = RendererAPI::GetDevice().CreateTexture(testTextureCreateInfo);


        float testData[4] = {1.0f, 0.0, 0.0f, 1.0f};
        BufferHandle testUniformBuffer = RendererAPI::GetDevice().CreateUniformBuffer(&testData, sizeof(testData));

        float testData2[3] = {1.0f, 1.0, 0.0f};
        BufferHandle testUniformBuffer2 = RendererAPI::GetDevice().CreateUniformBuffer(&testData2, sizeof(testData2));

        uint32 testData3[8] = {1, 0, 3, 1, 4, 1, 7, 3};
        BufferHandle testStorageBuffer = RendererAPI::GetDevice().CreateStorageBuffer(&testData3, sizeof(testData3));


        descriptorSet->BeginBuildingSet();
        descriptorSet->AddDescriptorImageSampler(testTexture, ShaderStage::ALL);
        descriptorSet->AddDescriptorStorageBuffer(testStorageBuffer, ShaderStage::ALL);
        descriptorSet->AddDescriptorUniformBuffer(testUniformBuffer, ShaderStage::ALL);
        descriptorSet->AddDescriptorImageSampler(testTexture, ShaderStage::ALL);
        descriptorSet->EndBuildingSet();

        descriptorSet->UpdateUniformBinding(2, testUniformBuffer2);
        BufferHandle bindedBuffer = descriptorSet->GetUniformBuffer(2);
        ASSERT_EQ(bindedBuffer, testUniformBuffer2);

        ASSERT_EQ(RendererAPI::GetContext().GetNumValidationErrorsAndWarnings(), 0);
    }


    TEST_F(DescriptorSetTests, UpdateUniformBinding_2_CorrectlyUpdatesBindingWithoutError)
    {
        DescriptorSetHandle descriptorSet = RendererAPI::GetDevice().CreateDescriptorSet();


        TextureCreateInfo testTextureCreateInfo = {
            .Format = ImageFormat::R8G8B8A8_UNORM,
            .Layout = ImageLayout::SHADER_READ_ONLY_OPTIMAL,
            .UsageFlags = IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            .Dimensions = Vec2(40, 40),
            .ImageData = nullptr,
            .LayerCount = 1,
            .MipMapCount = 1,
        };
        TextureHandle testTexture = RendererAPI::GetDevice().CreateTexture(testTextureCreateInfo);

        TextureCreateInfo testTexture2CreateInfo = {
            .Format = ImageFormat::R32G32B32A32_SFLOAT,
            .Layout = ImageLayout::SHADER_READ_ONLY_OPTIMAL,
            .UsageFlags = IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            .Dimensions = Vec2(10, 10),
            .ImageData = nullptr,
            .LayerCount = 1,
            .MipMapCount = 1,
        };
        TextureHandle testTexture2 = RendererAPI::GetDevice().CreateTexture(testTextureCreateInfo);


        float testData[4] = {1.0f, 0.0, 0.0f, 1.0f};
        BufferHandle testUniformBuffer = RendererAPI::GetDevice().CreateUniformBuffer(&testData, sizeof(testData));

        float testData2[3] = {1.0f, 1.0, 0.0f};
        BufferHandle testUniformBuffer2 = RendererAPI::GetDevice().CreateUniformBuffer(&testData2, sizeof(testData2));

        uint32 testData3[8] = {1, 0, 3, 1, 4, 1, 7, 3};
        BufferHandle testStorageBuffer = RendererAPI::GetDevice().CreateStorageBuffer(&testData3, sizeof(testData3));


        descriptorSet->BeginBuildingSet();
        descriptorSet->AddDescriptorImageSampler(testTexture, ShaderStage::ALL);
        descriptorSet->AddDescriptorStorageBuffer(testStorageBuffer, ShaderStage::ALL);
        descriptorSet->AddDescriptorUniformBuffer(testUniformBuffer, ShaderStage::ALL);
        descriptorSet->AddDescriptorImageSampler(testTexture, ShaderStage::ALL);
        descriptorSet->EndBuildingSet();

        descriptorSet->UpdateImageSamplerBinding(3, testTexture2);
        TextureHandle bindedTexture = descriptorSet->GetImageSampler(3);
        ASSERT_EQ(bindedTexture, testTexture2);

        ASSERT_EQ(RendererAPI::GetContext().GetNumValidationErrorsAndWarnings(), 0);
    }


}