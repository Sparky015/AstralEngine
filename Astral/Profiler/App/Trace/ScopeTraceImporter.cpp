/**
* @file TraceImporter.cpp
* @author Andrew Fagan 
* @date 10/13/26
*/

#include "ScopeTraceImporter.h"

#include "nlohmann/json.hpp"

#include <fstream>

#include "Core/Utilities/Loggers.h"

namespace Astral {

    bool ScopeTraceImporter::ImportTrace(const std::filesystem::path& filePath)
    {
        using json = nlohmann::json;

        std::ifstream f(filePath);

        try
        {
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
        catch (std::exception& e)
        {
            AE_WARN("Error occured during trace import! Error message: " << e.what());
            return false;
        }

        return true;
    }


    std::vector<ScopeTrace>& ScopeTraceImporter::GetScopeTraces()
    {
        return m_ScopeTraces;
    }

}
