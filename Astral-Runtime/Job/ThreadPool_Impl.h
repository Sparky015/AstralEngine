/**
* @file ThreadPool_Impl.h
* @author Andrew Fagan
* @date 10/7/25
*/

#pragma once

namespace Astral {

    template<typename ResultType>
    std::future<ResultType> ThreadPool::SubmitTaskWithResult(std::function<ResultType()>&& callback, float priority, int32 affinity)
    {
        std::shared_ptr<std::packaged_task<ResultType()>> packagedTask = std::make_shared<std::packaged_task<ResultType()>>(
            std::move(callback)
        );

        std::future<ResultType> resultFuture = packagedTask->get_future();

        SubmitTask([packagedTask]() {
            (*packagedTask)();
        }, priority, affinity);

        return resultFuture;
    }

}