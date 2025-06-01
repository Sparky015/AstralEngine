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
        explicit GenericRegionAllocator(Astral::MemoryRegion region);

    private:

        Astral::MemoryRegion m_Region;
    };

}