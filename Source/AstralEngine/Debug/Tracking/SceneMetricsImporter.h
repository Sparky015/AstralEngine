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

        SceneMetricsImporter();
        ~SceneMetricsImporter() = default;

        bool ImportMemoryProfile(const std::filesystem::path& filePath);

        [[nodiscard]] bool HasFileLoaded() const { return m_HasFileLoaded; }

        void CloseMemoryProfile();

        /**@brief  */
        [[nodiscard]] const SceneMetricsStorage& GetSceneMetrics() const { return m_SceneMetricsStorage; }

    private:
        bool m_HasFileLoaded;
        std::ifstream m_File;
        SceneMetricsStorage m_SceneMetricsStorage;
    };

}
