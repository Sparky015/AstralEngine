/**
* @file VulkanQueryPool.cpp
* @author Andrew Fagan
* @date 10/16/25
*/

#include "VulkanQueryPool.h"

#include "Renderer/RHI/Platform/Vulkan/Common/VkEnumConversions.h"
#include "Debug/Utilities/Asserts.h"

namespace Astral {

    VulkanQueryPool::VulkanQueryPool(const VulkanQueryPoolDesc& createInfo) :
        m_Device(createInfo.Device)
    {
        VkQueryPoolCreateInfo vkQueryPoolCreateInfo;
        vkQueryPoolCreateInfo.sType = VK_STRUCTURE_TYPE_QUERY_POOL_CREATE_INFO;
        vkQueryPoolCreateInfo.queryType = ConvertQueryTypeToVkQueryType(createInfo.QueryType);
        vkQueryPoolCreateInfo.queryCount = createInfo.QueryCount;

        vkCreateQueryPool(m_Device, &vkQueryPoolCreateInfo, nullptr, &m_QueryPool);
    }


    VulkanQueryPool::~VulkanQueryPool()
    {
        vkDestroyQueryPool(m_Device, m_QueryPool, nullptr);
    }


    bool VulkanQueryPool::GetTimestampQueryResults(std::pair<uint64, uint64>* queryResult)
    {
        VkResult errorResult = vkGetQueryPoolResults(m_Device, m_QueryPool,
        0, 2,
        sizeof(*queryResult), queryResult, sizeof(uint64),
            VK_QUERY_RESULT_64_BIT | VK_QUERY_RESULT_WITH_AVAILABILITY_BIT);

        if (errorResult == VK_NOT_READY)
        {
            return false;
        }

        ASSERT(errorResult == VK_SUCCESS, "Failed to get query pool results");

        return true;
    }

}

