/**
* @file AssetManager.cpp
* @author Andrew Fagan
* @date 4/12/25
*/

#include "AssetManager.h"

namespace Astral {

    AssetManager::AssetManager() :
        m_Registry(nullptr)
    {
    }


    void AssetManager::Init()
    {
        m_Registry = CreateScopedPtr<AssetRegistry>();
    }


    void AssetManager::Shutdown()
    {
        m_Registry.reset();
    }

}
