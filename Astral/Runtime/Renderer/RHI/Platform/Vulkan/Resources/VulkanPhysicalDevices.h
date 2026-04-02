/**
* @file VulkanPhysicalDevices.h
* @author Andrew Fagan
* @date 3/6/25
*/

#pragma once

#include "Core/FixedIntegerTypes.h"

#include <vector>
#include <vulkan/vulkan.h>

namespace Astral {

    struct VulkanPhysicalDevice
    {
        VkPhysicalDevice physicalDevice;
        VkPhysicalDeviceProperties deviceProperties;
        std::vector<VkQueueFamilyProperties> queueFamilyProperties;
        std::vector<VkBool32> supportsPresent;
        std::vector<VkSurfaceFormatKHR> surfaceFormats;
        VkSurfaceCapabilitiesKHR surfaceCapabilities;
        VkPhysicalDeviceMemoryProperties memoryProperties;
        std::vector<VkPresentModeKHR> presentModes;
        VkPhysicalDeviceFeatures2 featuresChain;
        VkPhysicalDeviceVulkan12Features features12;
    };

    class VulkanPhysicalDevices
    {
    public:
        VulkanPhysicalDevices();
        ~VulkanPhysicalDevices() = default;

        void Init(const VkInstance& instance, const VkSurfaceKHR& surface);

        uint32 SelectDevice(VkQueueFlags requiredQueueType, VkBool32 supportsPresent);

        VulkanPhysicalDevice& SelectedDevice();
        uint32 SelectedQueueFamily() { return m_QueueFamilyIndex; }

    private:
        std::vector<VulkanPhysicalDevice> m_Devices;
        int m_SelectedDeviceIndex;
        uint32 m_QueueFamilyIndex;

    };

}
