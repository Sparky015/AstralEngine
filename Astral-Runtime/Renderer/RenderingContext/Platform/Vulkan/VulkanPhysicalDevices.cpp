/**
* @file VulkanPhysicalDevices.cpp
* @author Andrew Fagan
* @date 3/6/25
*/

#include "VulkanPhysicalDevices.h"

#include "Debug/Utilities/Asserts.h"
#include "Debug/Utilities/Error.h"
#include "Debug/Utilities/Loggers.h"

namespace Graphics {

    VulkanPhysicalDevices::VulkanPhysicalDevices() : m_SelectedDeviceIndex(-1) {}


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

            m_Devices[i].queueFamilyProperties.resize(numberOfQueueFamilies);
            m_Devices[i].supportsPresent.resize(numberOfQueueFamilies);

            vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &numberOfQueueFamilies, m_Devices[i].queueFamilyProperties.data());

            for (uint32 j = 0; j < numberOfQueueFamilies; j++)
            {
                const VkQueueFamilyProperties& queueFamilyProperties = m_Devices[i].queueFamilyProperties[j];

                LOG("   Queue Family " << j << " has " << queueFamilyProperties.queueCount << " queues")

                VkQueueFlags queueFlags = queueFamilyProperties.queueFlags;
                LOG("Queue Flags: "
                    << "\nGFX: " << ((queueFlags & VK_QUEUE_GRAPHICS_BIT) ? "Yes" : "No")
                    << "\nCompute: " << ((queueFlags & VK_QUEUE_COMPUTE_BIT) ? "Yes" : "No")
                    << "\nTransfer: " << ((queueFlags & VK_QUEUE_TRANSFER_BIT) ? "Yes" : "No")
                    << "\nSparse Binding: " << ((queueFlags & VK_QUEUE_SPARSE_BINDING_BIT) ? "Yes" : "No") << "\n")

                result = vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, j, surface, &m_Devices[i].supportsPresent[j]);
                ASSERT(result == VK_SUCCESS, "Vulkan error trying to detect physical device surface support!")
            }

            uint32 numberOfSurfaceFormats = 0;
            result = vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &numberOfSurfaceFormats, nullptr);
            ASSERT(result == VK_SUCCESS, "vkGetPhysicalDeviceSurfaceFormatsKHR failed! (Getting number of devices) Error: " << result)

            m_Devices[i].surfaceFormats.resize(numberOfSurfaceFormats);

            result = vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &numberOfSurfaceFormats, m_Devices[i].surfaceFormats.data());
            ASSERT(result == VK_SUCCESS, "vkGetPhysicalDeviceSurfaceFormatsKHR failed! (Getting surface formats) Error: " << result)

            // This for loop is only for logging, delete later
            for (uint32 j = 0; j < numberOfSurfaceFormats; j++)
            {
                const VkSurfaceFormatKHR& surfaceFormat = m_Devices[i].surfaceFormats[j];
                LOG("   Format " << surfaceFormat.format << " Color Space: " << surfaceFormat.colorSpace)
            }

            result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &m_Devices[i].surfaceCapabilities);
            ASSERT(result == VK_SUCCESS, "vkGetPhysicalDeviceSurfaceCapabilitiesKHR failed getting! Error code: " << result);

            uint32 numberOfPresentModes = 0;
            result = vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &numberOfPresentModes, nullptr);
            ASSERT(result == VK_SUCCESS, "vkGetPhysicalDeviceSurfaceCapabilitiesKHR failed getting number of present modes! Error code: " << result)

            m_Devices[i].presentModes.resize(numberOfPresentModes);

            result = vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &numberOfPresentModes, m_Devices[i].presentModes.data());
            ASSERT(result == VK_SUCCESS, "vkGetPhysicalDeviceSurfaceCapabilitiesKHR failed getting present modes data! Error code: " << result)

            vkGetPhysicalDeviceMemoryProperties(physicalDevice, &m_Devices[i].memoryProperties);

            LOG("Number of Memory Types: ")
            for (uint32 j = 0; j < m_Devices[i].memoryProperties.memoryTypeCount; j++)
            {
                LOG(j << ": flags " << m_Devices[i].memoryProperties.memoryTypes[j].propertyFlags
                      << ": heap " << m_Devices[i].memoryProperties.memoryTypes[j].heapIndex)
            }

            vkGetPhysicalDeviceFeatures(m_Devices[i].physicalDevice, &m_Devices[i].features);
        }
    }


    uint32 VulkanPhysicalDevices::SelectDevice(VkQueueFlags requiredQueueType, bool supportsPresent)
    {
        ASSERT(!m_Devices.empty(), "Vulkan physical device(s) data has not been read in yet! Call Init()!");

        for (uint32 deviceIndex = 0; deviceIndex < m_Devices.size(); deviceIndex++)
        {
            for (uint32 queueFamilyIndex = 0; queueFamilyIndex < m_Devices[deviceIndex].queueFamilyProperties.size(); queueFamilyIndex++)
            {
                const VkQueueFamilyProperties queueFamilyProperties = m_Devices[deviceIndex].queueFamilyProperties[queueFamilyIndex];

                if ((queueFamilyProperties.queueFlags & requiredQueueType) && ((bool)m_Devices[deviceIndex].supportsPresent[queueFamilyIndex] == supportsPresent))
                {
                    m_SelectedDeviceIndex = deviceIndex;
                    LOG("Vulkan: Using device " << deviceIndex << " and queue family " << queueFamilyIndex)
                    return deviceIndex;
                }
            }
        }

        ASTRAL_ERROR("Vulkan: No device found with desired properties!");
    }


    const VulkanPhysicalDevice& VulkanPhysicalDevices::SelectedDevice()
    {
        ASSERT(m_SelectedDeviceIndex != -1, "Vulkan physical device has not been selected yet! Call SelectDevice first!");
        return m_Devices[m_SelectedDeviceIndex];
    }

}
