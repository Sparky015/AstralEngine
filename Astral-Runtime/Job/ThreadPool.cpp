/**
* @file ThreadPool.cpp
* @author Andrew Fagan
* @date 10/6/25
*/

#include "ThreadPool.h"


namespace Astral {


    ThreadPool::ThreadPool() :
        m_NextTaskID(0),
        m_NextThreadID(0),
        m_Threads(std::thread::hardware_concurrency()),
        m_StopFlag(false)
    {
        for (size_t i = 0; i < m_Threads.size(); i++)
        {
            const std::thread::id& threadID = m_Threads[i].get_id();
            m_ThreadToQueueIndex[threadID] = i;
            m_Threads[i] = std::thread(&ThreadPool::WorkerThreadMain, this);
        }
    }


    ThreadPool::~ThreadPool()
    {
        m_StopFlag = true;
        m_RunCondition.notify_all();
    }


    std::future<void> ThreadPool::SubmitTask(const std::function<void()>& callback, float priority, int32 affinity)
    {
        Task task = {
            .TaskCallback = callback,
            .Priority = priority,
            .Affinity = affinity,
            .TaskID = m_NextTaskID,
            .Promise = std::promise<void>(),
        };

        m_NextTaskID++;

        if (task.Affinity == -1)
        {
            task.Affinity = m_NextThreadID;
            m_NextThreadID++;
            if (m_NextThreadID == m_Threads.size()) { m_NextThreadID = 0; }
        }

        std::future<void> taskFuture = task.Promise.get_future();

        std::priority_queue<Task>& taskQueue = m_TaskQueues[task.Affinity];
        taskQueue.push(std::move(task));
        m_RunCondition.notify_one();

        return taskFuture;
    }


    std::future<void> ThreadPool::SubmitTask(std::function<void()>&& callback, float priority, int32 affinity)
    {
        Task task = {
            .TaskCallback = callback,
            .Priority = priority,
            .Affinity = affinity,
            .TaskID = m_NextTaskID,
            .Promise = std::promise<void>(),
        };

        m_NextTaskID++;

        if (task.Affinity == -1)
        {
            task.Affinity = m_NextThreadID;
            m_NextThreadID++;
            if (m_NextThreadID == m_Threads.size()) { m_NextThreadID = 0; }
        }

        std::future<void> taskFuture = task.Promise.get_future();

        std::priority_queue<Task>& taskQueue = m_TaskQueues[task.Affinity];
        taskQueue.push(std::move(task));
        m_RunCondition.notify_one();

        return taskFuture;
    }


    void ThreadPool::Task::Execute()
    {
        TaskCallback();
        Promise.set_value();
    }


    void ThreadPool::WorkerThreadMain()
    {
        while (!m_StopFlag)
        {
            std::unique_lock<std::mutex> lock(m_QueueLock);
            uint32 queueIndex = m_ThreadToQueueIndex[std::this_thread::get_id()];
            std::priority_queue<Task>& taskQueue = m_TaskQueues[queueIndex];

            m_RunCondition.wait(lock, [&](){ return m_StopFlag || !taskQueue.empty(); });

            if (m_StopFlag && taskQueue.empty())
            {
                break;
            }

            Task task = std::move(const_cast<Task&>(taskQueue.top()));
            taskQueue.pop();
            lock.unlock();

            task.Execute();
        }
    }

}
