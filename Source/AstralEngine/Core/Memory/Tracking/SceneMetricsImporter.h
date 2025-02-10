/**
* @file SceneMetricsImporter.h
* @author Andrew Fagan
* @date 2/8/2025
*/

#pragma once

#include <string_view>

namespace Core {

    class SceneMetricsImporter
    {
    public:

        bool ImportMetricsFile(std::string_view filePath);

    private:

    };

}
