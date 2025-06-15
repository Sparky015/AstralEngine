//
// Created by Andrew Fagan on 11/27/24.
//

#pragma once

#include "Core/FixedIntegerTypes.h"

#include <string_view>

namespace Astral {

    using EntityID = uint32;
    static constexpr EntityID NULL_ENTITY = 0;

    class Entity
    {
    public:
        Entity() = default;
        explicit Entity(EntityID id, std::string_view debugName = "") : m_ID(id), m_DebugName(debugName) {}

        [[nodiscard]] EntityID GetID() const { return m_ID; }
        [[nodiscard]] std::string_view GetDebugName() const { return m_DebugName; }

    private:

        EntityID m_ID{NULL_ENTITY};
        std::string_view m_DebugName;
    };

}