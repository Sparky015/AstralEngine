/**
* @file LoadFileComponent.cpp
* @author Andrew Fagan
* @date 2/15/25
*/

#include "LoadFileComponent.h"

#include "Debug/Utilities/Loggers.h"
#include "imgui.h"

#include <filesystem>
#include <string_view>

#include "Debug/MemoryTracking/Visualization/MemoryDebugWindow.h"
#include "nfd.hpp"

namespace Astral {


    /**@brief Pulls up a window for the user to select a file
     * @param outFilePath Populates the string with the file path of the file that was selected by the user. */
    void SelectFile(std::string& outFilePath)
    {
        nfdu8char_t* outPath;
        nfdu8filteritem_t filters[1] = { { "Astral Memory Profile", "ASTLMemProfile" } };
        nfdopendialogu8args_t args = {0};
        args.filterList = filters;
        args.filterCount = 1;

        nfdresult_t result = NFD_OpenDialogU8_With(&outPath, &args);

        if (result == NFD_OKAY)
        {
            outFilePath = std::string(outPath);
            NFD_FreePathU8(outPath);
        }
        else if (result == NFD_CANCEL)
        {
            outFilePath = "";
        }
    }


    void LoadMemoryProfileButtonComponent()
    {
        static bool showFileSelectMenu = false;
        static std::string filePath;

        if (ImGui::Button("Load Memory Profile File"))
        {
            showFileSelectMenu = true;
        }

        if (showFileSelectMenu)
        {
            SelectFile(filePath);

            if (!filePath.empty())
            {
                static MemoryDebugWindow memoryDebugWindow;

                AE_LOG(filePath);
                memoryDebugWindow.CloseMemoryDebugWindow();
                memoryDebugWindow.LoadMemoryProfile(filePath);
            }

            showFileSelectMenu = false;
        }
    }

}


