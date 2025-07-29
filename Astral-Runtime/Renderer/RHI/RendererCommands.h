/**
* @file RendererAPI.h
* @author Andrew Fagan
* @date 12/5/2024
*/

#pragma once

#include "Core/Events/EventListener.h"
#include "Core/EngineLoopEvents.h"
#include "Core/Math/Math.h"
#include "Resources/CommandBuffer.h"
#include "Resources/IndexBuffer.h"
#include "Resources/PipelineStateObject.h"
#include "Common/PipelineBarriers.h"
#include "Resources/RenderTarget.h"
#include "Resources/Shader.h"

namespace Astral {

    enum class API : uint8 {
        None = 0, Vulkan, DirectX12, Metal
    };

    struct RendererDebugStats
    {
        uint32 NumberOfDrawCalls;
        uint32 NumberOfTriangles;
        uint32 NumberOfVertices;
    };

    class RendererCommands {
    public:
        RendererCommands();
        virtual ~RendererCommands();

        static inline API GetAPI() { return s_RendererAPI; }

        virtual void Clear(CommandBufferHandle commandBuffer, RenderTargetHandle renderTarget) = 0;
        virtual void SetClearColor(float r, float g, float b, float a) = 0;
        virtual void DrawElementsIndexed(CommandBufferHandle commandBufferHandle, IndexBufferHandle indexBufferHandle) = 0;
        virtual void PushConstants(CommandBufferHandle commandBufferHandle, PipelineStateObjectHandle
                                        pipelineStateObjectHandle, void* data, uint32 sizeInBytes) = 0;
        virtual void SetPipelineBarrier(CommandBufferHandle commandBufferHandle, const PipelineBarrier& pipelineBarrier) = 0;

        virtual void SetBlending(bool enable) = 0; // TODO: Remove this

        virtual void BeginLabel(CommandBufferHandle commandBufferHandle, std::string_view label, Vec4 color) = 0;
        virtual void EndLabel(CommandBufferHandle commandBufferHandle) = 0;
        virtual void InsertMarker(CommandBufferHandle commandBufferHandle, std::string_view label, Vec4 color) = 0;
        virtual void NameObject(BufferHandle bufferHandle, std::string_view name) = 0;
        virtual void NameObject(DescriptorSetHandle descriptorSetHandle, std::string_view name) = 0;
        virtual void NameObject(TextureHandle textureHandle, std::string_view name) = 0;
        virtual void NameObject(ShaderHandle shaderHandle, std::string_view name) = 0;
        virtual void NameObject(FramebufferHandle framebufferHandle, std::string_view name) = 0;
        virtual void NameObject(CommandBufferHandle commandBufferHandle, std::string_view name) = 0;
        virtual void NameObject(RenderPassHandle renderPassHandle, std::string_view name) = 0;

        virtual void CallImGuiDraws(CommandBufferHandle commandBufferHandle) = 0;

        RendererDebugStats GetNumberOfDrawCalls() { return m_DebugStatsLastFrame; }

    protected:
        RendererDebugStats m_DebugStatsThisFrame = {};
        RendererDebugStats m_DebugStatsLastFrame = {};
        EventListener<NewFrameEvent> m_NewFrameListener{[this](NewFrameEvent) {
            m_DebugStatsLastFrame = m_DebugStatsThisFrame;
            m_DebugStatsThisFrame = {};
        }};

    private:
        static API s_RendererAPI;
    };

}
