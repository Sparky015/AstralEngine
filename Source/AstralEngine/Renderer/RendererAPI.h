/**
* @file RendererAPI.h
* @author Andrew Fagan
* @date 12/5/2024
*/

#pragma once

#include "Core/Events/EventListener.h"
#include "Renderer/Primitives/VertexArrayObject.h"
#include "Renderer/RendererEvents.h"

namespace Graphics {

    enum class API : uint8 {
        OpenGL = 0, Vulkan, DirectX12, Metal
    };

    class RendererAPI {
    public:
        RendererAPI();
        virtual ~RendererAPI();

        static inline API GetAPI() { return s_RendererAPI; }

        virtual void Clear() = 0;
        virtual void SetClearColor(float r, float g, float b, float a) = 0;
        virtual void DrawElements(VertexArrayObject* vertexArrayObject) = 0;
        virtual void SetBlending(bool enable) = 0 ;

        virtual inline uint32 GetNumberOfDrawCalls() { return m_NumberOfDrawCalls; }
    protected:
        uint32 m_NumberOfDrawCalls = 0;
        Event::EventListener<NewFrameEvent> m_NewFrameListener{[this](NewFrameEvent){this->m_NumberOfDrawCalls = 0;}};

    private:
        static API s_RendererAPI;
    };

}