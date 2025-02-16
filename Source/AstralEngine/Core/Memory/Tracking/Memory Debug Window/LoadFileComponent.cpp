/**
* @file LoadFileComponent.cpp
* @author Andrew Fagan
* @date 2/15/25
*/

#include "LoadFileComponent.h"

#include "Debug/Macros/Loggers.h"
#include "imgui.h"

#include "ImGuiFileDialog/ImGuiFileDialog.h"
#include "Core/Memory/Tracking/Memory Debug Window/MemoryDebugWindow.h"

namespace Core {

    void LoadMemoryProfileButtonComponent()
    {
        static bool isOn = false;
        static std::string filePath;

        if (ImGui::Checkbox("Load Memory Profile File", &isOn))
        {
            LOG("Test");
        }

        if (isOn)
        {
            isOn = SelectFile(filePath);

            if (!isOn)
            {
                static MemoryDebugWindow memoryDebugWindow;
                // Spawn Memory Debug Window
                LOG(filePath);
                memoryDebugWindow.LoadMemoryProfile(filePath);
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


