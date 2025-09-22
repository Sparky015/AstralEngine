/**
* @file FramebufferTest.cpp
* @author Andrew Fagan
* @date 9/21/25
*/

/**
* @file DescriptorSetTest.cpp
* @author Andrew Fagan
* @date 9/21/25
*/


#include "gtest/gtest.h"

#include "Renderer/RHI/RendererAPI.h"
#include "Renderer/RHI/Resources/Framebuffer.h"

namespace Astral {

    class FramebufferTests : public ::testing::Test
    {
    public:

        void SetUp() override
        {
            RendererAPI::GetContext().ClearNumValidationErrorsAndWarnings();
        }

    };

    TEST_F(FramebufferTests, BuildFramebuffer_CorrectlyCreatesFramebufferWithoutError)
    {
        AttachmentDescription attachmentDescription = {
            .Format = ImageFormat::R8G8B8A8_UNORM,
            .ImageUsageFlags = IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            .LoadOp = AttachmentLoadOp::CLEAR,
            .StoreOp = AttachmentStoreOp::DONT_CARE,
            .InitialLayout = ImageLayout::GENERAL,
            .FinalLayout = ImageLayout::GENERAL,
            .ClearColor = Vec4(0.0)
        };
        RenderPassHandle renderPass = RendererAPI::GetDevice().CreateRenderPass();
        renderPass->BeginBuildingRenderPass();
        AttachmentIndex attachmentIndex = renderPass->DefineAttachment(attachmentDescription);
        renderPass->BeginBuildingSubpass();
        renderPass->AddColorAttachment(attachmentIndex, ImageLayout::COLOR_ATTACHMENT_OPTIMAL);
        renderPass->EndBuildingSubpass();
        renderPass->EndBuildingRenderPass();

        FramebufferHandle framebuffer = RendererAPI::GetDevice().CreateFramebuffer(renderPass);

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

        framebuffer->BeginBuildingFramebuffer(20, 20, 1);
        framebuffer->AttachTexture(testTexture);
        framebuffer->EndBuildingFramebuffer();

        ASSERT_EQ(RendererAPI::GetContext().GetNumValidationErrorsAndWarnings(), 0);
    }


    TEST_F(FramebufferTests, BuildFramebuffer_2_CorrectlyCreatesFramebufferWithoutError)
    {
        AttachmentDescription attachmentDescription = {
            .Format = ImageFormat::R16G16B16A16_SFLOAT,
            .ImageUsageFlags = IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            .LoadOp = AttachmentLoadOp::CLEAR,
            .StoreOp = AttachmentStoreOp::DONT_CARE,
            .InitialLayout = ImageLayout::GENERAL,
            .FinalLayout = ImageLayout::GENERAL,
            .ClearColor = Vec4(0.0)
        };
        RenderPassHandle renderPass = RendererAPI::GetDevice().CreateRenderPass();
        renderPass->BeginBuildingRenderPass();
        AttachmentIndex attachmentIndex = renderPass->DefineAttachment(attachmentDescription);
        renderPass->BeginBuildingSubpass();
        renderPass->AddColorAttachment(attachmentIndex, ImageLayout::COLOR_ATTACHMENT_OPTIMAL);
        renderPass->EndBuildingSubpass();
        renderPass->EndBuildingRenderPass();

        FramebufferHandle framebuffer = RendererAPI::GetDevice().CreateFramebuffer(renderPass);

        TextureCreateInfo testTextureCreateInfo = {
            .Format = ImageFormat::R16G16B16A16_SFLOAT,
            .Layout = ImageLayout::GENERAL,
            .UsageFlags = IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            .Dimensions = Vec2(40, 40),
            .ImageData = nullptr,
            .LayerCount = 3,
            .MipMapCount = 1,
        };
        TextureHandle testTexture = RendererAPI::GetDevice().Create2DTextureArray(testTextureCreateInfo);

        framebuffer->BeginBuildingFramebuffer(40, 40, 3);
        framebuffer->AttachTexture(testTexture);
        framebuffer->EndBuildingFramebuffer();

        ASSERT_EQ(RendererAPI::GetContext().GetNumValidationErrorsAndWarnings(), 0);
    }



}