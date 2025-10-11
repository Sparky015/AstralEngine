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

    /**
     * @class ThreadPool
     * @brief Defines a set of reusable threads
     */
    class ThreadPool
    {
    public:

        /**
         * @brief Constructs a pool of threads with the size of the number of hardware threads available
         */
        ThreadPool();

        /**
         * @brief Constructs a pool of threads with the defined number of threads
         * @param numThreads The number of threads to fill the thread pool with
         */
        ThreadPool(uint32 numThreads);

        /**
         * @brief Shuts down and destroys thread pool
         */
        ~ThreadPool();

        /**
         * @brief Submits a task to the thread pool to execute
         * @param callback The function to execute for the task
         * @param priority The priority of the task relative to other submitted tasks
         * @param affinity The affinity of the task to a specific thread in the pool
         */
        std::future<void> SubmitTask(const std::function<void()>& callback, float priority, int32 affinity = -1);

        /**
         * @brief Submits a task to the thread pool to execute
         * @param callback The function to execute for the task
         * @param priority The priority of the task relative to other submitted tasks
         * @param affinity The affinity of the task to a specific thread in the pool
         */
        std::future<void> SubmitTask(std::function<void()>&& callback, float priority, int32 affinity = -1);

        /**
        * @brief Submits a task to the thread pool to execute
        * @param callback The function to execute for the task
        * @param priority The priority of the task relative to other submitted tasks
        * @param affinity The affinity of the task to a specific thread in the pool
        */
        template <typename ResultType>
        std::future<ResultType> SubmitTaskWithResult(std::function<ResultType()>&& callback, float priority, int32 affinity = -1);

        /**
         * @struct Task
         * @brief Defines a task/job that a thread pool can execute
         */
        struct Task
        {
            std::function<void()> TaskCallback;
            float Priority;
            int32 Affinity;
            size_t TaskID;
            std::promise<void> Promise;

            void Execute();

            Task() = default;
            ~Task() = default;

            Task(const Task&) = delete;
            Task& operator=(const Task&) = delete;
            Task(Task&&) = default;
            Task& operator=(Task&&) = default;

            bool operator==(const Task& other) const { return TaskID == other.TaskID; }
            std::partial_ordering operator<=>(const Task& other) const { return Priority <=> other.Priority; }
        };


    private:

        /**
         * @brief The base run loop function of a worker thread
         * @param threadIndex The index of the thread
         */
        void WorkerThreadMain(uint32 threadIndex);

        std::atomic_size_t m_NextTaskID;
        std::atomic_size_t m_NextThreadID;
        std::atomic_bool m_StopFlag;

        std::vector<std::priority_queue<Task>> m_TaskQueues;
        std::vector<std::mutex> m_QueueLocks;
        std::vector<std::thread> m_Threads;
        std::vector<std::condition_variable> m_RunConditions;
    };

}

#include "ThreadPool_Impl.h"