/**
* @file VulkanRendererCommands.cpp
* @author Andrew Fagan
* @date 3/4/25
*/

#include "VulkanRendererCommands.h"

#include "Common/VkEnumConversions.h"
#include "Core/Engine.h"
#include "Debug/ImGui/ImGuiDependencies/imgui_impl_vulkan.h"
#include "Renderer/RendererManager.h"
#include "Renderer/RHI/RendererAPI.h"

#include <vulkan/vulkan_core.h>

namespace Astral {

    void VulkanRendererCommands::ExecuteOneTimeAndBlock(const std::function<void(CommandBufferHandle)>& callback)
    {
        CommandBufferHandle commandBufferHandle = RendererAPI::GetDevice().AllocateCommandBuffer();

        commandBufferHandle->BeginRecording();
        callback(commandBufferHandle);
        commandBufferHandle->EndRecording();

        CommandQueueHandle queueHandle = RendererAPI::GetDevice().GetAsyncCommandQueue();
        queueHandle->SubmitSync(commandBufferHandle);
        queueHandle->WaitIdle();
    }


    void VulkanRendererCommands::NameObject(BufferHandle bufferHandle, std::string_view name)
    {
        RenderingContext& context = Engine::Get().GetRendererManager().GetContext();
        thread_local VkInstance instance = (VkInstance)context.GetInstanceHandle();
        thread_local PFN_vkSetDebugUtilsObjectNameEXT vkSetDebugUtilsObjectNameEXT = (PFN_vkSetDebugUtilsObjectNameEXT)vkGetInstanceProcAddr(instance, "vkSetDebugUtilsObjectNameEXT");
        VkDevice device = (VkDevice)context.GetDevice().GetNativeHandle();
        VkBuffer buffer = (VkBuffer)bufferHandle->GetNativeHandle();

        VkDebugUtilsObjectNameInfoEXT nameInfo = {
            .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT,
            .pNext = nullptr,
            .objectType = VK_OBJECT_TYPE_BUFFER,
            .objectHandle = (uint64)buffer,
            .pObjectName = name.data(),
        };

        vkSetDebugUtilsObjectNameEXT(device, &nameInfo);
    }


    void VulkanRendererCommands::NameObject(DescriptorSetHandle descriptorSetHandle, std::string_view name)
    {
        RenderingContext& context = Engine::Get().GetRendererManager().GetContext();
        thread_local VkInstance instance = (VkInstance)context.GetInstanceHandle();
        thread_local PFN_vkSetDebugUtilsObjectNameEXT vkSetDebugUtilsObjectNameEXT = (PFN_vkSetDebugUtilsObjectNameEXT)vkGetInstanceProcAddr(instance, "vkSetDebugUtilsObjectNameEXT");
        VkDevice device = (VkDevice)context.GetDevice().GetNativeHandle();
        VkDescriptorSet descriptorSet = (VkDescriptorSet)descriptorSetHandle->GetNativeHandle();

        VkDebugUtilsObjectNameInfoEXT nameInfo = {
            .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT,
            .pNext = nullptr,
            .objectType = VK_OBJECT_TYPE_DESCRIPTOR_SET,
            .objectHandle = (uint64)descriptorSet,
            .pObjectName = name.data(),
        };

        vkSetDebugUtilsObjectNameEXT(device, &nameInfo);
    }


    void VulkanRendererCommands::NameObject(TextureHandle textureHandle, std::string_view name)
    {
        RenderingContext& context = Engine::Get().GetRendererManager().GetContext();
        thread_local VkInstance instance = (VkInstance)context.GetInstanceHandle();
        thread_local PFN_vkSetDebugUtilsObjectNameEXT vkSetDebugUtilsObjectNameEXT = (PFN_vkSetDebugUtilsObjectNameEXT)vkGetInstanceProcAddr(instance, "vkSetDebugUtilsObjectNameEXT");
        VkDevice device = (VkDevice)context.GetDevice().GetNativeHandle();

        VkImageView imageView = (VkImageView)textureHandle->GetNativeImageView();
        std::string imageViewName = std::string(name) + "_ImageView";
        VkDebugUtilsObjectNameInfoEXT imageViewNameInfo = {
            .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT,
            .pNext = nullptr,
            .objectType = VK_OBJECT_TYPE_IMAGE_VIEW,
            .objectHandle = (uint64)imageView,
            .pObjectName = imageViewName.data(),
        };

        vkSetDebugUtilsObjectNameEXT(device, &imageViewNameInfo);

        VkImage image = (VkImage)textureHandle->GetNativeImage();
        std::string imageName = std::string(name) + "_Image";
        VkDebugUtilsObjectNameInfoEXT imageNameInfo = {
            .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT,
            .pNext = nullptr,
            .objectType = VK_OBJECT_TYPE_IMAGE,
            .objectHandle = (uint64)image,
            .pObjectName = imageName.data(),
        };

        vkSetDebugUtilsObjectNameEXT(device, &imageNameInfo);


        VkSampler sampler = (VkSampler)textureHandle->GetNativeSampler();
        std::string samplerName = std::string(name) + "_Sampler";
        VkDebugUtilsObjectNameInfoEXT samplerNameInfo = {
            .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT,
            .pNext = nullptr,
            .objectType = VK_OBJECT_TYPE_SAMPLER,
            .objectHandle = (uint64)sampler,
            .pObjectName = samplerName.data(),
        };

        vkSetDebugUtilsObjectNameEXT(device, &samplerNameInfo);
    }


