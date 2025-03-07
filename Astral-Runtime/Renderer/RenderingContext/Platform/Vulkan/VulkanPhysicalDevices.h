/**
* @file VulkanPhysicalDevices.h
* @author Andrew Fagan
* @date 3/6/25
*/

#pragma once

#include <vector>
#include <vulkan/vulkan.h>

#include "Core/CoreMacroDefinitions.h"

namespace Graphics {

    struct VulkanPhysicalDevice
    {
        VkPhysicalDevice physicalDevice;
        VkPhysicalDeviceProperties deviceProperties;
        std::vector<VkQueueFamilyProperties> queueFamilyProperties;
        std::vector<VkBool32> supportsPresent;
        std::vector<VkSurfaceFormatKHR> surfaceFormats;
        VkSurfaceCapabilitiesKHR surfaceCapabilities;
        std::vector<VkPresentModeKHR> presentModes;
    };

    class VulkanPhysicalDevices
    {
    public:
        VulkanPhysicalDevices();
        ~VulkanPhysicalDevices() = default;

        void Init(const VkInstance& instance, const VkSurfaceKHR& surface);

        uint32 SelectDevice(VkQueueFlags requiredQueueType, bool supportsPresent);

        const VulkanPhysicalDevice& SelectedDevice();

    private:
        std::vector<VulkanPhysicalDevice> m_Devices;
        int m_DeviceIndex;

    };

}
