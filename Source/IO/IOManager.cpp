//
// Created by Andrew Fagan on 10/30/24.
//

#include "IOManager.h"

namespace IO{

    IOManager& g_IOManager = IOManager::Get();

    IOManager& IOManager::Get()
    {
        static IOManager m_Instance = IOManager();
        return m_Instance;
    }


    void IOManager::Init()
    {

    }


    void IOManager::Shutdown()
    {

    }


    void IOManager::Update()
    {

    }

}
