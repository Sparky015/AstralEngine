/**
* @file ThreadPool.h
* @author Andrew Fagan
* @date 10/6/25
*/

#pragma once

#include "Core/FixedIntegerTypes.h"

#include <queue>
#include <future>

namespace Astral {

    class ThreadPool
    {
    public:
        ThreadPool();
        ~ThreadPool();

        std::future<void> SubmitTask(const std::function<void()>& callback, float priority, int32 affinity = -1);
        std::future<void> SubmitTask(std::function<void()>&& callback, float priority, int32 affinity = -1);

        struct Task
        {
            std::function<void()> TaskCallback;
            float Priority;
            int32 Affinity;
            size_t TaskID;
            std::promise<void> Promise;

            void Execute();

            bool operator==(const Task& other) const { return TaskID == other.TaskID; }
            std::partial_ordering operator<=>(const Task& other) const { return Priority <=> other.Priority; }
        };


    private:

        void WorkerThreadMain();

        std::atomic_size_t m_NextTaskID;
        std::atomic_size_t m_NextThreadID;
        std::unordered_map<uint32, std::priority_queue<Task>> m_TaskQueues;
        std::mutex m_QueueLock;

        std::vector<std::thread> m_Threads;
        std::unordered_map<std::thread::id, uint32> m_ThreadToQueueIndex;
        std::condition_variable m_RunCondition;
        std::atomic_bool m_StopFlag;
    };

}
