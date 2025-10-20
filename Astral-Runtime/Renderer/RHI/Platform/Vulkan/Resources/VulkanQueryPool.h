/**
* @file VulkanQueryPool.h
* @author Andrew Fagan
* @date 10/16/25
*/

#pragma once

#include "VulkanDevice.h"
#include "Renderer/RHI/Resources/QueryPool.h"

namespace Astral {

    struct VulkanQueryPoolDesc
    {
        VkDevice Device;
        QueryType QueryType;
        uint32 QueryCount;
    };

    class VulkanQueryPool : public QueryPool
    {
    public:
        VulkanQueryPool(const VulkanQueryPoolDesc& createInfo);
        ~VulkanQueryPool() override;

        uint32 GetNumQueries() override { return m_NumQueries; }
        bool GetTimestampQueryResults(std::array<uint64, 2>& outQueryResult) override;
        void ResetPool() override;
        bool NeedsReset() override { return m_NeedsReset; }
        void NotifyReset() override { m_NeedsReset = false; }

        void* GetNativeHandle() override { return m_QueryPool; }



    private:

        VkDevice m_Device;
        VkQueryPool m_QueryPool;
        uint32 m_NumQueries;
        bool m_NeedsReset;
    };

}
