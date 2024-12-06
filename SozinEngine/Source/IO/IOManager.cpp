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
        PROFILE_SCOPE();
        TRACE("Initializing IO Manager!")
        m_UpdateListener.StartListening();
    }


    void IOManager::Shutdown()
    {
        PROFILE_SCOPE();
        TRACE("Shutting down IO Manager!")
        m_UpdateListener.StopListening();
    }


    void IOManager::Update()
    {
        PROFILE_SCOPE();
    }


    IOManager::IOManager()
    {
        PROFILE_SCOPE();
        TRACE("Constructing IO System!")
    }

    IOManager::~IOManager()
    {
        PROFILE_SCOPE();
        TRACE("Destroying IO System!")
    }

}
