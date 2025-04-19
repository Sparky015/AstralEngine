//
// Created by Andrew Fagan on 11/27/24.
//

#pragma once

#include "Core/SystemManager.h"
#include "ECS.h"

namespace Astral {

    class ECSManager : public SystemManager
    {
    public:
        ECSManager();
        ~ECSManager() override;

        static ECSManager& Get();
        void Init() override;
        void Shutdown() override;

        ECS& GetECS() { return m_ECS; }

        ECSManager(const ECSManager&) = delete;
        ECSManager& operator=(const ECSManager&) = delete;
        ECSManager(ECSManager&&) = delete;
        ECSManager& operator=(ECSManager&&) = delete;

    private:

        ECS m_ECS;
    };

} // ECS
