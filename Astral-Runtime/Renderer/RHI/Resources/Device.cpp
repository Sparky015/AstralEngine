/**
* @file Device.cpp
* @author Andrew Fagan
* @date 5/13/25
*/

#include "Device.h"

#include "Renderer/RHI/RendererAPI.h"
#include "Debug/Utilities/Error.h"

namespace Astral {

    Device* Device::CreateDevice()
    {
        switch (RendererAPI::GetAPI())
        {
            case API::OpenGL: ASTRAL_ERROR("OpenGL is not supported yet!");
            case API::Vulkan: ASTRAL_ERROR("Vulkan is not supported yet!");
            case API::DirectX12: ASTRAL_ERROR("DirectX12 is not supported yet!");
            case API::Metal: ASTRAL_ERROR("Metal is not supported yet!");
            default: ASTRAL_ERROR("Invalid Renderer API");
        }
    }

}