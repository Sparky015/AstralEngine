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
#include "Resources/PipelineState.h"
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

        virtual void ExecuteOneTimeAndBlock(const std::function<void(CommandBufferHandle)>& callback) = 0;

        virtual void NameObject(BufferHandle bufferHandle, std::string_view name) = 0;
        virtual void NameObject(DescriptorSetHandle descriptorSetHandle, std::string_view name) = 0;
        virtual void NameObject(TextureHandle textureHandle, std::string_view name) = 0;
        virtual void NameObject(ShaderHandle shaderHandle, std::string_view name) = 0;
        virtual void NameObject(FramebufferHandle framebufferHandle, std::string_view name) = 0;
        virtual void NameObject(CommandBufferHandle commandBufferHandle, std::string_view name) = 0;
        virtual void NameObject(RenderPassHandle renderPassHandle, std::string_view name) = 0;

        virtual void CallImGuiDraws(CommandBufferHandle commandBufferHandle) = 0;

        const RendererDebugStats& GetRendererDebugStats() const { return m_DebugStatsLastFrame; }
        RendererDebugStats& GetInProgressRendererDebugStats() { return m_DebugStatsThisFrame; }

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
