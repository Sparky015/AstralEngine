/**
* @file RendererAPI.h
* @author Andrew Fagan
* @date 12/5/2024
*/

#pragma once

#include "Core/Events/EventListener.h"
#include "Core/EngineLoopEvents.h"
#include "Resources/CommandBuffer.h"
#include "Resources/IndexBuffer.h"
#include "Resources/PipelineStateObject.h"
#include "Resources/RenderTarget.h"

namespace Astral {

    enum class API : uint8 {
        None = 0, Vulkan, DirectX12, Metal
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
        virtual void SetBlending(bool enable) = 0 ;

        virtual void CallImGuiDraws(CommandBufferHandle commandBufferHandle) = 0;

        virtual inline uint32 GetNumberOfDrawCalls() { return m_NumberOfDrawCalls; }

    protected:
        uint32 m_NumberOfDrawCalls = 0;
        Astral::EventListener<NewFrameEvent> m_NewFrameListener{[this](NewFrameEvent){this->m_NumberOfDrawCalls = 0;}};

    private:
        static API s_RendererAPI;
    };

}
