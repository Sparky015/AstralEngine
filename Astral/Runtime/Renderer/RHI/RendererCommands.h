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
        std::atomic_uint32_t NumberOfDrawCalls;
        std::atomic_uint32_t NumberOfTriangles;
        std::atomic_uint32_t NumberOfVertices;
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
        virtual void NameObject(CommandBufferHandle commandBufferHandle, std::string_view name) = 0;

        virtual void CallImGuiDraws(CommandBufferHandle commandBufferHandle) = 0;

        const RendererDebugStats& GetRendererDebugStats() const { return m_DebugStatsLastFrame; }
        RendererDebugStats& GetInProgressRendererDebugStats() { return m_DebugStatsThisFrame; }

    protected:
        RendererDebugStats m_DebugStatsThisFrame = {};
        RendererDebugStats m_DebugStatsLastFrame = {};
        EventListener<NewFrameEvent> m_NewFrameListener{[this](NewFrameEvent) {
            m_DebugStatsLastFrame.NumberOfDrawCalls.store(m_DebugStatsThisFrame.NumberOfDrawCalls);
            m_DebugStatsLastFrame.NumberOfTriangles.store(m_DebugStatsThisFrame.NumberOfTriangles);
            m_DebugStatsLastFrame.NumberOfVertices.store(m_DebugStatsThisFrame.NumberOfVertices);

            m_DebugStatsThisFrame.NumberOfDrawCalls.store(0);
            m_DebugStatsThisFrame.NumberOfTriangles.store(0);
            m_DebugStatsThisFrame.NumberOfVertices.store(0);
        }};

    private:
        static API s_RendererAPI;
    };

}
