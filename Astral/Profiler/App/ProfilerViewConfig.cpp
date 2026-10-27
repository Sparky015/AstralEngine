/**
* @file ProfilerViewConfig.cpp
* @author Andrew Fagan 
* @date 10/26/26
*/

#include "ProfilerViewConfig.h"

namespace Astral {

    void ProfilerViewConfig::SetPlotLimitsFilterIn(Vec2 plotLimits)
    {
        m_FilterInPlotLimits = plotLimits;
    }


    Vec2 ProfilerViewConfig::GetPlotLimitsFilter()
    {
        return m_FilterInPlotLimits;
    }


    void ProfilerViewConfig::SetThreadIDFilterIn(size_t hashedThreadID)
    {
        m_FilterInThreadID = hashedThreadID;
    }


    size_t ProfilerViewConfig::GetThreadIDFilter()
    {
        return m_FilterInThreadID;
    }


    void ProfilerViewConfig::SetRegionFilterIn(MemoryRegion memoryRegion)
    {
        m_FilterInMemoryRegion = memoryRegion;
    }


    MemoryRegion ProfilerViewConfig::GetRegionFilter()
    {
        return m_FilterInMemoryRegion;
    }


    void ProfilerViewConfig::SetAllocatorFilterIn(MemoryTrackerAllocatorType allocatorType)
    {
        m_FilterInAllocatorType = allocatorType;
    }


    MemoryTrackerAllocatorType ProfilerViewConfig::GetAllocatorFilter()
    {
        return m_FilterInAllocatorType;
    }

}
