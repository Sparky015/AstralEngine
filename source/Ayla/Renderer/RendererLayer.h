//
// Created by sparky on 7/17/2024.
//

#pragma once


#include "Ayla/Core/Layers/Layer.h"


namespace Ayla::Renderer{

    class RendererLayer : public Core::Layers::ILayer
    {
    public:
        RendererLayer();
        ~RendererLayer();

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnUpdate() override;
        virtual Events::EEventCategory GetAcceptingEventFlags() override;

        static unsigned int CompileShader(unsigned int type, const std::string& source);
        static unsigned int CreateShaderProgram(const std::string& vertexShader, const std::string& fragmentShader);

        void InitializeGPUData();
        void Render() const;

    private:

        unsigned int m_Buffer;
        unsigned int m_Ibo;
        unsigned int m_ShaderProgram;

        std::string m_VertexShader;
        std::string m_FragmentShader;


    };



} // namespace Ayla::Renderer