/**
* @file GraphicsSmartPointers.h
* @author Andrew Fagan
* @date 5/15/2025
*/

#pragma once

#include <memory>

namespace Astral {

    template <typename T>
    using GraphicsRef = std::shared_ptr<T>;

    template <typename T, class... Args>
    inline GraphicsRef<T> CreateGraphicsRef(Args&&... args) { return std::make_shared<T>(std::forward<Args>(args)...); }

    template <typename T>
    using GraphicsWeakRef = std::weak_ptr<T>;

    template <typename T>
    inline GraphicsWeakRef<T> CreateGraphicsWeakRef(GraphicsRef<T> ref) { return std::weak_ptr<T>(ref); }

    template <typename T>
    using GraphicsOwnedPtr = std::unique_ptr<T>;

    template <typename T, class... Args>
    inline GraphicsOwnedPtr<T> CreateGraphicsOwnedPtr(Args&&... args) { return std::make_unique<T>(std::forward<Args>(args)...); }

}