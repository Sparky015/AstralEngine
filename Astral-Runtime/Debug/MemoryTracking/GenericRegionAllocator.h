/**
* @file RegionAllocator.h
* @author Andrew Fagan
* @date 4/14/25
*/

#pragma once

#include "AllocationData.h"

namespace Astral {

    class GenericRegionAllocator
    {
    public:
        explicit GenericRegionAllocator(Core::MemoryRegion region);

    private:

        Core::MemoryRegion m_Region;
    };

}