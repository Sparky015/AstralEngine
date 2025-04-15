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
        AssetManager() = default;
        ~AssetManager() = default;

        static AssetManager& Get();

        void Init() override {}
        void Shutdown() override {}

        AssetRegistry& GetRegistry() { return m_Registry; }

        AssetManager(const AssetManager&) = delete;
        AssetManager& operator=(const AssetManager&) = delete;
        AssetManager(AssetManager&&) = delete;
        AssetManager& operator=(AssetManager&&) = delete;

    private:

        AssetRegistry m_Registry;
    };

    extern AssetManager& g_AssetManager;

}