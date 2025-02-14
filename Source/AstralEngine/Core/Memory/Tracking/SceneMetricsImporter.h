/**
* @file SceneMetricsImporter.h
* @author Andrew Fagan
* @date 2/8/2025
*/

#pragma once

#include "MemoryMetrics.h"
#include "SceneMetricsStorage.h"

#include <filesystem>
#include <fstream>
#include <string_view>

namespace Core {

    class SceneMetricsImporter
    {
    public:


        bool ImportMemoryProfile(const std::filesystem::path& filePath);

        /**@brief  */
        [[nodiscard]] const SceneMetricsStorage& GetSceneMetrics() const { return m_SceneMetricsStorage; }

    private:

        std::ifstream m_File;
        SceneMetricsStorage m_SceneMetricsStorage;
    };

}
