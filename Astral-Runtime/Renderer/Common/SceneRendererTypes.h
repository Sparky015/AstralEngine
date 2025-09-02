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

    struct RendererSettings
    {
        RendererType RendererType;
        bool IsVSyncEnabled;
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
