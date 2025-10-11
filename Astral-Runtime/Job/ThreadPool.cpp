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
        m_StopFlag(false),
        m_TaskQueues(std::thread::hardware_concurrency()),
        m_QueueLocks(std::thread::hardware_concurrency()),
        m_Threads(std::thread::hardware_concurrency()),
        m_RunConditions(std::thread::hardware_concurrency())
    {
        for (size_t i = 0; i < m_Threads.size(); i++)
        {
            m_Threads[i] = std::thread([this, i](){ WorkerThreadMain(i); });
        }
    }


    ThreadPool::ThreadPool(uint32 numThreads) :
        m_NextTaskID(0),
        m_NextThreadID(0),
        m_StopFlag(false),
        m_TaskQueues(numThreads),
        m_QueueLocks(numThreads),
        m_Threads(numThreads),
        m_RunConditions(numThreads)
    {
        for (size_t i = 0; i < m_Threads.size(); i++)
        {
            m_Threads[i] = std::thread([this, i](){ WorkerThreadMain(i); });
        }
    }


    ThreadPool::~ThreadPool()
    {
        m_StopFlag = true;

        for (size_t i = 0; i < m_Threads.size(); i++)
        {
            m_RunConditions[i].notify_all();

            if (m_Threads[i].joinable())
            {
                m_Threads[i].join();
            }
        }
    }


    std::future<void> ThreadPool::SubmitTask(const std::function<void()>& callback, float priority, int32 affinity)
    {
        Task task{};
        task.TaskCallback = callback;
        task.Priority = priority;
        task.Affinity = affinity;
        task.TaskID = m_NextTaskID;
        task.Promise = std::promise<void>();
        m_NextTaskID++;

        if (task.Affinity < 0 || (size_t)task.Affinity >= m_Threads.size())
        {
            task.Affinity = m_NextThreadID;
            m_NextThreadID++;
            if (m_NextThreadID == m_Threads.size()) { m_NextThreadID = 0; }
        }

        std::future<void> taskFuture = task.Promise.get_future();

        std::unique_lock<std::mutex> lock(m_QueueLocks[task.Affinity]);

        std::priority_queue<Task>& taskQueue = m_TaskQueues[task.Affinity];
        taskQueue.push(std::move(task));
        m_RunConditions[task.Affinity].notify_one();

        return taskFuture;
    }


    std::future<void> ThreadPool::SubmitTask(std::function<void()>&& callback, float priority, int32 affinity)
    {
        Task task{};
        task.TaskCallback = std::move(callback);
        task.Priority = priority;
        task.Affinity = affinity;
        task.TaskID = m_NextTaskID;
        task.Promise = std::promise<void>();
        m_NextTaskID++;

        if (task.Affinity < 0 || (size_t)task.Affinity >= m_Threads.size())
        {
            task.Affinity = m_NextThreadID;
            m_NextThreadID++;
            if (m_NextThreadID == m_Threads.size()) { m_NextThreadID = 0; }
        }

        std::future<void> taskFuture = task.Promise.get_future();

        std::unique_lock<std::mutex> lock(m_QueueLocks[task.Affinity]);

        std::priority_queue<Task>& taskQueue = m_TaskQueues[task.Affinity];
        taskQueue.push(std::move(task));
        m_RunConditions[task.Affinity].notify_one();

        return taskFuture;
    }


    void ThreadPool::Task::Execute()
    {
        TaskCallback();
        Promise.set_value();
    }


    void ThreadPool::WorkerThreadMain(uint32 threadIndex)
    {
        while (!m_StopFlag)
        {
            std::unique_lock<std::mutex> lock(m_QueueLocks[threadIndex]);

            std::priority_queue<Task>& taskQueue = m_TaskQueues[threadIndex];

            m_RunConditions[threadIndex].wait(lock, [&](){ return m_StopFlag || !taskQueue.empty(); });

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
