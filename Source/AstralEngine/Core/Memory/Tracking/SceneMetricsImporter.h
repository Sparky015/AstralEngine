/**
* @file SceneMetricsImporter.h
* @author Andrew Fagan
* @date 2/8/2025
*/

#pragma once

#include "SceneMetricsStorage.h"
#include "MemoryMetrics.h"

#include <filesystem>
#include <fstream>
#include <string_view>

namespace Core {

    class SceneMetricsImporter
    {
    public:

        bool ImportMemoryProfile(const std::filesystem::path& filePath);

    private:

        std::ifstream m_File;
        SceneMetricsStorage m_SceneMetricsStorage;
        MemoryMetrics m_TempMemoryMetrics;
    };

}
