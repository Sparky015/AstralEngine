/**
* @file EditorModule.cpp
* @author Andrew Fagan
* @date 2/28/25
*/

#include "Astral.h"
#include "Editor.h"
#include "ViewportPanel.h"

#include "Input/InputState.h"

namespace Astral {

    class EditorModule : public ApplicationModule
    {
    public:
        ~EditorModule() override = default;

        void Init() override
        {
            PROFILE_SCOPE("EditorModuleInit")
            TRACE("Initializing Editor")

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