    void VulkanRendererCommands::NameObject(ShaderHandle shaderHandle, std::string_view name)
    {
        RenderingContext& context = Engine::Get().GetRendererManager().GetContext();
        thread_local VkInstance instance = (VkInstance)context.GetInstanceHandle();
        thread_local PFN_vkSetDebugUtilsObjectNameEXT vkSetDebugUtilsObjectNameEXT = (PFN_vkSetDebugUtilsObjectNameEXT)vkGetInstanceProcAddr(instance, "vkSetDebugUtilsObjectNameEXT");
        VkDevice device = (VkDevice)context.GetDevice().GetNativeHandle();
        VkShaderModule shader = (VkShaderModule)shaderHandle->GetNativeHandle();

        VkDebugUtilsObjectNameInfoEXT nameInfo = {
            .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT,
            .pNext = nullptr,
            .objectType = VK_OBJECT_TYPE_SHADER_MODULE,
            .objectHandle = (uint64)shader,
            .pObjectName = name.data(),
        };

        vkSetDebugUtilsObjectNameEXT(device, &nameInfo);
    }


    void VulkanRendererCommands::NameObject(FramebufferHandle framebufferHandle, std::string_view name)
    {
        RenderingContext& context = Engine::Get().GetRendererManager().GetContext();
        thread_local VkInstance instance = (VkInstance)context.GetInstanceHandle();
        thread_local PFN_vkSetDebugUtilsObjectNameEXT vkSetDebugUtilsObjectNameEXT = (PFN_vkSetDebugUtilsObjectNameEXT)vkGetInstanceProcAddr(instance, "vkSetDebugUtilsObjectNameEXT");
        VkDevice device = (VkDevice)context.GetDevice().GetNativeHandle();
        VkFramebuffer framebuffer = (VkFramebuffer)framebufferHandle->GetNativeHandle();

        VkDebugUtilsObjectNameInfoEXT nameInfo = {
            .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT,
            .pNext = nullptr,
            .objectType = VK_OBJECT_TYPE_FRAMEBUFFER,
            .objectHandle = (uint64)framebuffer,
            .pObjectName = name.data(),
        };

        vkSetDebugUtilsObjectNameEXT(device, &nameInfo);
    }


    void VulkanRendererCommands::NameObject(CommandBufferHandle commandBufferHandle, std::string_view name)
    {
        RenderingContext& context = Engine::Get().GetRendererManager().GetContext();
        thread_local VkInstance instance = (VkInstance)context.GetInstanceHandle();
        thread_local PFN_vkSetDebugUtilsObjectNameEXT vkSetDebugUtilsObjectNameEXT = (PFN_vkSetDebugUtilsObjectNameEXT)vkGetInstanceProcAddr(instance, "vkSetDebugUtilsObjectNameEXT");
        VkDevice device = (VkDevice)context.GetDevice().GetNativeHandle();

        VkCommandBuffer commandBuffer = (VkCommandBuffer)commandBufferHandle->GetNativeHandle();

        VkDebugUtilsObjectNameInfoEXT nameInfo = {
            .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT,
            .pNext = nullptr,
            .objectType = VK_OBJECT_TYPE_COMMAND_BUFFER,
            .objectHandle = (uint64)commandBuffer,
            .pObjectName = name.data(),
        };

        vkSetDebugUtilsObjectNameEXT(device, &nameInfo);
    }


    void VulkanRendererCommands::NameObject(RenderPassHandle renderPassHandle, std::string_view name)
    {
        RenderingContext& context = Engine::Get().GetRendererManager().GetContext();
        thread_local VkInstance instance = (VkInstance)context.GetInstanceHandle();
        thread_local PFN_vkSetDebugUtilsObjectNameEXT vkSetDebugUtilsObjectNameEXT = (PFN_vkSetDebugUtilsObjectNameEXT)vkGetInstanceProcAddr(instance, "vkSetDebugUtilsObjectNameEXT");
        VkDevice device = (VkDevice)context.GetDevice().GetNativeHandle();

        VkRenderPass renderPass = (VkRenderPass)renderPassHandle->GetNativeHandle();

        VkDebugUtilsObjectNameInfoEXT nameInfo = {
            .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT,
            .pNext = nullptr,
            .objectType = VK_OBJECT_TYPE_RENDER_PASS,
            .objectHandle = (uint64)renderPass,
            .pObjectName = name.data(),
        };

        vkSetDebugUtilsObjectNameEXT(device, &nameInfo);
    }


    void VulkanRendererCommands::CallImGuiDraws(CommandBufferHandle commandBufferHandle)
    {
        VkCommandBuffer commandBuffer = (VkCommandBuffer)commandBufferHandle->GetNativeHandle();
        ImDrawData* drawData = ImGui::GetDrawData();
        if (drawData)
        {
            ImGui_ImplVulkan_RenderDrawData(drawData, commandBuffer);
        }
    }

}
