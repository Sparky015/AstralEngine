//
// Created by Andrew Fagan on 11/27/24.
//

#pragma once

#include "Core/SystemManager.h"
#include "ECS.h"

namespace ECS {

    class ECSManager : SystemManager
    {
    public:
        static ECSManager& Get();
        void Init() override;
        void Shutdown() override;
        void Update() override;

        inline ECS& GetECS() { return m_ECS; }

        ECSManager(const ECSManager&) = delete;
        ECSManager& operator=(const ECSManager&) = delete;
        ECSManager(ECSManager&&) = delete;
        ECSManager& operator=(ECSManager&&) = delete;
    private:
        ECSManager();
        ~ECSManager() override;

        ECS m_ECS;
    };

    extern ECSManager& g_ECSManager;

} // ECS
