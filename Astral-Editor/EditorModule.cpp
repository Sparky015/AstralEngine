/**
* @file EditorModule.cpp
* @author Andrew Fagan
* @date 2/28/25
*/

#include "Astral.h"
#include "Editor.h"
#include "ViewportPanel.h"
#include "Debug/ImGui/ImGuiManager.h"

#include "Input/InputState.h"
#include "Window/WindowManager.h"

namespace Astral {

    class EditorModule : public ApplicationModule
    {
    public:
        ~EditorModule() override = default;

        void Init() override
        {
            PROFILE_SCOPE("EditorModuleInit")
            TRACE("Initializing Editor")
            Debug::g_ImGuiManager.LoadImGuiConfigFile(std::string(ASTRAL_EDITOR_SOURCE_DIR) + "imgui-editor-config.ini");
            Window::g_WindowManager.GetWindow().SetWindowName("Astral Editor");
            Window::g_WindowManager.GetWindow().SetWindowDimensions(1080, 720);
            m_Editor.Init();
        }

        void Update(const Core::DeltaTime& deltaTime) override
        {

        }

        void Shutdown() override
        {
            PROFILE_SCOPE("EditorModuleShutdown")
            TRACE("Shutting down Editor")

            m_Editor.Shutdown();
        }

    private:

        Editor m_Editor;
    };

}

IMPLEMENT_APPLICATION_MODULE(Astral::EditorModule);