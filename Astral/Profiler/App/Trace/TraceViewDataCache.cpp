/**
* @file TraceViewDataCache.cpp
* @author Andrew Fagan 
* @date 10/13/26
*/

#include "TraceViewDataCache.h"

#include <algorithm>
#include <ranges>
#include "Core/Utilities/Asserts.h"

namespace Astral {

    bool sort(const ScopeTrace& a, const ScopeTrace& b)
    {
        if (a.ThreadNumber == b.ThreadNumber && a.Timestamp == b.Timestamp)
        {
            return a.Duration > b.Duration;
        }
        if (a.ThreadNumber == b.ThreadNumber)
        {
            return a.Timestamp < b.Timestamp;
        }

        return a.ThreadNumber < b.ThreadNumber;
    }

    void TraceViewDataCache::InitializeCache(const std::filesystem::path& traceFilePath)
    {
        m_ScopeTraceImporter.ImportTrace(traceFilePath);

        std::vector<ScopeTrace>& scopeTraces = m_ScopeTraceImporter.GetScopeTraces();
        std::ranges::stable_sort(scopeTraces, sort); // Stable sort because if ThreadNumber and Timestamp and Duration are all the same, then the order that appears in the trace file should take precedence

        m_MinTimestamp = std::ranges::min(scopeTraces, {}, &ScopeTrace::Timestamp).Timestamp;
        m_MaxTimestamp = std::ranges::max(scopeTraces, {}, &ScopeTrace::Timestamp).Timestamp;

        m_TimestampBounds.first = m_MinTimestamp;
        m_TimestampBounds.second = m_MaxTimestamp;


        // Calculate the max overlapping durations for each thread
        std::stack<uint64> overlappingTimestamps;
        size_t maxThreadOverlappingTimestamps = 0;
        uint32 currentThreadNumber = 0;

        for (const ScopeTrace& scopeTrace : scopeTraces)
        {
            if (scopeTrace.Duration == 0) { continue; }

            if (scopeTrace.ThreadNumber != currentThreadNumber)
            {
                m_ThreadMaxOverlappingDurations[currentThreadNumber] = maxThreadOverlappingTimestamps;
                maxThreadOverlappingTimestamps = 0;
                overlappingTimestamps = {};
                currentThreadNumber = scopeTrace.ThreadNumber;
            }

            maxThreadOverlappingTimestamps = std::max(maxThreadOverlappingTimestamps, overlappingTimestamps.size());

            while (!overlappingTimestamps.empty() && overlappingTimestamps.top() <= scopeTrace.Timestamp)
            {
                overlappingTimestamps.pop();
            }

            overlappingTimestamps.push(scopeTrace.Timestamp + scopeTrace.Duration);
        }

    }


    void TraceViewDataCache::InvalidateCache()
    {
        m_ScopeTraceImporter = {};
        m_TimestampBounds = {0, 0};
        m_MinTimestamp = 0;
        m_MaxTimestamp = 0;
    }


    std::vector<ScopeTrace>& TraceViewDataCache::GetScopeTraces()
    {
        return m_ScopeTraceImporter.GetScopeTraces();
    }


    std::pair<uint64, uint64> TraceViewDataCache::GetTraceTimestampBounds()
    {
        return m_TimestampBounds;
    }


    void TraceViewDataCache::SetTraceTimestampBounds(const std::pair<uint64, uint64>& bounds)
    {
        m_TimestampBounds = bounds;
    }


    uint64 TraceViewDataCache::GetMaxTimestamp()
    {
        return m_MaxTimestamp;
    }


    uint64 TraceViewDataCache::GetMinTimestamp()
    {
        return m_MinTimestamp;
    }


    const std::unordered_map<uint32, uint32>& TraceViewDataCache::GetThreadMaxOverlappingDurations()
    {
        return m_ThreadMaxOverlappingDurations;
    }

}

