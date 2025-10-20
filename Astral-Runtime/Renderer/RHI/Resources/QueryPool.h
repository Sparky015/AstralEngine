/**
* @file QueryPool.h
* @author Andrew Fagan
* @date 10/16/25
*/

#pragma once

#include "Core/FixedIntegerTypes.h"
#include "Renderer/RHI/Common/GraphicsSmartPointers.h"

#include <array>

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
        virtual bool GetTimestampQueryResults(std::array<uint64, 2>& outQueryResult) = 0;
        virtual void ResetPool() = 0;
        virtual bool NeedsReset() = 0;
        virtual void NotifyReset() = 0;

        virtual void* GetNativeHandle() = 0;

    private:

    };

    using QueryPoolHandle = GraphicsRef<QueryPool>;

}