//
// Created by Andrew Fagan on 11/29/24.
//

#pragma once

#include "Common/Material.h"
#include "Common/Mesh.h"
#include "Core/Events/EventPublisher.h"
#include "Renderer/RHI/Resources/VertexBuffer.h"
#include "Renderer/Cameras/Camera.h"
#include "RHI/RendererCommands.h"
#include "RHI/Resources/Framebuffer.h"
#include "RHI/Resources/Renderpass.h"
#include "RHI/Resources/PipelineStateCache.h"
#include "Window/WindowEvents.h"
#include "ECS/Components/PointLightComponent.h"

#include <queue>

#include "ForwardRenderer.h"

namespace Astral {

    struct SceneDescription
    {
        Camera& Camera;
        std::vector<Light>& Lights;
    };

    class SceneRenderer
    {
    public:
        static void Init();
        static void Shutdown();

        static void BeginScene(const SceneDescription& sceneDescription);
        static void EndScene();

        static void Submit(Mesh& mesh, Material& material, Mat4& transform);

        static DescriptorSetHandle GetViewportTexture();
        static void ResizeViewport(uint32 width, uint32 height);
        static UVec2 GetViewportSize() { return m_RendererBackend->GetViewportSize(); }

        static RendererDebugStats GetRendererDebugStats();

        static API GetRendererAPIBackend();

    private:

        static GraphicsOwnedPtr<ForwardRenderer> m_RendererBackend;
    };

} // Astral
