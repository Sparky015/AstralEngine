/**
* @file TraceViewDataCache.h
* @author Andrew Fagan 
* @date 10/13/26
*/

#pragma once

#include "ScopeTraceImporter.h"

#include "Core/Math/Math.h"

namespace Astral {

    class TraceViewDataCache
    {
    public:

        /**
         * @brief Initalizes the cache with the trace at the given filepath
         * @param traceFilePath The file path to the trace file
         */
        void InitializeCache(const std::filesystem::path& traceFilePath);

        /**
         * @brief Invalidates the trace viewer cache
         */
        void InvalidateCache();

        /**
         * @brief Gets the scope traces of the current imported trace file
         * @return The scope traces of the imported trace file
         * @note Can be empty if no trace file has been imported or if the cache was invalidated
         */
        std::vector<ScopeTrace>& GetScopeTraces();

        /**
         * @brief Gets the trace viewer's timestamp boundaries
         * @return The trace viewer's timestamp boundaries
         */
        std::pair<uint64, uint64> GetTraceTimestampBounds();

        /**
         * @brief Sets the trace viewer's timestamp boundaries
         * @param bounds The new bounds to set
         */
        void SetTraceTimestampBounds(const std::pair<uint64, uint64>& bounds);

        /**
         * @brief Gets the highest timestamp value in the data
         * @return The highest timestamp value in the data
         */
        uint64 GetMaxTimestamp();

        /**
         * @brief Gets the lowest timestamp value in the data
         * @return The lowest timestamp value in the data
         */
        uint64 GetMinTimestamp();

        /**
         * @brief Gets the max overlapping durations for each thread
         * @return The max overlapping durations for each thread. The key is the thread number.
         * A thread number may not be contained in the hashmap if it is not in the data.
         */
        const std::unordered_map<uint32, uint32>& GetThreadMaxOverlappingDurations();

    private:

        ScopeTraceImporter m_ScopeTraceImporter;
        std::pair<uint64, uint64> m_TimestampBounds;
        uint64 m_MaxTimestamp = 0;
        uint64 m_MinTimestamp = 0;
        std::unordered_map<uint32, uint32> m_ThreadMaxOverlappingDurations;
    };

}