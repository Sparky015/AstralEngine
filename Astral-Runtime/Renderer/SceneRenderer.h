//
// Created by Andrew Fagan on 11/29/24.
//

#pragma once

#include "SceneRendererImpl.h"

namespace Astral {

    class SceneRenderer
    {
    public:
        static void Init();
        static void Shutdown();

        static void BeginScene(const SceneDescription& sceneDescription);
        static void EndScene();

        static void Submit(Mesh& mesh, Material& material, Mat4& transform);

        static void SetRendererSettings(const RendererSettings& rendererSettings);
        static const RendererSettings& GetRendererSettings();
        static DescriptorSetHandle GetViewportTexture();
        static void ResizeViewport(uint32 width, uint32 height);
        static UVec2 GetViewportSize() { return m_Renderer->GetViewportSize(); }

        static RendererDebugStats GetRendererDebugStats();

        static RendererType GetRendererType();
        static API GetRendererAPIBackend();

        static void SetRendererType(RendererType rendererType);

    private:

        static GraphicsOwnedPtr<SceneRendererImpl> m_Renderer;
    };

} // Astral
