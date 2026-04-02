/**
* @file JobManager.h
* @author Andrew Fagan
* @date 10/6/25
*/

#pragma once

#include "ThreadPool.h"
#include "Core/SystemManager.h"

namespace Astral {

    class JobManager : public SystemManager
    {
    public:
        explicit JobManager();
        ~JobManager() override;

        static JobManager& Get();

        void Init() override;
        void Shutdown() override;

        ThreadPool& GetThreadPool() { return *m_ThreadPool; }

        JobManager(JobManager&&) = delete;
        JobManager& operator=(JobManager&&) = delete;
        JobManager(const JobManager&) = delete;
        JobManager& operator=(const JobManager&) = delete;

    private:

        static bool m_IsActive;
        std::unique_ptr<ThreadPool> m_ThreadPool;
    };

}
