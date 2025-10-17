/**
* @file QueryPool.h
* @author Andrew Fagan
* @date 10/16/25
*/

#pragma once

#include "Core/FixedIntegerTypes.h"

namespace Astral {

    enum class QueryType
    {
        TIMESTAMP
    };

    class QueryPool
    {
    public:
        QueryPool() = default;
        virtual ~QueryPool() = default;

        virtual uint32 GetNumQueries() = 0;
        virtual bool GetTimestampQueryResults(std::pair<uint64, uint64>* queryResult) = 0;

        virtual void* GetNativeHandle() = 0;

    private:

    };

    using QueryPoolHandle = GraphicsRef<QueryPool>;

}