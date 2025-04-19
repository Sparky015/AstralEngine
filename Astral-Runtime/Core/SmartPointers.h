/**
* @file SmartPointers.h
* @author Andrew Fagan
* @date 4/16/25
*/

#pragma once

#include <memory>

namespace Astral {

    template <typename T>
    using Ref = std::shared_ptr<T>;

    template <typename T, class... Args>
    inline Ref<T> CreateRef(Args&&... args) { return std::make_shared<T>(std::forward<Args>(args)...); }

    template <typename T>
    using ScopedPtr = std::unique_ptr<T>;

    template <typename T, class... Args>
    inline ScopedPtr<T> CreateScopedPtr(Args&&... args) { return std::make_unique<T>(std::forward<Args>(args)...); }

}
