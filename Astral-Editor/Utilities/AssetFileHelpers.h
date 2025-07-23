/**
* @file AssetFileHelpers.h
* @author Andrew Fagan
* @date 7/23/25
*/

#pragma once

#include "Asset/AssetRegistry.h"

namespace Astral {

    void GetAssetFilePath(AssetRegistry& registry, AssetID materialAssetID, char* inputBuffer, int inputBufferSize);
    void SelectFileFromDialog(std::string& outFilePath, std::string_view filterName, std::string_view filterSpec);
    void SelectFileFromDialog(std::filesystem::path& outFilePath, std::string_view filterName, std::string_view filterSpec);

}
