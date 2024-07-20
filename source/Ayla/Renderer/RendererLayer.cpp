//
// Created by sparky on 7/17/2024.
//

#include "RendererLayer.h"

#include "Ayla/Renderer/ParseShaders.h"


#include "glad/glad.h"


namespace Ayla::Renderer {


    RendererLayer::RendererLayer() :
            m_Renderer(Shader("../../source/Ayla/Renderer/Shaders/vertex.shader"),
                       Shader("../../source/Ayla/Renderer/Shaders/fragment.shader"))
    {
        AY_TRACE("[Sholas] RendererLayer: Initializing Renderer Layer");
        this->AttachLayer();
        m_DebugName = "Renderer Layer";
    }


    RendererLayer::~RendererLayer()
    {
        AY_TRACE("[Sholas] RendererLayer: Destroying Renderer Layer");
        this->DetachLayer();
    }


    void RendererLayer::OnAttach()
    {
        m_Renderer.InitBuffers();
    }

    void RendererLayer::OnUpdate()
    {
        m_Renderer.Render();
    }


    Events::EEventCategory RendererLayer::GetAcceptingEventFlags()
    {
        return Ayla::Events::None;
    }


} // namespace Ayla::Renderer




