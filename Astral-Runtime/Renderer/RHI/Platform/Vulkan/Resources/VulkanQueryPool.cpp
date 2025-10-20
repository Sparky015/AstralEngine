/**
* @file VulkanQueryPool.cpp
* @author Andrew Fagan
* @date 10/16/25
*/

#include "VulkanQueryPool.h"

#include "Renderer/RHI/Platform/Vulkan/Common/VkEnumConversions.h"
#include "Debug/Utilities/Asserts.h"
#include "Debug/Utilities/Loggers.h"

namespace Astral {

    VulkanQueryPool::VulkanQueryPool(const VulkanQueryPoolDesc& createInfo) :
        m_Device(createInfo.Device),
        m_NumQueries(createInfo.QueryCount),
        m_NeedsReset(true)
    {
        VkQueryPoolCreateInfo vkQueryPoolCreateInfo{};
        vkQueryPoolCreateInfo.sType = VK_STRUCTURE_TYPE_QUERY_POOL_CREATE_INFO;
        vkQueryPoolCreateInfo.queryType = ConvertQueryTypeToVkQueryType(createInfo.QueryType);
        vkQueryPoolCreateInfo.queryCount = createInfo.QueryCount;

        vkCreateQueryPool(m_Device, &vkQueryPoolCreateInfo, nullptr, &m_QueryPool);
    }


    VulkanQueryPool::~VulkanQueryPool()
    {
        vkDestroyQueryPool(m_Device, m_QueryPool, nullptr);
    }


    bool VulkanQueryPool::GetTimestampQueryResults(std::array<uint64, 2>& outQueryResult)
    {
        VkResult errorResult = vkGetQueryPoolResults(m_Device, m_QueryPool,
        0, 2,
        sizeof(std::array<uint64, 2>), outQueryResult.data(), sizeof(uint64),
            VK_QUERY_RESULT_64_BIT);

        if (errorResult == VK_NOT_READY)
        {
            return false;
        }

        ASSERT(errorResult == VK_SUCCESS, "Failed to get query pool results");

        m_NeedsReset = true;

        return true;
    }


    void VulkanQueryPool::ResetPool()
    {
        vkResetQueryPool(m_Device, m_QueryPool, 0, m_NumQueries);
        m_NeedsReset = false;
    }

}

