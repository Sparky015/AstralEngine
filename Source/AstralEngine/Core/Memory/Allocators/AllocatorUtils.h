/**
* @file AllocatorUtils.h
* @author Andrew Fagan
* @date 1/28/25
*/

#pragma once

namespace Core::AllocatorUtils {

    [[nodiscard]] constexpr bool IsAlignmentPowerOfTwo(const size_t alignment) noexcept
    {
        return alignment & alignment - 1 == 0;
    }

}