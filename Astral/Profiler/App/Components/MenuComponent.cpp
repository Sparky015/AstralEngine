/**
* @file MenuComponent.cpp
* @author Andrew Fagan
* @date 3/29/26
*/

#include "MenuComponent.h"

#include "Core/Engine.h"
#include "Core/Utilities/Loggers.h"
#include "Profiler/App/ProfilerApp.h"

#include "nfd.hpp"
#include "imgui.h"

namespace Astral {


    void MenuComponent::Show()
    {
        PROFILE_SCOPE("MenuComponent::Show")

        ImGui::BeginMainMenuBar();


        if (ImGui::BeginMenu("File"))
        {

            if (ImGui::MenuItem("Load Profile"))
            {
                nfdu8char_t* outPath;
                nfdu8filteritem_t filters[1] = { { "Astral Memory Profile", "ASTLMemProfile" }};
                nfdopendialogu8args_t args = {0};
                args.filterList = filters;
                args.filterCount = 1;

                nfdresult_t result = NFD_OpenDialogU8_With(&outPath, &args);

                if (result == NFD_OKAY)
                {
                    std::string outFilePath = std::string(outPath);
                    NFD_FreePathU8(outPath);
                    ProfilerApp::Get().LoadMemoryScene(outFilePath);
                }
                else if (result == NFD_CANCEL)
                {
                    AE_LOG("Open File Dialog Canceled")
                }
                else if (result == NFD_ERROR)
                {
                    AE_WARN("NFD Error: " << NFD_GetError())
                }

            }

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }


    void MenuComponent::SelectFile(std::string& outFilePath)
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

}
