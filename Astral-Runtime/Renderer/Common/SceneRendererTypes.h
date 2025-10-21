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

    /**
     * @struct SceneDescription
     * @brief Defines properties of the scene being rendered
     */
    struct SceneDescription
    {
        Camera& Camera;
        std::vector<Light>& Lights;
        Ref<EnvironmentMap> EnvironmentMap;
        float AmbientLightConstant;
        float Exposure;
    };

    /**
     * @enum RendererType
     * @brief The type of rendering path
     */
    enum class RendererType : uint32
    {
        FORWARD,
        DEFERRED
    };

    /**
     * @enum RendererType
     * @brief The type of rendering path
     */
    enum class RendererDebugView : uint32
    {
        NONE,
        GBUFFER_ALBEDO,
        GBUFFER_ROUGHNESS,
        GBUFFER_METALLIC,
        GBUFFER_EMISSION,
        GBUFFER_NORMAL,
        DEPTH,
        CASCADED_SHADOW_MAP_BOUNDARIES,
        TONE_MAPPING_NONE,
        TONE_MAPPING_REINHARD,
    };

    /**
     * @struct RendererSettings
     * @brief Defines the settings for the renderer
     */
    struct RendererSettings
    {
        RendererType RendererType;
        bool IsVSyncEnabled;
        bool IsFrustumCullingEnabled;
        bool IsShadowsOn;
        int32 NumShadowCascades = 1;
        RendererDebugView DebugView;
        int32 ShadowMapResolution;
        float ShadowMapBias;
        float ShadowMapZMultiplier = 1;
    };


    /**
     * @brief Gets the string version of a renderer type enum value
     * @return The renderer type name as a string
     */
    inline std::string_view RendererTypeToString(RendererType rendererType)
    {
        switch (rendererType)
        {
            case RendererType::DEFERRED: return "Deferred";
            case RendererType::FORWARD: return "Forward";
            default: return "Unsupported Renderer Type";
        }
    }

    /**
     * @brief Gets the string version of a renderer debug view enum value
     * @return The debug view name as a string
     */
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
            case RendererDebugView::CASCADED_SHADOW_MAP_BOUNDARIES: return "Cascaded Shadow Map Boundaries";
            case RendererDebugView::TONE_MAPPING_NONE: return "Tone Mapping-Off";
            case RendererDebugView::TONE_MAPPING_REINHARD: return "Tone Mapping-Reinhard";
            default: return "Unsupported Renderer Type";
        }
    }


}
