//
// Created by sparky on 7/17/2024.
//

#pragma once

#include "Solas/Core/Layers/Layer.h"

#include "Renderer.h"

#ifdef AYLA_OPENGL_RENDERER
    #include "Solas/Renderer/Platform/OpenGL/OpenGLRenderer.h"
#endif


namespace Solas::Renderer{

    class RendererLayer : public Core::Layers::ILayer
    {
    public:
        RendererLayer();
        ~RendererLayer();

        virtual void OnAttach() override;
        virtual void OnUpdate() override;
        virtual EventManagement::EEventCategory GetAcceptingEventFlags() override;

    private:
#ifdef AYLA_OPENGL_RENDERER
        OpenGLRenderer m_Renderer;
#endif
    };



} // namespace Solas::Renderer