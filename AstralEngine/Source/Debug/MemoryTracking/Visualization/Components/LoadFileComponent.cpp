/**
* @file LoadFileComponent.cpp
* @author Andrew Fagan
* @date 2/15/25
*/

#include "LoadFileComponent.h"

#include "../../../Macros/Loggers.h"
#include "imgui.h"

#include <filesystem>
#include <string_view>

#include "Debug/MemoryTracking/Visualization/MemoryDebugWindow.h"
#include "ImGuiFileDialog/ImGuiFileDialog.h"

namespace Core {


    /**@brief Pulls up a window for the user to select a file
     * @param outFilePath Populates the string with the file path of the file that was selected by the user. */
    bool SelectFile(std::string& outFilePath)
    {
        IGFD::FileDialogConfig config;
        config.path = LOG_FILE_DIR;
        ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".ASTLMemProfile", config);


        if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey"))
        {
            if (ImGuiFileDialog::Instance()->IsOk())
            {
                outFilePath = ImGuiFileDialog::Instance()->GetFilePathName();
            }

            ImGuiFileDialog::Instance()->Close();
            return false;
        }

        return true;
    }


    void LoadMemoryProfileButtonComponent()
    {
        static bool showFileSelectMenu = false;
        static bool doLoadFile = false;
        static std::string filePath;

        if (ImGui::Button("Load Memory Profile File"))
        {
            showFileSelectMenu = true;
        }

        if (doLoadFile && !filePath.empty())
        {
            static MemoryDebugWindow memoryDebugWindow;

            LOG(filePath);
            memoryDebugWindow.CloseMemoryDebugWindow();
            memoryDebugWindow.LoadMemoryProfile(filePath);

            doLoadFile = false;
        }
        else if (showFileSelectMenu)
        {
            showFileSelectMenu = SelectFile(filePath);

            if (!showFileSelectMenu)
            {
                doLoadFile = true;
            }
        }

    }




}


