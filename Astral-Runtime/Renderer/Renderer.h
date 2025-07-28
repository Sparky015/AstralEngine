/**
* @file Renderer.h
* @author Andrew Fagan
* @date 7/4/25
*/

#pragma once

namespace Astral {

    struct SceneDescription; // Forward declared
    struct RendererSettings; // Forward declared

    enum class RendererType : uint8
    {
        FORWARD,
        DEFERRED
    };

    class Renderer
    {
    public:
        virtual ~Renderer() = default;

        virtual void Init() = 0;
        virtual void Shutdown() = 0;

        virtual void BeginScene(const SceneDescription& sceneDescription) = 0;
        virtual void EndScene() = 0;

        virtual void Submit(Mesh& mesh, Material& material, Mat4& transform) = 0;

        virtual void SetRendererSettings(const RendererSettings& rendererSettings) = 0;
        virtual const RendererSettings& GetRendererSettings() = 0;
        virtual DescriptorSetHandle GetViewportTexture() = 0;
        virtual void ResizeViewport(uint32 width, uint32 height) = 0;
        virtual UVec2 GetViewportSize() = 0;

        virtual RendererType GetType() = 0;
    };

    inline std::string_view RendererTypeToString(RendererType rendererType)
    {
        switch (rendererType)
        {
            case RendererType::DEFERRED: return "Deferred";
            case RendererType::FORWARD: return "Forward";
            default: return "Unsupported Renderer Type";
        }
    }

}
