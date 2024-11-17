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
        TRACE("Initializing IO Manager!")
    }


    void IOManager::Shutdown()
    {
        TRACE("Shutting down IO Manager!")
    }


    void IOManager::Update()
    {

    }


    IOManager::IOManager()
    {
        TRACE("Constructing IO System!")
    }

    IOManager::~IOManager()
    {
        TRACE("Destroying IO System!")
    }

}
