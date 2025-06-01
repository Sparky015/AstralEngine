/**
* @file EditorModule.cpp
* @author Andrew Fagan
* @date 2/28/25
*/

#include "Astral.h"
#include "Editor.h"
#include "Input/InputState.h"
#include "Core/Engine.h"
#include "Window/WindowManager.h"
#include "Debug/ImGui/ImGuiManager.h"

namespace Astral {

    class EditorModule : public ApplicationModule
    {
    public:
        ~EditorModule() override = default;

        void Init() override
        {
            PROFILE_SCOPE("EditorModuleInit")
            TRACE("Initializing Editor")

            Astral::ImGuiManager& imguiManager = Astral::Engine::Get().GetImGuiManager();
            imguiManager.LoadImGuiConfigFile(std::string(ASTRAL_EDITOR_SOURCE_DIR) + "imgui-editor-config.ini");

            Astral::Window& window = Astral::Engine::Get().GetWindowManager().GetWindow();
            window.SetWindowName("Astral Editor");
            window.SetWindowDimensions(1080, 720);
            m_Editor.Init();
        }

        void Update(const Astral::DeltaTime& deltaTime) override
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