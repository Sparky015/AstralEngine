//
// Created by sparky on 7/17/2024.
//

#pragma once


#include "Ayla/Core/Layers/Layer.h"

#include "Renderer.h"

#ifdef AYLA_OPENGL_RENDERER
    #include "Ayla/Renderer/Platform/OpenGL/OpenGLRenderer.h"
#endif


namespace Ayla::Renderer{

    class RendererLayer : public Core::Layers::ILayer
    {
    public:
        RendererLayer();
        ~RendererLayer();

        virtual void OnAttach() override;
        virtual void OnUpdate() override;
        virtual Events::EEventCategory GetAcceptingEventFlags() override;

    private:
#ifdef AYLA_OPENGL_RENDERER
        OpenGLRenderer m_Renderer;
#endif
    };



} // namespace Ayla::Renderer