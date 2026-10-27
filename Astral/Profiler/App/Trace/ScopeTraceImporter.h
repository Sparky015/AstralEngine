/**
* @file TraceImporter.h
* @author Andrew Fagan 
* @date 10/13/26
*/

#pragma once

#include "Core/FixedIntegerTypes.h"

#include <filesystem>
#include <string>
#include <vector>

namespace Astral {

    struct ScopeTrace
    {
        std::string Name;
        uint64 Duration;
        uint64 Timestamp;
        uint32 ThreadNumber;
    };

    class ScopeTraceImporter
    {
    public:

        void ImportTrace(const std::filesystem::path& filePath);

        std::vector<ScopeTrace>& GetScopeTraces();

    private:

        std::vector<ScopeTrace> m_ScopeTraces;
    };

}