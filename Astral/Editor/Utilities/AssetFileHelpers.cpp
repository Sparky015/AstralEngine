/**
* @file AssetFileHelpers.cpp
* @author Andrew Fagan
* @date 7/23/25
*/

#include "AssetFileHelpers.h"

#include "nfd.h"

namespace Astral {

        void GetAssetFilePath(AssetRegistry& registry, AssetID materialAssetID, char* inputBuffer, int inputBufferSize)
    {
        std::string filePath;
        if (materialAssetID != NullAssetID)
        {
            filePath = registry.GetFilePathFromAssetID(materialAssetID).string();
        }
        else
        {
            filePath = "";
        }
        strncpy(inputBuffer, filePath.c_str(), inputBufferSize - 1);
        inputBuffer[inputBufferSize - 1] = '\0'; // Ensure null-termination
    }


    void SelectFileFromDialog(std::string& outFilePath, std::string_view filterName, std::string_view filterSpec)
    {
        nfdu8char_t* outPath;
        nfdu8filteritem_t filters[1] = { { filterName.data(), filterSpec.data() }};
        nfdopendialogu8args_t args = {0};
        args.filterList = filters;
        args.filterCount = 1;
        // args.defaultPath = registry.GetAssetDirectoryPath().string().c_str();

        nfdresult_t result = NFD_OpenDialogU8_With(&outPath, &args);

        if (result == NFD_OKAY)
        {
            outFilePath = std::string(outPath);
            NFD_FreePathU8(outPath);
        }
        else if (result == NFD_CANCEL)
        {
            outFilePath = "";
            AE_LOG("Open File Dialog Canceled")
        }
        else if (result == NFD_ERROR)
        {
            outFilePath = "";
            AE_WARN("NFD Error: " << NFD_GetError())
        }
    }


    void SelectFileFromDialog(std::filesystem::path& outFilePath, std::string_view filterName,
        std::string_view filterSpec)
    {
        nfdu8char_t* outPath;
        nfdu8filteritem_t filters[1] = { { filterName.data(), filterSpec.data() }};
        nfdopendialogu8args_t args = {0};
        args.filterList = filters;
        args.filterCount = 1;

        if (filterSpec == "")
        {
            args.filterCount = 0;
        }

        nfdresult_t result = NFD_OpenDialogU8_With(&outPath, &args);

        if (result == NFD_OKAY)
        {
            outFilePath = std::string(outPath);
            NFD_FreePathU8(outPath);
        }
        else if (result == NFD_CANCEL)
        {
            outFilePath = "";
            AE_LOG("Open File Dialog Canceled")
        }
        else if (result == NFD_ERROR)
        {
            outFilePath = "";
            AE_WARN("NFD Error: " << NFD_GetError())
        }
    }

}
