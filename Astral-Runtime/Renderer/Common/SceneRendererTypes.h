/**
* @file SceneRendererTypes.h
* @author Andrew Fagan
* @date 9/2/25
*/

#pragma once

#include "EnvironmentMap.h"
#include "Asset/Ref.h"
#include "ECS/Components/PointLightComponent.h"
#include "Renderer/Cameras/Camera.h"

#include <string_view>

namespace Astral {

    struct SceneDescription
    {
        Camera& Camera;
        std::vector<Light>& Lights;
        Ref<EnvironmentMap> EnvironmentMap;
        float AmbientLightConstant;
        float Exposure;
    };

    enum class RendererType : uint8
    {
        FORWARD,
        DEFERRED
    };

    enum class RendererDebugView
    {
        NONE,
        GBUFFER_ALBEDO,
        GBUFFER_ROUGHNESS,
        GBUFFER_METALLIC,
        GBUFFER_EMISSION,
        GBUFFER_NORMAL,
        DEPTH
    };

    struct RendererSettings
    {
        RendererType RendererType;
        bool IsVSyncEnabled;
        bool IsFrustumCullingEnabled;
        bool IsShadowsOn;
        RendererDebugView DebugView;
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

    inline std::string_view RendererDebugViewToString(RendererDebugView debugView)
    {
        switch (debugView)
        {
            case RendererDebugView::NONE: return "None";
            case RendererDebugView::GBUFFER_ALBEDO: return "GBuffer-Albedo";
            case RendererDebugView::GBUFFER_ROUGHNESS: return "GBuffer-Roughness";
            case RendererDebugView::GBUFFER_METALLIC: return "GBuffer-Metallic";
            case RendererDebugView::GBUFFER_EMISSION: return "GBuffer-Emission";
            case RendererDebugView::GBUFFER_NORMAL: return "GBuffer-Normal";
            case RendererDebugView::DEPTH: return "Depth";
            default: return "Unsupported Renderer Type";
        }
    }


}
