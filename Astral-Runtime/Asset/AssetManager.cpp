/**
* @file AssetManager.cpp
* @author Andrew Fagan
* @date 4/12/25
*/

#include "AssetManager.h"

namespace Astral {

    AssetManager& g_AssetManager = AssetManager::Get();

    AssetManager& AssetManager::Get()
    {
        static AssetManager instance;
        return instance;
    }

}
