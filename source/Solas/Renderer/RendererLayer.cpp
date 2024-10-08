//
// Created by sparky on 7/17/2024.
//

#include "RendererLayer.h"

#include "Solas/Renderer/Shaders/ParseShaders.h"


#include "glad/glad.h"


namespace Solas::Renderer {


    RendererLayer::RendererLayer() :
            m_Renderer(Shader(std::string(SHADER_DIR) + "vertex.shader"),
                       Shader(std::string(SHADER_DIR) + "fragment.shader"))
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


    EventManagement::EEventCategory RendererLayer::GetAcceptingEventFlags()
    {
        return Solas::EventManagement::None;
    }


} // namespace Solas::Renderer




