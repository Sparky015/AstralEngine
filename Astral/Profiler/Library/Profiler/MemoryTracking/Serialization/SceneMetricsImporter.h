/**
* @file SceneMetricsImporter.h
* @author Andrew Fagan
* @date 2/8/2025
*/

#pragma once

#include "Profiler/MemoryTracking/MemoryMetrics.h"
#include "SceneMetricsStorage.h"

#include <filesystem>
#include <fstream>
#include <string_view>

namespace Astral {

    class SceneMetricsImporter
    {
    public:

        SceneMetricsImporter();
        ~SceneMetricsImporter() = default;

        bool ImportMemoryProfile(const std::filesystem::path& filePath);

        [[nodiscard]] bool HasFileLoaded() const { return m_HasFileLoaded; }

        void CloseMemoryProfile();

        /**
         * @brief Gets the scene metrics of the imported scene
         * @return The scene metrics of the imported scene
         */
        [[nodiscard]] const SceneMetricsStorage& GetSceneMetrics() const { return m_SceneMetricsStorage; }

        /**
         * @brief Gets the scene starting time point of the imported scene
         * @return The scene starting time point of the imported scene
         */
        [[nodiscard]] uint64 GetSceneStartingTimePoint() const { return m_SceneStartingTimePoint; }

    private:
        bool m_HasFileLoaded;
        std::ifstream m_File;
        SceneMetricsStorage m_SceneMetricsStorage;
        uint64 m_SceneStartingTimePoint;
    };

}
