/**
* @file TraceImporter.cpp
* @author Andrew Fagan 
* @date 10/13/26
*/

#include "ScopeTraceImporter.h"

#include "nlohmann/json.hpp"

#include <fstream>

namespace Astral {

    void ScopeTraceImporter::ImportTrace(const std::filesystem::path& filePath)
    {
        using json = nlohmann::json;

        std::ifstream f(filePath);
        json data = json::parse(f);

        json& traces = data["traceEvents"];

        m_ScopeTraces.reserve(traces.size());

        for (json& trace : traces)
        {
            ScopeTrace scopeTrace = {};

            scopeTrace.Name = trace["name"];
            scopeTrace.Duration = trace["dur"];
            scopeTrace.Timestamp = trace["ts"];
            scopeTrace.ThreadNumber = trace["tid"];

            m_ScopeTraces.push_back(scopeTrace);
        }
    }


    std::vector<ScopeTrace>& ScopeTraceImporter::GetScopeTraces()
    {
        return m_ScopeTraces;
    }

}
