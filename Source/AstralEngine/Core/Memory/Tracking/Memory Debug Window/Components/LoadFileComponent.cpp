/**
* @file LoadFileComponent.cpp
* @author Andrew Fagan
* @date 2/15/25
*/

#include "LoadFileComponent.h"

#include "Debug/Macros/Loggers.h"
#include "imgui.h"

#include "Core/Memory/Tracking/Memory Debug Window/MemoryDebugWindow.h"
#include "ImGuiFileDialog/ImGuiFileDialog.h"

namespace Core {

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

}


