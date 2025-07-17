/**
* @file AssetManager.h
* @author Andrew Fagan
* @date 4/12/25
*/

#pragma once

#include "Core/SystemManager.h"
#include "AssetRegistry.h"

namespace Astral {

    class AssetManager : public SystemManager
    {
    public:
        AssetManager();
        ~AssetManager() = default;

        void Init() override;
        void Shutdown() override;

        AssetRegistry& GetRegistry() { return *m_Registry; }

        AssetManager(const AssetManager&) = delete;
        AssetManager& operator=(const AssetManager&) = delete;
        AssetManager(AssetManager&&) = delete;
        AssetManager& operator=(AssetManager&&) = delete;

    private:

        ScopedPtr<AssetRegistry> m_Registry;
    };

}