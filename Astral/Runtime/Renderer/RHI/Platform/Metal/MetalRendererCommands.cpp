/**
* @file MetalRendererCommands.cpp
* @author Andrew Fagan
* @date 6/27/26
*/

#include "MetalRendererCommands.h"

#include "Debug/ImGui/ImGuiDependencies/imgui_impl_metal.h"
#include "Renderer/RHI/RendererAPI.h"
#include "Resources/MetalCommandBuffer.h"
#include "Resources/MetalShader.h"

namespace Astral {

    void MetalRendererCommands::ExecuteOneTimeAndBlock(const std::function<void(CommandBufferHandle)>& callback)
    {
        CommandBufferHandle commandBufferHandle = RendererAPI::GetDevice().AllocateCommandBuffer();

        commandBufferHandle->BeginRecording();
        callback(commandBufferHandle);
        commandBufferHandle->EndRecording();

        CommandQueueHandle queueHandle = RendererAPI::GetDevice().GetAsyncCommandQueue();
        queueHandle->Submit(commandBufferHandle);
        queueHandle->WaitIdle();
    }


    void MetalRendererCommands::NameObject(BufferHandle bufferHandle, std::string_view name)
    {
        MTL::Buffer* buffer = (MTL::Buffer*)bufferHandle->GetNativeHandle();
        buffer->setLabel(NS::String::string(name.data(), NS::UTF8StringEncoding));
    }


    void MetalRendererCommands::NameObject(DescriptorSetHandle descriptorSetHandle, std::string_view name)
    {
        MTL::Buffer* buffer = (MTL::Buffer*)descriptorSetHandle->GetNativeHandle();
        buffer->setLabel(NS::String::string(name.data(), NS::UTF8StringEncoding));
    }


    void MetalRendererCommands::NameObject(TextureHandle textureHandle, std::string_view name)
    {
        MTL::Texture* texture = (MTL::Texture*)textureHandle->GetNativeImage();
        texture->setLabel(NS::String::string(name.data(), NS::UTF8StringEncoding));
    }


    void MetalRendererCommands::NameObject(ShaderHandle shaderHandle, std::string_view name)
    {
        MTL::Library* shaderLibrary = (MTL::Library*)shaderHandle->GetNativeHandle();
        shaderLibrary->setLabel(NS::String::string(name.data(), NS::UTF8StringEncoding));

        GraphicsRef<MetalShader> metalShader = std::static_pointer_cast<MetalShader>(shaderHandle);
        metalShader->GetFunctionHandle()->setLabel(NS::String::string(name.data(), NS::UTF8StringEncoding));
    }


    void MetalRendererCommands::NameObject(CommandBufferHandle commandBufferHandle, std::string_view name)
    {
        MTL4::CommandBuffer* commandBuffer = (MTL4::CommandBuffer*)commandBufferHandle->GetNativeHandle();
        commandBuffer->setLabel(NS::String::string(name.data(), NS::UTF8StringEncoding));
    }


    void MetalRendererCommands::CallImGuiDraws(CommandBufferHandle commandBufferHandle)
    {
        GraphicsRef<MetalCommandBuffer> metalCommandBuffer = std::static_pointer_cast<MetalCommandBuffer>(commandBufferHandle);
        MTL4::CommandBuffer* commandBuffer = (MTL4::CommandBuffer*)metalCommandBuffer->GetNativeHandle();
        MTL4::RenderCommandEncoder* renderCommandEncoder = metalCommandBuffer->GetRenderCommandEncoder();
        ImDrawData* drawData = ImGui::GetDrawData();

        if (drawData)
        {
            ImGui_ImplMetal_RenderDrawData(drawData, commandBuffer, renderCommandEncoder);
        }
    }

}

