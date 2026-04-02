/**
* @file Ref.h
* @author Andrew Fagan
* @date 7/29/2025
*/


#pragma once

#include "Debug/MemoryTracking/GenericRegionAllocator.h"

#include <memory>

namespace Astral {

    template <typename T>
    using Ref = std::shared_ptr<T>;

    template <typename T, class... Args>
    inline Ref<T> CreateRef(Args&&... args)
    {
        return std::allocate_shared<T>(Astral::GenericRegionAllocator<T>{Astral::MemoryRegion::ASSETS}, std::forward<Args>(args)...);
    }

}
