/**
* @file VulkanFramebuffer.h
* @author Andrew Fagan
* @date 5/16/25
*/

#pragma once

#include "Core/CoreMacroDefinitions.h"
#include "Renderer/RHI/Resources/Framebuffer.h"

#include <vulkan/vulkan_core.h>

#include "Renderer/RHI/Resources/RenderTarget.h"
#include "Renderer/RHI/Resources/Texture.h"

namespace Astral {

    /**
    * @brief Information to create a VulkanFramebuffer from
    */
    struct VulkanFramebufferDesc
    {
        VkDevice Device;
        VkRenderPass RenderPass;
    };

    /**
     * @brief Vulkan Backend RHI Framebuffer Object
     */
    class VulkanFramebuffer : public Framebuffer
    {
    public:
        explicit VulkanFramebuffer(const VulkanFramebufferDesc& desc);
        ~VulkanFramebuffer() override;

        /**
         * @brief  Gets the extent of the framebuffer
         * @return The extent of the framebuffer
         */
        UVec2 GetExtent() override { return {m_FramebufferWidth, m_FramebufferHeight}; }

        /**
         * @brief Begins the building of a framebuffer
         * @param framebufferWidth Specifies the width of the framebuffer
         * @param framebufferHeight Specifies the height of the framebuffer
         * @post  @ref EndBuildingFramebuffer should be called to create the framebuffer
         */
        void BeginBuildingFramebuffer(uint32 framebufferWidth, uint32 framebufferHeight) override;

        /**
         * @brief Attaches a render target to the framebuffer
         * @param renderTargetHandle The render target to attach to the framebuffer
         */
        void AttachRenderTarget(RenderTargetHandle renderTargetHandle) override;

        /**
         * @brief Attaches a render target to the framebuffer
         * @param textureHandle The texture to attach to the framebuffer
         */
        void AttachTexture(TextureHandle textureHandle) override;

        /**
         * @brief Attaches a layer of a texture to the framebuffer
         * @param textureHandle The texture to attach to the framebuffer
         */
        void AttachTexture(TextureHandle textureHandle, uint32 layer) override;

        /**
         * @brief Ends the building of a framebuffer and creates the framebuffer
         * @pre   @ref BeginBuildingFramebuffer should be called to create the framebuffer
         */
        void EndBuildingFramebuffer() override;

        /**
         * @brief  Gets the attachment at the specified index
         * @param  attachmentIndex The index of the attachment
         * @return The texture located at the specified index
         */
        TextureHandle GetAttachment(uint32 attachmentIndex) override;

        /**
         * @brief Destroys the framebuffer and removes any held textures
         */
        void InvalidateFramebuffer() override;

        /**
         * @brief Gets the native framebuffer handle
         * @note  The void pointer maps to the native vulkan framebuffer handle (VkFramebuffer)
         */
        void* GetNativeHandle() override { return m_Framebuffer; }

    private:

        /**
         * @brief Creates the vulkan framebuffer handle (VkFramebuffer)
         */
        void CreateFramebuffer();

        /**
         * @brief Destroys the vulkan framebuffer handle (VkFramebuffer)
         */
        void DestroyFramebuffer();

        VkDevice m_Device;
        VkRenderPass m_RenderPass;

        // Hold a ref to each image added so they will stay alive while framebuffer is alive
        std::vector<TextureHandle> m_Textures;
        std::vector<VkImageView> m_ImageViews;

        uint32 m_FramebufferWidth;
        uint32 m_FramebufferHeight;

        VkFramebuffer m_Framebuffer;
    };

}
