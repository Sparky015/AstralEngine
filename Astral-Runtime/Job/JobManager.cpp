/**
* @file JobManager.cpp
* @author Andrew Fagan
* @date 10/6/25
*/

#include "JobManager.h"

#include "Debug/Utilities/Asserts.h"

namespace Astral {

    bool JobManager::m_IsActive = false;

    JobManager::JobManager()
    {
    }


    JobManager::~JobManager()
    {
    }

    JobManager& JobManager::Get()
    {
        ASSERT(m_IsActive, "Job System is not initialized or has already been shut down!")
        static JobManager m_Instance = JobManager();
        return m_Instance;
    }


    void JobManager::Init()
    {
        m_IsActive = true;
    }


    void JobManager::Shutdown()
    {
        m_IsActive = false;
    }

}
