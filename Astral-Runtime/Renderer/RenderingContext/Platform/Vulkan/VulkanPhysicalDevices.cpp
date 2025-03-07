/**
* @file VulkanPhysicalDevices.cpp
* @author Andrew Fagan
* @date 3/6/25
*/

#include "VulkanPhysicalDevices.h"

#include "Debug/Utilities/Asserts.h"
#include "Debug/Utilities/Loggers.h"

namespace Graphics {

    VulkanPhysicalDevices::VulkanPhysicalDevices() : m_DeviceIndex(-1) {}


    void VulkanPhysicalDevices::Init(const VkInstance& instance, const VkSurfaceKHR& surface)
    {
        uint32 numberOfDevices = 0;

        VkResult result = vkEnumeratePhysicalDevices(instance, &numberOfDevices, nullptr);
        ASSERT(result == VK_SUCCESS, "vkEnumeratePhysicalDevices failed! (Getting number of devices) Error: " << result);

        m_Devices.resize(numberOfDevices);
        LOG("Vulkan Init: Number of Physical Devices: " << numberOfDevices);

        std::vector<VkPhysicalDevice> deviceHandles;
        deviceHandles.resize(numberOfDevices);

        result = vkEnumeratePhysicalDevices(instance, &numberOfDevices, deviceHandles.data());
        ASSERT(result == VK_SUCCESS, "vkEnumeratePhysicalDevices failed! (Getting device handles) Error: " << result);

        for (uint32 i = 0; i < numberOfDevices; i++)
        {
            VkPhysicalDevice physicalDevice = deviceHandles[i];
            m_Devices[i].physicalDevice = physicalDevice;

            vkGetPhysicalDeviceProperties(physicalDevice, &m_Devices[i].deviceProperties);

            LOG("Device " << i << " name: " << m_Devices[i].deviceProperties.deviceName);

            uint32 apiVersion = m_Devices[i].deviceProperties.apiVersion;
            LOG("Supported API Version: " << VK_API_VERSION_MAJOR(apiVersion) << "."
                                          << VK_API_VERSION_MINOR(apiVersion) << "."
                                          << VK_API_VERSION_PATCH(apiVersion));

            uint32 numberOfQueueFamilies = 0;
            vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &numberOfQueueFamilies, nullptr);

            LOG("Device " << i << " Number of Queue Families: " << numberOfQueueFamilies);
        }
    }


    uint32 VulkanPhysicalDevices::SelectDevice(VkQueueFlags requiredQueueType, bool supportsPresent)
    {

    }


    const VulkanPhysicalDevice& VulkanPhysicalDevices::SelectedDevice()
    {

    }

}
