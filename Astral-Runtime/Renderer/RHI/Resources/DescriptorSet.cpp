/**
* @file DescriptorSet.cpp
* @author Andrew Fagan
* @date 6/20/2025
*/

#include "DescriptorSet.h"

#include "Device.h"
#include "Core/Engine.h"
#include "Renderer/RHI/RendererCommands.h"
#include "Renderer/RendererManager.h"

namespace Astral {

    bool DescriptorSetLayout::operator==(const DescriptorSetLayout& other) const
    {
        return Descriptors == other.Descriptors;
    }


    GraphicsRef<DescriptorSet> DescriptorSet::CreateDescriptorSet()
    {
        Device& device = Engine::Get().GetRendererManager().GetContext().GetDevice();

        switch (RendererCommands::GetAPI())
        {
            case API::Vulkan: return device.CreateDescriptorSet();
            case API::DirectX12: ASTRAL_ERROR("DirectX12 is not supported yet!");
            case API::Metal: ASTRAL_ERROR("Metal is not supported yet!");
            default: ASTRAL_ERROR("Invalid Renderer API");
        }
    }

}
