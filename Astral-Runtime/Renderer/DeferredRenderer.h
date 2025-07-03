/**
* @file DeferredRenderer.h
* @author Andrew Fagan
* @date 7/1/2025
*/


#pragma once

#include "Common/Material.h"
#include "Common/Mesh.h"
#include "Core/Events/EventPublisher.h"
#include "Renderer/Cameras/Camera.h"
#include "RHI/RendererCommands.h"
#include "RHI/Resources/Framebuffer.h"
#include "RHI/Resources/Renderpass.h"
#include "RHI/Resources/PipelineStateCache.h"
#include "Window/WindowEvents.h"

#include <queue>


namespace Astral {

    struct SceneDescription;

    class DeferredRenderer
    {
    public:
        void Init();
        void Shutdown();

        void BeginScene(const SceneDescription& sceneDescription);
        void EndScene();

        void Submit(Mesh& mesh, Material& material, Mat4& transform);

        DescriptorSetHandle GetViewportTexture();
        void ResizeViewport(uint32 width, uint32 height);
        UVec2 GetViewportSize() { return m_ViewportSize; }

    private:

        struct SceneData
        {
            Mat4 CameraViewProjection;
            Mat4 CameraInverseViewMat;
            Mat4 CameraInverseProjectionMat;
            Vec2 ScreenSize;
            alignas(16) Vec3 CameraPosition;
            uint32 NumLights;
        };

        struct GBuffer
        {
            TextureHandle AlbedoBuffer;
            TextureHandle MetallicBuffer;
            TextureHandle RoughnessBuffer;
            TextureHandle EmissionBuffer;
            TextureHandle NormalBuffer;
            TextureHandle DepthBuffer;
            DescriptorSetHandle GBufferDescriptorSet;
        };

        struct FrameContext
        {
            std::vector<Mesh> Meshes;
            std::vector<Material> Materials;
            std::vector<Mat4> Transforms;

            GBuffer GBuffer;

            TextureHandle OffscreenRenderTarget;
            DescriptorSetHandle OffscreenDescriptorSet;

            CommandBufferHandle SceneCommandBuffer;
            FramebufferHandle SceneFramebuffer;
            RenderTargetHandle SceneRenderTarget;
            BufferHandle SceneDataBuffer;
            BufferHandle SceneLightsBuffer;
            DescriptorSetHandle SceneDataDescriptorSet;

            FramebufferHandle WindowFramebuffer;

            std::vector<DescriptorSetHandle> ImGuiTexturesToBeFreed;
            std::vector<TextureHandle> TexturesToBeFreed;
            uint32 FramesTillFree = 2;
        };


        void BuildRenderPasses();
        void CreateGBufferTextures(GBuffer& outGBuffer, UVec2 dimensions);
        void InitializeFrameResources();

        void RenderScene();

        void ResizeImages(uint32 width, uint32 height);

        void GeometryPass();
        void LightingPass();



        std::vector<FrameContext> m_FrameContexts;
        uint32 m_CurrentFrameIndex = -1;
        RenderPassHandle m_MainRenderPass;
        RenderPassHandle m_ImGuiRenderPass;
        EventListener<FramebufferResizedEvent> m_WindowResizedListener{[](FramebufferResizedEvent){}};
        EventPublisher<ViewportResizedEvent> m_ViewportResizedPublisher;
        bool m_IsSceneStarted = false;
        std::queue<DescriptorSetHandle> m_CurrentViewportTexture; // TODO: Remove queue and just make single instance that is nullable

        UVec2 m_ViewportSize{};
        PipelineStateCache m_PipelineStateCache;

        ShaderHandle m_GeometryPassShader;
        ShaderHandle m_LightingShader;
    };

}