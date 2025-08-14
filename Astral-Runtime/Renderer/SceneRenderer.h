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

#include "Renderer/Renderer.h"

namespace Astral {

    struct SceneDescription
    {
        Camera& Camera;
        std::vector<Light>& Lights;
        TextureHandle EnvironmentMap;
        float AmbientLightConstant;
        float Exposure;
    };

    struct RendererSettings
    {
        bool IsVSyncEnabled;
    };

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

        static void UpdateRendererType();

        static GraphicsOwnedPtr<Renderer> m_Renderer;
        static bool m_IsRendererTypeUpdateNeeded;
        static RendererType m_NewRendererType;
    };

} // Astral
