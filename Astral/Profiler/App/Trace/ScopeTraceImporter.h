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

        /**
         * @brief Imports a trace file from disk into scope trace data
         * @param filePath The file path of the trace to import
         * @return True if the import was successful. False if an error occurred during importing.
         */
        bool ImportTrace(const std::filesystem::path& filePath);

        /**
         * @brief Gets the scope traces from an imported trace file
         * @return The scope traces from an imported trace file
         */
        std::vector<ScopeTrace>& GetScopeTraces();

    private:

        std::vector<ScopeTrace> m_ScopeTraces;
    };

}